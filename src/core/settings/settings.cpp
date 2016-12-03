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

#include "settings.h"

#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

Settings::Settings(QObject *parent) : QObject(parent)
{
    // Create exposed configuration objects
    startConfigChanged(m_startConfig = new StartConfig(this));
    searchConfigChanged(m_searchConfig = new SearchConfig(this));

    // Check for settings directory
    if (!QDir(APP_CONFIG_LOCATION).exists()) {
        qDebug() << "Settings path doesn't exist.";
        qDebug() << "Creating" << APP_CONFIG_LOCATION << "...";
        QDir().mkpath(APP_CONFIG_LOCATION);
    }
    // Check for settings file
    if (!QFile(SETTINGS_FILENAME).exists()) {
        qDebug() << "Settings file doesn't exist.";
        qDebug() << "Creating" << SETTINGS_FILENAME << "...";
        QFile file(SETTINGS_FILENAME);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open settings file for write!");
        }
        else {
            QTextStream stream(&file);
            stream << defaultJSON();
            qDebug() << "Default settings file created.";
            file.close();
        }
    }
}

void Settings::load()
{
    QFile file(SETTINGS_FILENAME);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open settings file for read!");
        return;
    }

    QByteArray bytes = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(bytes));

    QJsonObject root = doc.object();
    QJsonObject meta = root["meta"].toObject();
    QString metaSchema = meta["schema"].toString();
    if (metaSchema != "0.1") {
        qWarning() << "Unknown settings schema version " << metaSchema  << "!";
        return;
    }
    QJsonObject data = root["data"].toObject();
    QJsonObject dataStart = data["start"].toObject();
    m_startConfig->setStartUrl(QUrl(dataStart["url"].toString()));
    QJsonObject dataSearch = data["search"].toObject();
    QString searchEngineString = dataSearch["engine"].toString();
    SearchConfig::SearchEngine searchEngine;
    if (searchEngineString == "duckduckgo")
        searchEngine = SearchConfig::SearchEngine::DuckDuckGo;
    else if (searchEngineString == "google")
        searchEngine = SearchConfig::SearchEngine::Google;
    else if (searchEngineString == "bing")
        searchEngine = SearchConfig::SearchEngine::Bing;
    else if (searchEngineString == "yahoo")
        searchEngine = SearchConfig::SearchEngine::Yahoo;
    else
        searchEngine = SearchConfig::SearchEngine::Custom;
    m_searchConfig->setSearchEngine(searchEngine);
    m_searchConfig->setCustomSearchUrl(QUrl(dataSearch["custom_url"].toString()));
    setDirty(false);
    file.close();
}

void Settings::save()
{
    QFile file(SETTINGS_FILENAME);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open settings file for write!");
        return;
    }
    QTextStream stream(&file);
    stream << json();
    setDirty(false);
    file.close();
}

QByteArray Settings::defaultJSON()
{
    QJsonObject meta {
        {"schema", "0.1"}
    };
    QJsonObject dataStart {
        {"url", m_startConfig->defaultStartUrl().toString()},
    };
    QJsonObject dataSearch {
        {"engine", "duckduckgo"},
        {"custom_url", ""}
    };
    QJsonObject data {
        {"start", dataStart},
        {"search", dataSearch}
    };
    QJsonObject root {
        {"meta", meta},
        {"data", data}
    };
    QJsonDocument doc(root);
    return doc.toJson();
}

QByteArray Settings::json()
{
    QJsonObject meta {
        {"schema", "0.1"}
    };
    QJsonObject dataStart {
        {"url", m_startConfig->startUrl().toString()},
    };
    QString searchEngineString;
    switch (m_searchConfig->searchEngine()) {
        case SearchConfig::SearchEngine::DuckDuckGo:
            searchEngineString = "duckduckgo";
            break;
        case SearchConfig::SearchEngine::Google:
            searchEngineString = "google";
            break;
        case SearchConfig::SearchEngine::Bing:
            searchEngineString = "bing";
            break;
        case SearchConfig::SearchEngine::Yahoo:
            searchEngineString = "yahoo";
            break;
        default:
            searchEngineString = "custom";
            break;
    }
    QJsonObject dataSearch {
        {"engine", searchEngineString},
        {"custom_url", m_searchConfig->customSearchUrl().toString()}
    };
    QJsonObject data {
        {"start", dataStart},
        {"search", dataSearch}
    };
    QJsonObject root {
        {"meta", meta},
        {"data", data}
    };
    QJsonDocument doc(root);
    return doc.toJson();
}
