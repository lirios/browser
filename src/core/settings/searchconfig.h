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

#ifndef SEARCHCONFIG_H
#define SEARCHCONFIG_H

#include <QObject>
#include <QUrl>

class SearchConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString searchEngine READ searchEngine WRITE setSearchEngine NOTIFY searchEngineChanged)
    Q_PROPERTY(QUrl customSearchUrl READ customSearchUrl WRITE setCustomSearchUrl NOTIFY customSearchUrlChanged)
    Q_ENUMS(SearchEngine)
public:
    explicit SearchConfig(QObject *parent = nullptr);

    QString searchEngine() const { return m_searchEngine; }
    void setSearchEngine(QString searchEngine) { searchEngineChanged(m_searchEngine = searchEngine); }

    QUrl customSearchUrl() const { return m_customSearchUrl; }
    void setCustomSearchUrl(QUrl customSearchUrl) { customSearchUrlChanged(m_customSearchUrl = customSearchUrl); }

signals:
    void searchEngineChanged(QString searchEngine);
    void searchUrlChanged(QUrl searchUrl);
    void customSearchUrlChanged(QUrl customSearchUrl);

private:
    QString m_searchEngine;
    QUrl m_searchUrl;
    QUrl m_customSearchUrl;
};

#endif // SEARCHCONFIG_H
