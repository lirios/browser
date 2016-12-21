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
#include <QQmlApplicationEngine>
#include <QDebug>

#include "../core/models/tabsmodel.h"
#include "../core/models/tab.h"
#include "../core/models/downloadsmodel.h"
#include "../core/models/webdownload.h"
#include "../core/settings/settings.h"

// Include QtWebEngine if enabled (otherwise Oxide is expected)
#if IS_QTWEBENGINE_ENABLED == 1
    #include <QtWebEngine>
#endif

int main(int argc, char *argv[])
{
    // Set Material Design QtQuick Controls 2 style
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");

    // Set the port for DevTools
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "0.0.0.0:9992");

    #if defined(ENABLE_HIGH_DPI_SCALING)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QGuiApplication app(argc, argv);

    #if IS_QTWEBENGINE_ENABLED == 1
        QtWebEngine::initialize();
    #endif

    // Create settings instance and load
    Settings settings;
    settings.load();

    // create qml app engine
    QQmlApplicationEngine engine;

    // register core types
    qmlRegisterUncreatableType<SearchConfig>("core", 1, 0, "SearchConfig", "SearchConfig (from module core) may not be created directly.");

    qmlRegisterUncreatableType<Tab>("core", 1, 0, "Tab", "Tab (from module core) may not be created directly.");
    qmlRegisterType<TabsModel>("core", 1, 0, "TabsModel");

    qmlRegisterUncreatableType<WebDownload>("core", 1, 0, "WebDownload", "WebDownload (from module core) may not be created directly.");
    qmlRegisterType<DownloadsModel>("core", 1, 0, "DownloadsModel");

    // Register context properties
    engine.rootContext()->setContextProperty("Settings", &settings);

    // setup qml imports
    engine.addImportPath("qrc:/");

    // load main ui
    engine.load(QUrl(QLatin1String("qrc:/ui/Main.qml")));

    // set webengine property
    #if IS_QTWEBENGINE_ENABLED == 1
        qDebug() << "Using QtWebEngine";
        engine.rootObjects()[0]->setProperty("webengine", 1);
    #else
        qDebug() << "Using Oxide engine";
        engine.rootObjects()[0]->setProperty("webengine", 0);
    #endif

    // load main
    QMetaObject::invokeMethod(engine.rootObjects()[0], "load");

    return app.exec();
}
