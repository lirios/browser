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
#include "browserapplication.h"

#ifdef Q_OS_MACOS
    #include "mac/MacOsEventListener.h"
#endif

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

    // Set the X11 WM_CLASS so X11 desktops can find the desktop file
    qputenv("RESOURCE_NAME", "io.liri.Browser");

    #ifdef Q_OS_MACOS
        MacOsEventListener evListener;
        initMacOsEventListener(&evListener);
    #endif

    QtWebEngine::initialize();

    BrowserApplication browser;

    // Launch the browser or forward arguments to an already running instance (if any)
    if (browser.loadOrForward()) {
        return app.exec();
    }

    return 0;
}
