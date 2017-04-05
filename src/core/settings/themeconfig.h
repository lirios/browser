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

#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QObject>
#include <QTime>

class ThemeConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString primary READ primary WRITE setPrimary NOTIFY primaryChanged)
    Q_PROPERTY(QString secondary READ secondary WRITE setSecondary NOTIFY secondaryChanged)
    Q_PROPERTY(QString incognito READ incognito WRITE setIncognito NOTIFY incognitoChanged)
    Q_PROPERTY(bool secondaryEnabled READ secondaryEnabled WRITE setSecondaryEnabled NOTIFY secondaryEnabledChanged)
    Q_PROPERTY(QTime secondaryStartTime READ secondaryStartTime WRITE setSecondaryStartTime NOTIFY secondaryStartTimeChanged)
    Q_PROPERTY(QTime secondaryEndTime READ secondaryEndTime WRITE setSecondaryEndTime NOTIFY secondaryEndTimeChanged)
public:
    explicit ThemeConfig(QObject *parent = nullptr);

    QString primary() const { return m_primary; }
    void setPrimary(QString primary) { primaryChanged(m_primary = primary); }

    QString secondary() const { return m_secondary; }
    void setSecondary(QString secondary) { secondaryChanged(m_secondary = secondary); }

    QString incognito() const { return m_incognito; }
    void setIncognito(QString incognito) { incognitoChanged(m_incognito = incognito); }

    bool secondaryEnabled() const { return m_secondaryEnabled; }
    void setSecondaryEnabled(bool secondaryEnabled) { secondaryEnabledChanged(m_secondaryEnabled = secondaryEnabled); }

    QTime secondaryStartTime() const { return m_secondaryStartTime; }
    void setSecondaryStartTime(QTime secondaryStartTime) { secondaryStartTimeChanged(m_secondaryStartTime = secondaryStartTime); }
    Q_INVOKABLE void setSecondaryStartTime(QString secondaryStartTime, QString format) {
        secondaryStartTimeChanged(m_secondaryStartTime = QTime::fromString(secondaryStartTime, format));
    }

    QTime secondaryEndTime() const { return m_secondaryEndTime; }
    void setSecondaryEndTime(QTime secondaryEndTime) { secondaryEndTimeChanged(m_secondaryEndTime = secondaryEndTime); }
    Q_INVOKABLE void setSecondaryEndTime(QString secondaryEndTime, QString format) {
        secondaryEndTimeChanged(m_secondaryEndTime = QTime::fromString(secondaryEndTime, format));
    }

signals:
    void primaryChanged(QString primary);
    void secondaryChanged(QString secondary);
    void incognitoChanged(QString incognito);
    void secondaryEnabledChanged(bool secondaryEnabled);
    void secondaryStartTimeChanged(QTime secondaryStartTime);
    void secondaryEndTimeChanged(QTime secondaryEndTime);

private:
    QString m_primary;
    QString m_secondary;
    QString m_incognito;
    bool m_secondaryEnabled;
    QTime m_secondaryStartTime;
    QTime m_secondaryEndTime;
};

#endif // THEMECONFIG_H
