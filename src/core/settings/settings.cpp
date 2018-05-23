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

Settings::Settings(QObject *parent)
    : QObject(parent)
{
    // Create exposed configuration objects
    startConfigChanged(m_startConfig = new StartConfig(this));
    searchConfigChanged(m_searchConfig = new SearchConfig(this));
    themeConfigChanged(m_themeConfig = new ThemeConfig(this));

    // Check for settings directory
    if (!QDir(Paths::ConfigLocation).exists()) {
        qDebug() << "Settings path doesn't exist.";
        qDebug() << "Creating" << Paths::ConfigLocation << "...";
        QDir().mkpath(Paths::ConfigLocation);
    }
    // Check for settings file
    if (!QFile(Paths::SettingsFile).exists()) {
        qDebug() << "Settings file doesn't exist.";
        qDebug() << "Creating" << Paths::SettingsFile << "...";
        QFile file(Paths::SettingsFile);
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
    QFile file(Paths::SettingsFile);
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
    m_startConfig->setPrimaryStartUrl(QUrl(dataStart["primaryUrl"].toString()));
    m_startConfig->setDarkStartUrl(QUrl(dataStart["darkThemeUrl"].toString()));
    m_startConfig->setIncognitoStartUrl(QUrl(dataStart["incognitoUrl"].toString()));

    StartConfig::StartupType startupType;
    QString startupTypeString = dataStart["startupType"].toString();

    if (startupTypeString == "StartFromNewPage")
        startupType = StartConfig::StartupType::StartFromNewPage;
    else if (startupTypeString == "StartFromPreviouslyOpenedTabs")
        startupType = StartConfig::StartupType::StartFromPreviouslyOpenedTabs;

    m_startConfig->setStartupType(startupType);
    m_startConfig->setPersistentCookies(dataStart["persistentCookies"].toBool());

    QJsonObject dataSearch = data["search"].toObject();
    QString searchEngineString = dataSearch["engine"].toString();
    SearchConfig::SearchEngine searchEngine;
    if (searchEngineString == "DuckDuckGo")
        searchEngine = SearchConfig::SearchEngine::DuckDuckGo;
    else if (searchEngineString == "Google")
        searchEngine = SearchConfig::SearchEngine::Google;
    else if (searchEngineString == "Bing")
        searchEngine = SearchConfig::SearchEngine::Bing;
    else if (searchEngineString == "Yahoo")
        searchEngine = SearchConfig::SearchEngine::Yahoo;
    else if (searchEngineString == "StartPage")
        searchEngine = SearchConfig::SearchEngine::StartPage;
    else
        searchEngine = SearchConfig::SearchEngine::Custom;
    m_searchConfig->setSearchEngine(searchEngine);
    m_searchConfig->setCustomSearchUrl(QUrl(dataSearch["customUrl"].toString()));
    QJsonObject dataTheme = data["theme"].toObject();
    m_themeConfig->setThemeColorEnabled(dataTheme["adaptWebsiteTheme"].toBool());
    m_themeConfig->setDarkThemeEnabled(dataTheme["darkThemeEnabled"].toBool());
    m_themeConfig->setDarkThemeStartTime(QTime::fromString(
        dataTheme["darkThemeStartTime"].toString(),
        "HH:mm"
    ));
    m_themeConfig->setDarkThemeEndTime(QTime::fromString(
        dataTheme["darkThemeEndTime"].toString(),
        "HH:mm"
    ));
    setDirty(false);
    file.close();
}

void Settings::save()
{
    QFile file(Paths::SettingsFile);
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
        {"primaryUrl", m_startConfig->defaultPrimaryStartUrl().toString()},
        {"darkThemeUrl", m_startConfig->defaultDarkStartUrl().toString()},
        {"incognitoUrl", m_startConfig->defaultIncognitoStartUrl().toString()},
        {"startupType", "StartFromNewPage"},
        {"persistentCookies", true}
    };
    QJsonObject dataSearch {
        {"engine", "DuckDuckGo"},
        {"customUrl", ""}
    };
    QJsonObject dataTheme {
        {"adaptWebsiteTheme", true},
        {"darkThemeEnabled", false},
        {"darkThemeStartTime", "21:00"},
        {"darkThemeEndTime", "07:00"}
    };
    QJsonObject data {
        {"start", dataStart},
        {"search", dataSearch},
        {"theme", dataTheme}
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

    QString startupTypeString;

    switch (m_startConfig->startupType()) {
        case StartConfig::StartupType::StartFromNewPage:
            startupTypeString = "StartFromNewPage";
            break;
        case StartConfig::StartupType::StartFromPreviouslyOpenedTabs:
            startupTypeString = "StartFromPreviouslyOpenedTabs";
            break;
    }

    QJsonObject dataStart {
        {"primaryUrl", m_startConfig->primaryStartUrl().toString()},
        {"darkThemeUrl", m_startConfig->darkStartUrl().toString()},
        {"incognitoUrl", m_startConfig->incognitoStartUrl().toString()},
        {"startupType", startupTypeString},
        {"persistentCookies", m_startConfig->persistentCookies()},
    };

    QString searchEngineString;
    switch (m_searchConfig->searchEngine()) {
        case SearchConfig::SearchEngine::DuckDuckGo:
            searchEngineString = "DuckDuckGo";
            break;
        case SearchConfig::SearchEngine::Google:
            searchEngineString = "Google";
            break;
        case SearchConfig::SearchEngine::Bing:
            searchEngineString = "Bing";
            break;
        case SearchConfig::SearchEngine::Yahoo:
            searchEngineString = "Yahoo";
            break;
        case SearchConfig::SearchEngine::StartPage:
            searchEngineString = "StartPage";
            break;
        default:
            searchEngineString = "Custom";
            break;
    }
    QJsonObject dataSearch {
        {"engine", searchEngineString},
        {"customUrl", m_searchConfig->customSearchUrl().toString()}
    };
    QJsonObject dataTheme {
        {"adaptWebsiteTheme", m_themeConfig->themeColorEnabled()},
        {"darkThemeEnabled", m_themeConfig->darkThemeEnabled()},
        {"darkThemeStartTime", m_themeConfig->darkThemeStartTime().toString("HH:mm")},
        {"darkThemeEndTime", m_themeConfig->darkThemeEndTime().toString("HH:mm")}
    };
    QJsonObject data {
        {"start", dataStart},
        {"search", dataSearch},
        {"theme", dataTheme}
    };
    QJsonObject root {
        {"meta", meta},
        {"data", data}
    };
    QJsonDocument doc(root);
    return doc.toJson();
}
