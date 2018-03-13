/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Ivan Fateev <johnny.j.poison@gmail.com>
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


#include "tabstate.h"

TabState::TabState(QObject *parent) : QObject(parent)
{

}

void TabState::setUrl(QString url)
{
    if (m_url == url)
        return;

    m_url = url;
}

QString TabState::url() const
{
    return m_url;
}

void TabState::setTitle(QString title)
{
    m_title = title;
}

QString TabState::title() const
{
    return m_title;
}

void TabState::setIcon(QString icon)
{
   m_icon = icon;
}

QString TabState::icon() const
{
    return m_icon;
}

float TabState::readingProgress() const
{
    return m_readingProgress;
}

void TabState::setReadingProgress(float readingProgress)
{
    m_readingProgress = readingProgress;
}
