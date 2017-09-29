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

#ifndef DARKTHEMETIMER_H
#define DARKTHEMETIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class DarkThemeTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QTime endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(bool isActiveTime MEMBER m_isActiveTime NOTIFY isActiveTimeChanged)
public:
    explicit DarkThemeTimer(QObject *parent = nullptr);

    QTime startTime() const { return m_startTime; }
    void setStartTime(QTime startTime) { startTimeChanged(m_startTime = startTime); }

    QTime endTime() const { return m_endTime; }
    void setEndTime(QTime endTime) { endTimeChanged(m_endTime = endTime); }

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void startTimeChanged(QTime startTime);
    void endTimeChanged(QTime endTime);
    void isActiveTimeChanged(bool isActiveTime);

private:
    QTime m_startTime;
    QTime m_endTime;
    QTimer* m_timer;
    bool m_isActiveTime;

public slots:
    Q_INVOKABLE void update();

};

#endif // DARKTHEMETIMER_H
