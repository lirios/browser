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

#ifndef VERSION_H
#define VERSION_H

#include <QVersionNumber>
#include <QString>

namespace Version {
    const int major = 0;
    const int minor = 4;
    const int update = 0;
    const bool isRelease = false;
    const QString fullString = QString("v%1.%2.%3-%4").arg(major).arg(minor).arg(update).arg(isRelease ? "release" : "devel");
    const QVersionNumber version = QVersionNumber(major, minor, update);
}

#endif // VERSION_H
