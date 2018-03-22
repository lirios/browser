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

#ifndef STARTCONFIG_H
#define STARTCONFIG_H

#include <QObject>
#include <QUrl>

class StartConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl primaryStartUrl READ primaryStartUrl WRITE setPrimaryStartUrl NOTIFY primaryStartUrlChanged)
    Q_PROPERTY(QUrl defaultPrimaryStartUrl MEMBER m_defaultPrimaryStartUrl NOTIFY defaultPrimaryStartUrlChanged)
    Q_PROPERTY(QUrl darkStartUrl READ darkStartUrl WRITE setDarkStartUrl NOTIFY darkStartUrlChanged)
    Q_PROPERTY(QUrl defaultDarkStartUrl MEMBER m_defaultDarkStartUrl NOTIFY defaultDarkStartUrlChanged)
    Q_PROPERTY(QUrl incognitoStartUrl READ incognitoStartUrl WRITE setIncognitoStartUrl NOTIFY incognitoStartUrlChanged)
    Q_PROPERTY(QUrl defaultIncognitoStartUrl MEMBER m_defaultIncognitoStartUrl NOTIFY defaultIncognitoStartUrlChanged)
    Q_PROPERTY(StartupType startupType MEMBER m_startupType NOTIFY startupTypeChanged)
    Q_PROPERTY(bool persistentCookies MEMBER m_persistentCookies WRITE setPersistentCookies NOTIFY persistentCookiesChanged)
    Q_ENUMS(StartupType)
public:
    explicit StartConfig(QObject *parent = nullptr);

    enum StartupType {
        StartFromNewPage,
        StartFromPreviouslyOpenedTabs,
    };

    QUrl primaryStartUrl() const { return m_primaryStartUrl; }
    void setPrimaryStartUrl(QUrl url) { primaryStartUrlChanged(m_primaryStartUrl = url); }

    QUrl darkStartUrl() const { return m_darkStartUrl; }
    void setDarkStartUrl(QUrl url) { darkStartUrlChanged(m_darkStartUrl = url); }

    QUrl incognitoStartUrl() const { return m_incognitoStartUrl; }
    void setIncognitoStartUrl(QUrl url) { incognitoStartUrlChanged(m_incognitoStartUrl = url); }

    QUrl defaultPrimaryStartUrl() const { return m_defaultPrimaryStartUrl; }
    QUrl defaultIncognitoStartUrl() const { return m_defaultIncognitoStartUrl; }
    QUrl defaultDarkStartUrl() const { return m_defaultDarkStartUrl; }

    StartupType startupType() const { return m_startupType; }
    void setStartupType(const StartupType &startupType) { startupTypeChanged(m_startupType = startupType); }

    bool persistentCookies() const { return m_persistentCookies; }
    void setPersistentCookies(bool value) { persistentCookiesChanged(m_persistentCookies = value); }

signals:
    void primaryStartUrlChanged(QUrl url);
    void defaultPrimaryStartUrlChanged(QUrl url);
    void darkStartUrlChanged(QUrl url);
    void defaultDarkStartUrlChanged(QUrl url);
    void incognitoStartUrlChanged(QUrl url);
    void defaultIncognitoStartUrlChanged(QUrl url);
    void startupTypeChanged(StartupType url);
    void persistentCookiesChanged(bool persistentCookies);

private:
    QUrl m_primaryStartUrl;
    QUrl m_defaultPrimaryStartUrl;
    QUrl m_incognitoStartUrl;
    QUrl m_defaultIncognitoStartUrl;
    QUrl m_darkStartUrl;
    QUrl m_defaultDarkStartUrl;
    StartupType m_startupType;
    bool m_persistentCookies;

};

#endif // STARTCONFIG_H
