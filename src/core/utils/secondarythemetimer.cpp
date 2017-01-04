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

#include "secondarythemetimer.h"

SecondaryThemeTimer::SecondaryThemeTimer(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(60000);
    connect(m_timer, &QTimer::timeout, this, &SecondaryThemeTimer::update);
}

void SecondaryThemeTimer::start()
{
    m_timer->start();
}

void SecondaryThemeTimer::stop()
{
    m_timer->stop();
}

void SecondaryThemeTimer::update()
{
    bool isDark = false;
    QTime now = QTime::currentTime();
    if (m_endTime > m_startTime) {
        isDark = (now > m_startTime && now < m_endTime);
    }
    else {
        isDark = (now > m_startTime || now < m_endTime );
    }
    isActiveTimeChanged(m_isActiveTime = isDark);
}
