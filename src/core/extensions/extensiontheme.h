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

#ifndef EXTENSIONTHEME_H
#define EXTENSIONTHEME_H

#include <QObject>
#include <QColor>

class ExtensionTheme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
    Q_PROPERTY(QString extensionName READ extensionName WRITE setExtensionName NOTIFY extensionNameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool dark READ dark WRITE setDark NOTIFY darkChanged)
    Q_PROPERTY(bool adaptWebsiteTheme READ adaptWebsiteTheme WRITE setAdaptWebsiteTheme NOTIFY adaptWebsiteThemeChanged)
    Q_PROPERTY(QColor accent READ accent WRITE setAccent NOTIFY accentChanged)
    Q_PROPERTY(QColor primary READ primary WRITE setPrimary NOTIFY primaryChanged)
    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
public:
    explicit ExtensionTheme(QObject *parent = nullptr);

    ExtensionTheme* clone(QObject* parent = nullptr);

    bool valid() const { return m_valid; }
    void setValid(bool valid) { validChanged(m_valid=valid); }

    QString extensionName() const { return m_extensionName; }
    void setExtensionName(QString extensionName) { extensionNameChanged(m_extensionName = extensionName); }

    QString name() const { return m_name; }
    void setName(QString name) { nameChanged(m_name = name); }

    QString title() const { return m_title; }
    void setTitle(QString title) { titleChanged(m_title = title); }

    QString summary() const { return m_summary; }
    void setSummary(QString summary) { summaryChanged(m_summary = summary); }

    QString description() const { return m_description; }
    void setDescription(QString description) { descriptionChanged(m_description = description); }

    bool dark() const { return m_dark; }
    void setDark(bool dark) { darkChanged(m_dark = dark); }

    bool adaptWebsiteTheme() const { return m_adaptWebsiteTheme; }
    void setAdaptWebsiteTheme(bool adaptWebsiteTheme) { adaptWebsiteThemeChanged(m_adaptWebsiteTheme = adaptWebsiteTheme); }

    QColor accent() const { return m_accent; }
    void setAccent(QColor accent) { accentChanged(m_accent = accent); }

    QColor primary() const { return m_primary; }
    void setPrimary(QColor primary) { primaryChanged(m_primary = primary); }

    QColor foreground() const { return m_foreground; }
    void setForeground(QColor foreground) { foregroundChanged(m_foreground = foreground); }

    QColor background() const { return m_background; }
    void setBackground(QColor background) { backgroundChanged(m_background = background); }

signals:
    void validChanged(bool valid);

    void extensionNameChanged(QString extensionName);
    void nameChanged(QString name);
    void titleChanged(QString title);
    void summaryChanged(QString summary);
    void descriptionChanged(QString description);

    void darkChanged(bool dark);
    void adaptWebsiteThemeChanged(bool adaptWebsiteTheme);
    void accentChanged(QColor accent);
    void primaryChanged(QColor primary);
    void foregroundChanged(QColor foreground);
    void backgroundChanged(QColor background);

private:
    bool m_valid;

    QString m_extensionName;
    QString m_name;
    QString m_title;
    QString m_summary;
    QString m_description;

    bool m_dark;
    bool m_adaptWebsiteTheme;
    QColor m_accent;
    QColor m_primary;
    QColor m_foreground;
    QColor m_background;
};

#endif // EXTENSIONTHEME_H
