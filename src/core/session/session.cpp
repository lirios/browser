#include "Session.h"

#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../global/paths.h"
#include "../models/tabsmodel.h"
#include "tabstate.h"

Session::Session(QObject *parent) : QObject(parent)
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
        m_tabs.append(state);
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
        tabObject["url"] = tabs->get(i)->url().toString();
        tabObject["readingProgress"] = 0.f;
        tabsArray.append(tabObject);
    }

    QJsonObject root {
        {"meta", meta},
        {"tabs", tabsArray},
    };

    QJsonDocument doc(root);
    return doc.toJson();
}
