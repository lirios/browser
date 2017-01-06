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

#ifndef EXTENSIONSMANAGER_H
#define EXTENSIONSMANAGER_H

#include <QObject>
#include <QTextStream>
#include <QFileSystemWatcher>
#include <QDir>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#include "../global/paths.h"
#include "extension.h"
#include "extensionparser.h"
#include "extensionsmodel.h"
#include "extensionthemesmodel.h"
#include "extensionsearchenginesmodel.h"

class ExtensionsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ExtensionsModel* extensionsModel MEMBER m_extensionsModel NOTIFY extensionsModelChanged)
    Q_PROPERTY(ExtensionThemesModel* themesModel MEMBER m_extensionThemesModel NOTIFY extensionThemesModelChanged)
    Q_PROPERTY(ExtensionSearchEnginesModel* searchEnginesModel MEMBER m_extensionSearchEnginesModel NOTIFY extensionSearchEnginesModelChanged)
public:
    explicit ExtensionsManager(QObject *parent = nullptr);

    void loadBuiltins();
    void startWatching();
    void scan();

    void addExtension(Extension* extension);
    void removeExtension(Extension* extension);
    Extension* extensionByName(const QString name);

    void loadFile(const QString filePath);
    void unloadFile(const QString filePath);

    ExtensionThemesModel* themesModel() const { return m_extensionThemesModel; }

signals:
    void extensionsModelChanged(ExtensionsModel* model);
    void extensionThemesModelChanged(ExtensionThemesModel* model);
    void extensionSearchEnginesModelChanged(ExtensionSearchEnginesModel* model);

private:
    ExtensionsModel* m_extensionsModel;
    ExtensionThemesModel* m_extensionThemesModel;
    ExtensionSearchEnginesModel* m_extensionSearchEnginesModel;
    QFileSystemWatcher* m_watcher;

    bool m_loaded;

    QList<QString> m_files;
    QDir m_dir;


private slots:
    void directoryChanged(const QString path);
    void fileAdded(const QString filename);
    void fileRemoved(const QString filename);
};

#endif // EXTENSIONSMANAGER_H
