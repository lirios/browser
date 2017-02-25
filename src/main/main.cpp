/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <tim.sueberkrueb@web.de>
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
#include <QSysInfo>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlContext>
#include <QDebug>
#include <QIcon>

#include "../core/global/version.h"
#include "../core/models/tabsmodel.h"
#include "../core/models/tab.h"
#include "../core/models/downloadsmodel.h"
#include "../core/models/webdownload.h"
#include "../core/settings/settings.h"
#include "../core/extensions/extensionsmanager.h"
#include "../core/extensions/extensionsmodel.h"
#include "../core/extensions/extensiontheme.h"
#include "../core/extensions/extensionsearchengine.h"
#include "../core/extensions/extensionthemesmodel.h"
#include "../core/extensions/extension.h"
#include "../core/utils/secondarythemetimer.h"
#include "../core/utils/themeprovider.h"
#include "../core/utils/searchprovider.h"

#ifdef Q_OS_MACOS
    #include "mac/MacOsEventListener.h"
#endif

// Include QtWebEngine if enabled (otherwise Oxide is expected)
#if IS_QTWEBENGINE_ENABLED == 1
    #include <QtWebEngine>
#endif

int main(int argc, char *argv[])
{
    qDebug() << "Liri Browser" << Version::fullString;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    // Set application info
    QCoreApplication::setOrganizationName("liri-project");
    QCoreApplication::setOrganizationDomain("liri.io");
    QCoreApplication::setApplicationName("io.liri.browser");
    QCoreApplication::setApplicationVersion(Version::fullString);

    #ifdef Q_OS_MACOS
        MacOsEventListener evListener;
        initMacOsEventListener(&evListener);
    #endif
    app.setWindowIcon(QIcon(":/res/icons/iconx512.png"));


    QQuickStyle::setStyle(QLatin1String("Material"));

    #if IS_QTWEBENGINE_ENABLED == 1
        QtWebEngine::initialize();
    #endif

    // Create settings instance and load
    Settings settings;
    settings.load();

    // Create extensions manager and load
    ExtensionsManager extensionsManager;
    extensionsManager.loadBuiltins();
    extensionsManager.startWatching();
    extensionsManager.scan();

    // Create and start secondary theme time
    SecondaryThemeTimer secondaryThemeTimer;
    secondaryThemeTimer.start();

    // Create and configure theme provider
    ThemeProvider themeProvider;
    themeProvider.setDefaultName("default.light");
    themeProvider.setModel(extensionsManager.themesModel());

    ThemeProvider incognitoThemeProvider;
    incognitoThemeProvider.setDefaultName("default.bluegrey");
    incognitoThemeProvider.setModel(extensionsManager.themesModel());

    // Create and configure search provider
    SearchProvider searchProvider;
    searchProvider.setModel(extensionsManager.searchEnginesModel());

    // Create qml app engine
    QQmlApplicationEngine engine;

    // Register core types
    qmlRegisterUncreatableType<SearchConfig>("core", 1, 0, "SearchConfig", "SearchConfig (from module core) may not be created directly.");

    qmlRegisterUncreatableType<Tab>("core", 1, 0, "Tab", "Tab (from module core) may not be created directly.");
    qmlRegisterType<TabsModel>("core", 1, 0, "TabsModel");

    qmlRegisterUncreatableType<WebDownload>("core", 1, 0, "WebDownload", "WebDownload (from module core) may not be created directly.");
    qmlRegisterType<DownloadsModel>("core", 1, 0, "DownloadsModel");

    qmlRegisterUncreatableType<Extension>("core", 1, 0, "Extension", "Extension (from module core) may not be created directly.");
    qmlRegisterUncreatableType<ExtensionsModel>("core", 1, 0, "ExtensionsModel", "ExtensionsModel (from module core) may not be created directly.");
    qmlRegisterUncreatableType<ExtensionTheme>("core", 1, 0, "ExtensionTheme", "ExtensionTheme (from module core) may not be created directly.");
    qmlRegisterUncreatableType<ExtensionSearchEngine>("core", 1, 0, "ExtensionSearchEngine", "ExtensionSearchEngine (from module core) may not be created directly.");
    qmlRegisterUncreatableType<ExtensionThemesModel>("core", 1, 0, "ExtensionThemesModel", "ExtensionThemesModel (from module core) may not be created directly.");

    // Register context properties
    engine.rootContext()->setContextProperty("ApplicationVersion", Version::fullString);
    engine.rootContext()->setContextProperty("QtVersion", QT_VERSION_STR);
    engine.rootContext()->setContextProperty("SystemInformation", QSysInfo::prettyProductName());
    engine.rootContext()->setContextProperty("Settings", &settings);
    engine.rootContext()->setContextProperty("SecondaryThemeTimer", &secondaryThemeTimer);
    engine.rootContext()->setContextProperty("Extensions", &extensionsManager);
    engine.rootContext()->setContextProperty("Theme", &themeProvider);
    engine.rootContext()->setContextProperty("IncognitoTheme", &incognitoThemeProvider);
    engine.rootContext()->setContextProperty("Search", &searchProvider);

    #ifdef Q_OS_MACOS
        engine.rootContext()->setContextProperty("MacEvents", &evListener);
    #endif

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
