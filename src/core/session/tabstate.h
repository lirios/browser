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

#ifndef TABSTATE_H
#define TABSTATE_H

#include <QObject>

class TabState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)
    Q_PROPERTY(float readingProgress READ readingProgress WRITE setReadingProgress)

public:
    explicit TabState(QObject *parent = 0);

    void setUrl(QString url);
    QString url() const;
    void setTitle(QString title);
    QString title() const;
    void setIcon(QString icon);
    QString icon() const;
    float readingProgress() const;
    void setReadingProgress(float readingProgress);

signals:

public slots:

private:
    QString m_url;
    QString m_title;
    QString m_icon;
    float m_readingProgress;
};

#endif // TABSTATE_H
