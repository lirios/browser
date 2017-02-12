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


ExtensionsManager::ExtensionsManager(QObject *parent)
    : QObject(parent)
{
    m_dir = QDir(Paths::ExtensionsLocation);

    // Check for paths
    if (!QDir(m_dir).exists()) {
        qDebug() << "Extensions location doesn't exist.";
        qDebug() << "Creating" << Paths::ExtensionsLocation << "...";
        QDir().mkpath(Paths::ExtensionsLocation);
    }

    // Create models
    m_extensionsModel = new ExtensionsModel(this);
    extensionsModelChanged(m_extensionsModel);
    m_extensionThemesModel = new ExtensionThemesModel(this);
    extensionThemesModelChanged(m_extensionThemesModel);
    m_extensionSearchEnginesModel = new ExtensionSearchEnginesModel(this);
    extensionSearchEnginesModelChanged(m_extensionSearchEnginesModel);

    // Create watcher
    m_watcher = new QFileSystemWatcher(this);
}

void ExtensionsManager::loadBuiltins()
{
    Extension* extension = new Extension(this);
    ExtensionParser parser;
    parser.setExtension(extension);
    if (!parser.open(QUrl("qrc://extensions/default"), ExtensionType::QRC))
        return;
    if (!parser.load()) {
        extension->deleteLater();
        parser.close();
        return;
    }
    parser.close();
    parser.deleteLater();
    addExtension(extension);
}

void ExtensionsManager::startWatching()
{
    connect(m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &ExtensionsManager::directoryChanged);
    m_watcher->addPath(Paths::ExtensionsLocation);
}

void ExtensionsManager::scan()
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

void ExtensionsManager::addExtension(Extension *extension)
{
    // Add extension themes to theme model
    for (ExtensionTheme* theme : extension->themes()) {
        m_extensionThemesModel->add(theme->clone(this));
    }

    // Add search engines to search engines model
    for (ExtensionSearchEngine* searchEngine : extension->searchEngines()) {
        m_extensionSearchEnginesModel->add(searchEngine->clone(this));
    }

    // Add extension to extensions model
    m_extensionsModel->add(extension);
}

void ExtensionsManager::removeExtension(Extension *extension)
{
    // Remove from model
    m_extensionsModel->remove(extension);
    // Remove themes from theme model
    m_extensionThemesModel->removeFromExtensionName(extension->name());
    // Delete object
    extension->deleteLater();
    qDebug() << "Extension unloaded.";
}

Extension *ExtensionsManager::extensionByName(const QString name)
{
    return m_extensionsModel->get(name);
}

void ExtensionsManager::loadFile(const QString filePath)
{
    qDebug() << "Loading extension file" << filePath << "...";

    // Check for duplicate name
    QString name = filePath.section("/", -1).section(".", 0, 0);
    if (m_extensionsModel->hasName(name)) {
        qWarning() << "There is already an extension with name" << name << "registered.";
        qWarning() << "Stopped loading extension.";
        return;
    }

    Extension* extension = new Extension(this);

    ExtensionParser parser;
    parser.setExtension(extension);
    if(!parser.open(QUrl(filePath), ExtensionType::LBX)) {
        return;
    }
    if (!parser.load()) {
        extension->deleteLater();
        parser.close();
        return;
    }
    parser.close();
    parser.deleteLater();
    addExtension(extension);
}

void ExtensionsManager::unloadFile(const QString filePath)
{
    qDebug() << "Unloading extension file" << filePath << "...";
    QString name = filePath.section("/", -1).section(".", 0, 0);
    Extension* extension = m_extensionsModel->get(name);
    removeExtension(extension);
}

void ExtensionsManager::directoryChanged(const QString path)
{
    Q_UNUSED(path)
    Q_ASSERT(path == Paths::ExtensionsLocation);
    scan();
}

void ExtensionsManager::fileAdded(const QString filename)
{
    qDebug() << "Extension file" << filename << "added.";
    m_files.append(filename);
    loadFile(Paths::ExtensionsLocation + filename);
}

void ExtensionsManager::fileRemoved(const QString filename)
{
    qDebug() << "Extension file" << filename << "removed.";
    m_files.removeAll(filename);
    unloadFile(Paths::ExtensionsLocation + filename);
}
