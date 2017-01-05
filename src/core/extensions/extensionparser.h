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

#ifndef EXTENSIONPARSER_H
#define EXTENSIONPARSER_H

#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QColor>
#include <QException>
#include <QUrl>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#include "extension.h"
#include "extensiontheme.h"

using Type = QJsonValue::Type;

class FieldError : public QException
{
public:
    void raise() const { throw *this; }
    FieldError *clone() const { return new FieldError(*this); }
};

class ResourceError : public QException
{
public:
    void raise() const { throw *this; }
    ResourceError *clone() const { return new ResourceError(*this); }
};

class ParseError : public QException
{
public:
    void raise() const { throw *this; }
    ParseError *clone() const { return new ParseError(*this); }
};

enum ExtensionType {
    LBX,
    QRC
};

class ExtensionParser : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionParser(QObject *parent=nullptr);

    enum FieldStatus {
        Valid,
        NonExistent,
        WrongType,
        Empty
    };

    bool open(const QUrl url, ExtensionType type=ExtensionType::LBX);
    void close();

    bool load();

    QByteArray readResource(const QString resourceName);
    QString fromResourceName(QString resourceName);

    bool parseMeta(const QByteArray jsonData);
    bool loadTheme(const QString resourceName);
    bool parseTheme(const QByteArray jsonData);

    Extension* extension() const { return m_extension; }
    void setExtension(Extension* extension) { extensionChanged(m_extension = extension); }

signals:
    void extensionChanged(Extension* extension);

private:
    QuaZip* m_zip;
    QuaZipFile* m_zipFile;

    Extension* m_extension;

    ExtensionType m_type;
    QUrl m_url;
    QString m_filePath;
    QString m_pureFileName;

    FieldStatus fieldStatus(QJsonObject object, QString fieldName, Type fieldType);
    void assertField(QJsonObject object, QString fieldName, Type fieldType);

    QString typeName(Type type);
};

#endif // EXTENSIONPARSER_H
