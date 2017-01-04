/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <tim.sueberkrueb@web.de>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
*/

#include "extensionsmanager.h"

#include <QDebug>


ExtensionsManager::ExtensionsManager(QObject *parent) : QObject(parent)
{
    m_dir = QDir(Paths::ExtensionsLocation);

    // Only load once
    m_loaded = false;
    // Check for paths
    if (!QDir(m_dir).exists()) {
        qDebug() << "Extensions location doesn't exist.";
        qDebug() << "Creating" << Paths::ExtensionsLocation << "...";
        QDir().mkpath(Paths::ExtensionsLocation);
    }
    // Create watcher
    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &ExtensionsManager::directoryChanged);
    m_watcher->addPath(Paths::ExtensionsLocation);

    // Create models
    m_extensionsModel = new ExtensionsModel(this);
    extensionsModelChanged(m_extensionsModel);
    m_extensionThemesModel = new ExtensionThemesModel(this);
    extensionThemesModelChanged(m_extensionThemesModel);

    // Find files
    findFiles();
}

void ExtensionsManager::loadFile(const QString filePath)
{
    qDebug() << "Loading extension file" << filePath << "...";

    Extension* extension = new Extension(this);

    ExtensionParser parser;
    parser.setExtension(extension);
    if(!parser.open(filePath)) {
        return;
    }
    if (!parser.load()) {
        extension->deleteLater();
        parser.close();
        return;
    }
    parser.close();
    parser.deleteLater();
    // Add extension themes to theme model
    for (ExtensionTheme* theme : extension->themes()) {
        m_extensionThemesModel->add(theme->clone(this));
    }
    m_extensionsModel->add(extension);
}

void ExtensionsManager::unloadFile(const QString filePath)
{
    qDebug() << "Unloading extension file" << filePath << "...";
    QString name = filePath.section("/", -1).section(".", 0, 0);
    Extension* extension = m_extensionsModel->get(name);
    m_extensionsModel->remove(extension);
    // Remove themes from theme model
    m_extensionThemesModel->removeFromExtension(name);
    extension->deleteLater();
    qDebug() << "Extension unloaded.";
}

void ExtensionsManager::findFiles()
{
    QStringList nameFilters;
    nameFilters << "*.zip";
    nameFilters << "*.lbx";
    QStringList files = m_dir.entryList(nameFilters, QDir::Files);
    for (QString filename : files) {
        if (!m_files.contains(filename)) {
            // New file detected
            fileAdded(filename);
        }
    }
    for (QString filename : m_files) {
        if (!files.contains(filename)) {
            // Removed file detected
            fileRemoved(filename);
        }
    }
}

void ExtensionsManager::directoryChanged(QString path)
{
    Q_UNUSED(path)
    findFiles();
}

void ExtensionsManager::fileAdded(QString filename)
{
    qDebug() << "Extension file" << filename << "added.";
    m_files.append(filename);
    loadFile(Paths::ExtensionsLocation + filename);
}

void ExtensionsManager::fileRemoved(QString filename)
{
    qDebug() << "Extension file" << filename << "removed.";
    m_files.removeAll(filename);
    unloadFile(Paths::ExtensionsLocation + filename);
}
