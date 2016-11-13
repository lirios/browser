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
    setDirty(false);
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
}

QByteArray Settings::defaultJSON()
{
    QJsonObject meta {
        {"schema", "0.1"}
    };
    QJsonObject dataStart {
        {"url", m_startConfig->defaultStartUrl().toString()},
    };
    QJsonObject data {
        {"start", dataStart}
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
    QJsonObject data {
        {"start", dataStart}
    };
    QJsonObject root {
        {"meta", meta},
        {"data", data}
    };
    QJsonDocument doc(root);
    return doc.toJson();
}
