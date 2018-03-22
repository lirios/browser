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

#include "startconfig.h"

StartConfig::StartConfig(QObject *parent)
    : QObject(parent)
{
    m_defaultPrimaryStartUrl = QUrl("https://duckduckgo.com");
    m_defaultDarkStartUrl = QUrl("https://duckduckgo.com/?kae=#303030");
    m_defaultIncognitoStartUrl = QUrl("https://duckduckgo.com/?kae=#37474f");
    m_startupType = StartupType::StartFromNewPage;
    m_persistentCookies = true;
}
