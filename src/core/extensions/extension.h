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

#ifndef EXTENSION_H
#define EXTENSION_H

#include <QObject>
#include <QColor>

#include "extensiontheme.h"

class Extension : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)

    friend class ExtensionParser;

public:
    explicit Extension(QObject *parent = nullptr);

    bool load(QString filePath);

    bool valid() const { return m_valid; }
    void setValid(bool valid) { validChanged(m_valid = valid); }

    QString name() const { return m_name; }
    void setName(QString name) { nameChanged(m_name = name); }

    QString version() const { return m_version; }
    void setVersion(QString version) { versionChanged(m_version = version); }

    QString title() const { return m_title; }
    void setTitle(QString title) { titleChanged(m_title = title); }

    QString author() const { return m_author; }
    void setAuthor(QString author) { authorChanged(m_author = author); }

    QString summary() const { return m_summary; }
    void setSummary(QString summary) { summaryChanged(m_summary = summary); }

    QString description() const { return m_description; }
    void setDescription(QString description) { descriptionChanged(m_description = description); }

    QString email() const { return m_email; }
    void setEmail(QString email) { emailChanged(m_email = email); }

    QList<ExtensionTheme*> themes() const { return m_extensionThemes; }

signals:
    void validChanged(bool valid);
    void nameChanged(QString name);
    void versionChanged(QString version);
    void titleChanged(QString title);
    void authorChanged(QString author);
    void descriptionChanged(QString description);
    void summaryChanged(QString summary);
    void emailChanged(QString email);

private:  
    bool m_valid;

    QString m_name;
    QString m_version;
    QString m_title;
    QString m_author;
    QString m_description;
    QString m_summary;
    QString m_email;

    QList<ExtensionTheme*> m_extensionThemes;
};

#endif // EXTENSION_H
