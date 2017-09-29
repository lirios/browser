/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <dev@timsueberkrueb.io>
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
 */

#ifndef PATHS_H
#define PATHS_H

#include <QString>
#include <QStandardPaths>

namespace Paths {
    const QString ConfigLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/liri-browser/";
    const QString DataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/liri-browser/";
    const QString SettingsFile = ConfigLocation + "settings.json";
}

#endif // PATHS_H
