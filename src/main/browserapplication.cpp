/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2018 Tim Süberkrüb <dev@timsueberkrueb.io>
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

#include <QDebug>
#include <QList>

#include "browserapplication.h"

BrowserApplication::BrowserApplication(QObject *parent)
    : QObject(parent)
{

}

void BrowserApplication::load()
{
    // Load settings
    m_settings.load();

    // Start dark theme time
    m_darkThemeTimer.start();

    // Load translations
    BrowserApplication::loadTranslations();

    // register core types
    qmlRegisterUncreatableType<SearchConfig>("core", 1, 0, "SearchConfig", "SearchConfig (from module core) may not be created directly.");
    qmlRegisterUncreatableType<StartConfig>("core", 1, 0, "StartConfig", "StartConfig (from module core) may not be created directly.");

    qmlRegisterUncreatableType<Tab>("core", 1, 0, "Tab", "Tab (from module core) may not be created directly.");
    qmlRegisterType<TabsModel>("core", 1, 0, "TabsModel");

    qmlRegisterType<DownloadsModel>("core", 1, 0, "DownloadsModel");

    #ifdef Q_OS_MACOS
        initMacOsEventListener(&m_evListener);
    #endif

    // Register context properties
    m_engine.rootContext()->setContextProperty("Settings", &m_settings);
    m_engine.rootContext()->setContextProperty("Session", &m_session);
    m_engine.rootContext()->setContextProperty("DarkThemeTimer", &m_darkThemeTimer);
    #ifdef Q_OS_MACOS
        m_engine.rootContext()->setContextProperty("MacEvents", &m_evListener);
    #endif

    // setup qml imports
    m_engine.addImportPath("qrc:/");

    // load main ui
    m_engine.load(QUrl(QLatin1String("qrc:/ui/Main.qml")));

    m_loaded = true;
}

void BrowserApplication::openUrl(const QUrl &url, bool incognito)
{
    if (!m_loaded) {
        qWarning() << "Ignoring request to open url because the application is not loaded yet";
        return;
    }
    QMetaObject::invokeMethod(m_engine.rootObjects()[0], "openUrl",
            Q_ARG(QVariant, QVariant(url)), Q_ARG(QVariant, QVariant(incognito)));
}

void BrowserApplication::openStartUrl(bool incognito)
{
    if (!m_loaded) {
        qCritical() << "Cannot open start url as the application is not loaded yet";
        return;
    }
    QMetaObject::invokeMethod(m_engine.rootObjects()[0], "openNewWindow",
            Q_ARG(QVariant, QVariant(incognito)), Q_ARG(QVariant, QVariant(true)));
}

void BrowserApplication::onMessageReceived(const QString &message)
{
    QStringList fragments = message.split(' ');

    if (message == QStringLiteral("heartbeat")) {
        // Heartbeat received, doing nothing.
    } else if (fragments[0] == QStringLiteral("open")) {
        if (fragments.count() >= 2) {
            auto url = QUrl(fragments[1]);
            openUrl(url);
        } else {
            openStartUrl();
        }
    } else if (fragments[0] == QStringLiteral("open_incognito")) {
        if (fragments.count() >= 2) {
            auto url = QUrl(fragments[1]);
            openUrl(url, true);
        } else {
            openStartUrl(true);
        }
    } else {
        qWarning() << "Received invalid message" << message;
    }
}
