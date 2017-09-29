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

#include <QtGlobal>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlContext>
#include <QtWebEngine>
#include <QStandardPaths>
#ifndef QT_NO_TRANSLATION
    #include <QtCore/QTranslator>
#endif
#include <QDebug>

#include "../core/models/tabsmodel.h"
#include "../core/models/tab.h"
#include "../core/models/downloadsmodel.h"
#include "../core/settings/settings.h"
#include "../core/utils/darkthemetimer.h"

#ifdef Q_OS_MACOS
    #include "mac/MacOsEventListener.h"
#endif

static void loadTranslations()
{
    #ifndef QT_NO_TRANSLATION
        QString locale = QLocale::system().name();

        // Find the translations directory
        const QString path = QLatin1String("liri-browser/translations");
        const QString translationsDir =
            QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                   path,
                                   QStandardPaths::LocateDirectory);

        // Load translations
        QTranslator *appTranslator = new QTranslator(qGuiApp);
        if (appTranslator->load(QStringLiteral("%1/browser_%3").arg(translationsDir, locale))) {
            QCoreApplication::installTranslator(appTranslator);
        } else if (locale == QLatin1String("C") ||
                    locale.startsWith(QLatin1String("en"))) {
            // English is the default, it's translated anyway
            delete appTranslator;
        }
    #endif
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle(QLatin1String("Material"));

    QGuiApplication app(argc, argv);

    // Set app info
    app.setOrganizationName(QStringLiteral("Liri"));
    app.setOrganizationDomain(QStringLiteral("liri.io"));
    app.setApplicationName(QStringLiteral("Browser"));
    app.setDesktopFileName(QStringLiteral("io.liri.Browser.desktop"));
    app.setWindowIcon(QIcon(":/res/icons/512x512/io.liri.Browser.png"));

    #ifdef Q_OS_MACOS
        MacOsEventListener evListener;
        initMacOsEventListener(&evListener);
    #endif

    QtWebEngine::initialize();

    // Create settings instance and load
    Settings settings;
    settings.load();

    // Create and start dark theme time
    DarkThemeTimer darkThemeTimer;
    darkThemeTimer.start();

    // Load translations
    loadTranslations();

    // create qml app engine
    QQmlApplicationEngine engine;

    // register core types
    qmlRegisterUncreatableType<SearchConfig>("core", 1, 0, "SearchConfig", "SearchConfig (from module core) may not be created directly.");

    qmlRegisterUncreatableType<Tab>("core", 1, 0, "Tab", "Tab (from module core) may not be created directly.");
    qmlRegisterType<TabsModel>("core", 1, 0, "TabsModel");

    qmlRegisterType<DownloadsModel>("core", 1, 0, "DownloadsModel");

    // Register context properties
    engine.rootContext()->setContextProperty("Settings", &settings);
    engine.rootContext()->setContextProperty("DarkThemeTimer", &darkThemeTimer);
    #ifdef Q_OS_MACOS
        engine.rootContext()->setContextProperty("MacEvents", &evListener);
    #endif

    // setup qml imports
    engine.addImportPath("qrc:/");

    // load main ui
    engine.load(QUrl(QLatin1String("qrc:/ui/Main.qml")));

    // load main
    QMetaObject::invokeMethod(engine.rootObjects()[0], "load");

    return app.exec();
}
