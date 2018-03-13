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

#include "session.h"

#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../global/paths.h"
#include "../models/tabsmodel.h"
#include "tabstate.h"

Session::Session(QObject *parent)
    : QObject(parent)
    , m_activeTab(0)
{
    load();
}

void Session::save(TabsModel* tabs)
{
    QFile file(Paths::SessionDataFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open session file for write!");
        return;
    }
    QTextStream stream(&file);
    stream << json(tabs);
    file.close();

    qDebug() << "Session written to" << Paths::SessionDataFile;
}

QVariantList Session::getTabsToRestore()
{
    QVariantList tabs;
    for (TabState* state : m_tabs)
    {
        tabs.append(QVariant::fromValue(state));
    }
    m_tabs.clear();
    return tabs;
}

void Session::load()
{
    QFile file(Paths::SessionDataFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open session file for read!");
        return;
    }

    QByteArray bytes = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(bytes));

    QJsonObject root = doc.object();
    QJsonObject meta = root["meta"].toObject();
    QString metaSchema = meta["schema"].toString();
    if (metaSchema != "0.1") {
        qWarning() << "Unknown session schema version " << metaSchema  << "!";
        return;
    }
    QJsonArray tabs = root["tabs"].toArray();
    for (QJsonValue tab : tabs)
    {
        auto tabObj = tab.toObject();
        auto state = new TabState(this);
        state->setUrl(tabObj["url"].toString());
        state->setTitle(tabObj["title"].toString());
        state->setIcon(tabObj["icon"].toString());
        m_tabs.append(state);
    }

    m_activeTab = std::max(m_tabs.count() - 1, 0);
    if (root.find("activeTab") != root.end())
    {
        m_activeTab = root["activeTab"].toInt();
    }
}

QByteArray Session::json(TabsModel* tabs)
{
    QJsonObject meta {
        {"schema", "0.1"}
    };

    QJsonArray tabsArray;

    for (int i = 0; i < tabs->count(); ++i) {
        QJsonObject tabObject;
        auto tab = tabs->get(i);
        tabObject["url"] = tab->url().toString();
        tabObject["title"] = tab->title();
        tabObject["icon"] = tab->iconUrl().toString().replace("image://favicon/","");
        tabObject["readingProgress"] = 0.f;
        tabsArray.append(tabObject);
    }

    QJsonObject root {
        {"meta", meta},
        {"tabs", tabsArray},
        {"activeTab", tabs->activeIndex()},
    };

    QJsonDocument doc(root);
    return doc.toJson();
}
