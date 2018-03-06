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

#include <QQmlContext>
#include <QScopedPointer>
#include <QByteArray>
#include <QDataStream>
#include <QFileInfo>
#include <QDebug>

#include "browserapplication.h"

const int IPC_ARG_COUNT_LIMIT = 100;
const QString IPC_HEADER = QLatin1String("io.liri.Browser");
const int IPC_QDATASTREAM_VERSION = QDataStream::Qt_5_10;

BrowserApplication::BrowserApplication(QObject *parent) : QObject(parent)
{
    connect(&m_localServer, &QLocalServer::newConnection,
            this, &BrowserApplication::onSocketConnection);
}

bool BrowserApplication::checkForRunningInstance()
{
    m_localSocket.connectToServer(LOCAL_SOCKET_PATH);
    return m_localSocket.waitForConnected(1000);
}

bool BrowserApplication::loadOrForward()
{
    // FIXME: This probably does not work on Windows, yet
    bool socketExists = QFileInfo(LOCAL_SOCKET_PATH).exists();
    bool alreadyRunning = checkForRunningInstance();

    // Handle invalid existing local socket
    if (socketExists && !alreadyRunning) {
        qWarning() << "Local socket" << LOCAL_SOCKET_PATH << "exists but we failed to connect";
        qWarning() << "Assuming the application crashed";
        qWarning() << "Attempting to remove local socket";
        QFile file(LOCAL_SOCKET_PATH);
        if (file.remove()) {
            qWarning() << "Assumingly invalid socket removed successfully";
        } else {
            qCritical() << "Could not remove local socket";
            qCritical() << "Application will run without ipc";
            m_ipcEnabled = false;
        }
    }

    if (alreadyRunning) {
        qDebug() << m_localSocket.isValid();
        qDebug() << "Already running.";
        QStringList args = qGuiApp->arguments();
        args.removeFirst();
        // Forward command line arguments to already running instance
        if (args.count() > 0) {
            QByteArray data;
            QDataStream request(&data, QIODevice::WriteOnly);
            request.setVersion(IPC_QDATASTREAM_VERSION);
            request << IPC_HEADER;
            for (int i = 0; i<qMin(args.count(), IPC_ARG_COUNT_LIMIT); i++) {
                request << args[i];
            }
            if (args.count() > IPC_ARG_COUNT_LIMIT) {
                qWarning() << "Maximum number of command line arguments exceeded.";
            }
            m_localSocket.write(data);
            m_localSocket.flush();
            m_localSocket.waitForBytesWritten();
            m_localSocket.close();
        }
        return false;
    } else {
        if (m_ipcEnabled) {
            m_localServer.listen(LOCAL_SOCKET_PATH);
        }
        loadUI();
        // Handle command line arguments
        QStringList args = qGuiApp->arguments();
        args.removeFirst();
        for (int i = 0; i<qMin(args.count(), IPC_ARG_COUNT_LIMIT); i++) {
            QMetaObject::invokeMethod(m_engine.rootObjects()[0], "openUrl",
                    Q_ARG(QVariant, QVariant(args[i])), Q_ARG(QVariant, QVariant(false)));
        }
        return true;
    }
}

void BrowserApplication::loadUI()
{
    // Load settings
    m_settings.load();

    // Start dark theme time
    m_darkThemeTimer.start();

    // Load translations
    BrowserApplication::loadTranslations();

    // register core types
    qmlRegisterUncreatableType<SearchConfig>("core", 1, 0, "SearchConfig", "SearchConfig (from module core) may not be created directly.");

    qmlRegisterUncreatableType<Tab>("core", 1, 0, "Tab", "Tab (from module core) may not be created directly.");
    qmlRegisterType<TabsModel>("core", 1, 0, "TabsModel");

    qmlRegisterType<DownloadsModel>("core", 1, 0, "DownloadsModel");

    // Register context properties
    m_engine.rootContext()->setContextProperty("Settings", &m_settings);
    m_engine.rootContext()->setContextProperty("DarkThemeTimer", &m_darkThemeTimer);
    #ifdef Q_OS_MACOS
        m_engine.rootContext()->setContextProperty("MacEvents", &evListener);
    #endif

    // setup qml imports
    m_engine.addImportPath("qrc:/");

    // load main ui
    m_engine.load(QUrl(QLatin1String("qrc:/ui/Main.qml")));

    // load main
    QMetaObject::invokeMethod(m_engine.rootObjects()[0], "load");
}

void BrowserApplication::onSocketConnection()
{
    auto socket = m_localServer.nextPendingConnection();
    if (socket->waitForReadyRead()) {
        QByteArray data = socket->readAll();
        QDataStream request(&data, QIODevice::ReadOnly);
        request.setVersion(IPC_QDATASTREAM_VERSION);
        QString header_message;
        request >> header_message;
        if (header_message != IPC_HEADER) {
            socket->close();
            socket->deleteLater();
            return;
        }
        unsigned int limit = IPC_ARG_COUNT_LIMIT;
        while(!request.atEnd() && limit > 0) {
            QString msg;
            request >> msg;
            QMetaObject::invokeMethod(m_engine.rootObjects()[0], "openUrl",
                    Q_ARG(QVariant, QVariant(msg)), Q_ARG(QVariant, QVariant(false)));
            limit--;
        }
        if (limit == 0) {
            qCritical() << "Socket connection reached local socket ipc message limit";
        }
    }
    socket->close();
    socket->deleteLater();
}
