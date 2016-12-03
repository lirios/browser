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

#ifndef STARTCONFIG_H
#define STARTCONFIG_H

#include <QObject>
#include <QUrl>

class StartConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl startUrl READ startUrl WRITE setStartUrl NOTIFY startUrlChanged)
    Q_PROPERTY(QUrl defaultStartUrl MEMBER m_defaultStartUrl NOTIFY defaultStartUrlChanged)
public:
    explicit StartConfig(QObject *parent = nullptr);

    QUrl startUrl() const { return m_startUrl; }
    void setStartUrl(QUrl url) { startUrlChanged(m_startUrl = url); }

    QUrl defaultStartUrl() const { return m_defaultStartUrl; }

signals:
    void startUrlChanged(QUrl url);
    void defaultStartUrlChanged(QUrl url);

private:
    QUrl m_startUrl;
    QUrl m_defaultStartUrl;

};

#endif // STARTCONFIG_H
