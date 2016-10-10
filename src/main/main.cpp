/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb (https://github.com/tim-sueberkrueb)
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
 */

#include <QtGlobal>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "../core/tabsmodel.h"
#include "../core/tab.h"

// Include QtWebEngine if enabled (otherwise Oxide is expected)
#if IS_QTWEBENGINE_ENABLED == 1
    #include <QtWebEngine>
#endif

int main(int argc, char *argv[])
{
    // Set Material Design QtQuick Controls 2 style
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");

    #if defined(ENABLE_HIGH_DPI_SCALING)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QGuiApplication app(argc, argv);

    #if IS_QTWEBENGINE_ENABLED == 1
        QtWebEngine::initialize();
    #endif

    // create qml app engine
    QQmlApplicationEngine engine;

    // register types
    qmlRegisterUncreatableType<Tab>("core", 1, 0, "Tab", "Tab (from module Liri.Browser.Core) may not be created directly.");
    qmlRegisterType<TabsModel>("core", 1, 0, "TabsModel");

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
