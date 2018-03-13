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
#include <QList>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlContext>
#include <QtWebEngine>
#include <QCommandLineParser>
#ifndef QT_NO_TRANSLATION
    #include <QtCore/QTranslator>
#endif
#include <QDebug>


#include <qtsingleapplication.h>

#include "browserapplication.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle(QLatin1String("Material"));

    QtSingleApplication app(argc, argv);

    // Set app info
    app.setOrganizationName(QStringLiteral("Liri"));
    app.setOrganizationDomain(QStringLiteral("liri.io"));
    app.setApplicationName(QStringLiteral("Browser"));
    app.setApplicationVersion(QStringLiteral(PROJECT_VERSION));
    app.setDesktopFileName(QStringLiteral("io.liri.Browser.desktop"));
    app.setWindowIcon(QIcon(":/res/icons/512x512/io.liri.Browser.png"));

    // Set the X11 WM_CLASS so X11 desktops can find the desktop file
    qputenv("RESOURCE_NAME", "io.liri.Browser");

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Liri Browser\n\nA cross-platform Material Design web browser"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption incognitoOption(QStringLiteral("incognito"), QStringLiteral("Open incognito mode"));
    parser.addOption(incognitoOption);

    parser.addPositionalArgument("urls", "Space-separated list of urls to open");

    parser.process(app);

    bool incognito = parser.isSet(incognitoOption);
    auto args = parser.positionalArguments();

    // Send messages to already running instance (if any)
    if (app.isRunning()) {
        qDebug() << "Already running";
        if (args.count() > 0) {
            for (const QString &arg: args) {
                QString message = incognito ? QStringLiteral("open_incognito ")
                                            : QStringLiteral("open ");
                message.append(QUrl(arg).toString(QUrl::FullyEncoded));
                app.sendMessage(message);
            }
        } else {
            QString message = incognito ? QStringLiteral("open_incognito")
                                        : QStringLiteral("open");
            app.sendMessage(message);
        }
        return 0;
    }

    QtWebEngine::initialize();

    BrowserApplication browser;

    QObject::connect(&app, &QtSingleApplication::messageReceived,
                     &browser, &BrowserApplication::onMessageReceived);

    browser.load();

    if (args.count() > 0) {
        // Open urls from commandline arguments
        for (const QString &arg: args) {
            browser.openUrl(QUrl(arg), incognito);
        }
    } else {
        browser.openStartUrl(incognito);
    }

    return app.exec();
}
