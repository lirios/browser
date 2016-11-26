/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb <tim.sueberkrueb@web.de>
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QStandardPaths>

#include "startconfig.h"
#include "searchconfig.h"

const QString APP_CONFIG_LOCATION = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/liri-browser/";
const QString SETTINGS_FILENAME = APP_CONFIG_LOCATION + "settings.json";

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StartConfig* startConfig MEMBER m_startConfig NOTIFY startConfigChanged)
    Q_PROPERTY(SearchConfig* searchConfig MEMBER m_searchConfig NOTIFY searchConfigChanged)
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
public:
    explicit Settings(QObject *parent = 0);

    bool dirty() const { return m_dirty; }
    void setDirty(bool dirty) { dirtyChanged(m_dirty = dirty); }

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

    QByteArray defaultJSON();
    QByteArray json();

private:
    StartConfig* m_startConfig;
    SearchConfig* m_searchConfig;
    bool m_dirty;

signals:
    void startConfigChanged(StartConfig* startConfig);
    void searchConfigChanged(SearchConfig* searchConfig);
    void dirtyChanged(bool dirty);
};

#endif // SETTINGS_H
