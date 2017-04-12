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
    m_startConfig->setPrimaryStartUrl(QUrl(dataStart["primary_url"].toString()));
    m_startConfig->setDarkStartUrl(QUrl(dataStart["dark_theme_url"].toString()));
    m_startConfig->setIncognitoStartUrl(QUrl(dataStart["incognito_url"].toString()));

    StartConfig::StartupType startupType;
    QString startupTypeString = dataStart["startupType"].toString();

    if (startupTypeString == "start_from_new_page")
        startupType = StartConfig::StartupType::StartFromNewPage;
    else if (startupTypeString == "start_from_previously_opened_tabs")
        startupType = StartConfig::StartupType::StartFromPreviouslyOpenedTabs;

    m_startConfig->setStartupType(startupType);

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
    QJsonObject dataTheme = data["theme"].toObject();
    m_themeConfig->setThemeColorEnabled(dataTheme["adapt_website_theme"].toBool());
    m_themeConfig->setDarkThemeEnabled(dataTheme["dark_theme_enabled"].toBool());
    m_themeConfig->setDarkThemeStartTime(QTime::fromString(
        dataTheme["dark_theme_start_time"].toString(),
        "HH:mm"
    ));
    m_themeConfig->setDarkThemeEndTime(QTime::fromString(
        dataTheme["dark_theme_end_time"].toString(),
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
        {"primary_url", m_startConfig->defaultPrimaryStartUrl().toString()},
        {"dark_theme_url", m_startConfig->defaultDarkStartUrl().toString()},
        {"incognito_url", m_startConfig->defaultIncognitoStartUrl().toString()},
        {"startupType", "start_from_new_page"}
    };
    QJsonObject dataSearch {
        {"engine", "duckduckgo"},
        {"custom_url", ""}
    };
    QJsonObject dataTheme {
        {"adapt_website_theme", true},
        {"dark_theme_enabled", false},
        {"dark_theme_start_time", "21:00"},
        {"dark_theme_end_time", "07:00"}
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
            startupTypeString = "start_from_new_page";
            break;
        case StartConfig::StartupType::StartFromPreviouslyOpenedTabs:
            startupTypeString = "start_from_previously_opened_tabs";
            break;
    }

    QJsonObject dataStart {
        {"primary_url", m_startConfig->primaryStartUrl().toString()},
        {"dark_theme_url", m_startConfig->darkStartUrl().toString()},
        {"incognito_url", m_startConfig->incognitoStartUrl().toString()},
        {"startupType", startupTypeString},
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
    QJsonObject dataTheme {
        {"adapt_website_theme", m_themeConfig->themeColorEnabled()},
        {"dark_theme_enabled", m_themeConfig->darkThemeEnabled()},
        {"dark_theme_start_time", m_themeConfig->darkThemeStartTime().toString("HH:mm")},
        {"dark_theme_end_time", m_themeConfig->darkThemeEndTime().toString("HH:mm")}
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
