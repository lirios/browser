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

#include "extensionparser.h"

#include <QDebug>

ExtensionParser::ExtensionParser(QObject *parent)
    : QObject(parent)
{

}

bool ExtensionParser::open(const QUrl url, ExtensionType type)
{
    m_type = type;
    m_url = url;
    m_filePath = url.toString();
    if (m_filePath.startsWith("qrc://"))
        m_filePath = m_filePath.mid(6);
    m_pureFileName = m_filePath.section("/", -1).section(".", 0, 0);

    if (type == ExtensionType::LBX) {
        // Create QuaZip and QuaZipFile
        m_zip = new QuaZip(m_filePath);
        m_zipFile = new QuaZipFile(m_zip);

        // Attempt to open QuaZip
        if (!m_zip->open(QuaZip::mdUnzip)) {
            qWarning() << "Error: Could not open file" << m_filePath << "for read.";
            return false;
        }
    }
    return true;
}

void ExtensionParser::close()
{
    if (m_type == ExtensionType::LBX) {
        m_zip->close();
        m_zipFile->close();
        delete m_zip;
        delete m_zipFile;
    }
}

QByteArray ExtensionParser::readResource(const QString resource)
{
    QString resourceFileName = fromResourceName(resource);
    QByteArray data;

    if (m_type == ExtensionType::LBX) {
        // Get file list
        QStringList fileList = m_zip->getFileNameList();

        if (!fileList.contains(resourceFileName) && fileList.contains(m_pureFileName + "/" + resourceFileName)) {
            resourceFileName = m_pureFileName + "/" + resourceFileName;
        } else {
            qWarning() << "Error: Could not find resource" << resource << "in" << m_filePath;
            throw ResourceError();
        }

        m_zip->setCurrentFile(resourceFileName);
        if (!m_zipFile->open(QIODevice::ReadOnly)) {
            qCritical() << "Could not open resource" << resource << "in" << m_filePath << "for read.";
            throw ResourceError();
        }

        data = m_zipFile->readAll();
        m_zipFile->close();
    } else if (m_type == ExtensionType::QRC) {
        QFile file(":/" + m_filePath + "/" + resourceFileName);
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << "Could not open resource" << resource << "in" << m_filePath << "for read.";
            throw ResourceError();
        }
        data = file.readAll();
        file.close();
    }
    return data;
}

QString ExtensionParser::fromResourceName(QString resourceName)
{
    if (resourceName.indexOf("res://") == 0)
        resourceName = resourceName.mid(6);
    return resourceName;
}

bool ExtensionParser::load()
{
    // Read meta data
    try {
        QByteArray metaData = readResource("res://meta/package.json");
        if(!parseMeta(metaData)) {
            qCritical() << "Failed to parse meta data.";
            qCritical() << "Stopped loading extension.";
            return false;
        }
    }
    catch (ResourceError &e) {
        qCritical() << "Stopped loading extension.";
        return false;
    }

    qDebug() << "Extension loaded.";
    return true;
}

bool ExtensionParser::parseMeta(const QByteArray jsonData)
{
    try {
        QJsonDocument doc(QJsonDocument::fromJson(jsonData));
        if (doc.isEmpty()) {
            qWarning() << "Error: Invalid JSON document.";
            return false;
        }
        QJsonObject root = doc.object();

        // Parse schema
        assertField(root, "schema", Type::Object);
        QJsonObject schema = root["schema"].toObject();

        assertField(schema, "type", Type::String);
        QString schemaType = schema["type"].toString();
        if (schemaType != "lbx/meta") {
            qWarning() << "Error: Invalid schema type" << schemaType << "detected. Expected \"lbx/meta\".";
            throw ParseError();
        }

        assertField(schema, "version", Type::String);
        QString schemaVersion = schema["version"].toString();
        if (schemaVersion != "0.1.0") {
            qWarning() << "Error: Invalid schema version" << schemaVersion << "detected.";
            throw ParseError();
        }

        // Parse meta
        assertField(root, "meta", Type::Object);
        QJsonObject meta = root["meta"].toObject();

        assertField(meta, "name", Type::String);
        QString metaName = meta["name"].toString();
        if (metaName != m_pureFileName) {
            qWarning() << "Error: Meta name" << metaName << "doesn't match package name.";
            throw ParseError();
        }
        m_extension->setName(metaName);

        assertField(meta, "name", Type::String);
        m_extension->setTitle(meta["title"].toString());

        assertField(meta, "version", Type::String);
        m_extension->setVersion(meta["version"].toString());

        assertField(meta, "author", Type::String);
        m_extension->setAuthor(meta["author"].toString());

        assertField(meta, "description", Type::String);
        m_extension->setDescription(meta["description"].toString());

        assertField(meta, "summary", Type::String);
        m_extension->setSummary(meta["summary"].toString());

        assertField(meta, "email", Type::String);
        m_extension->setEmail(meta["email"].toString());

        // Parse content
        assertField(root, "content", Type::Object);
        QJsonObject content = root["content"].toObject();

        // Parse static content
        if (content.contains("static")) {
            assertField(content, "static", Type::Object);
            QJsonObject contentStatic = content["static"].toObject();
            qDebug() << "Extension contains static content.";

            // Parse themes
            if (contentStatic.contains("themes")) {
                assertField(contentStatic, "themes", Type::Array);
                QJsonArray themes = contentStatic["themes"].toArray();
                qDebug() << "Extension contains themes.";
                for (QJsonValue value: themes) {
                    if (!value.isString()) {
                        qDebug() << "Error: Field \"themes\" must be an array of resource names!";
                        throw ParseError();
                    }
                    if (!loadTheme(value.toString())) {
                        qCritical() << "Failed to parse theme data.";
                        qCritical() << "Stopped loading extension.";
                        throw ParseError();
                    }
                }
            }

            // Parse search engines
            if (contentStatic.contains("search")) {
                assertField(contentStatic, "search", Type::Array);
                QJsonArray searchEngines = contentStatic["search"].toArray();
                qDebug() << "Extension contains search engines.";
                for (QJsonValue value: searchEngines) {
                    if (!value.isString()) {
                        qDebug() << "Error: Field \"search\" must be an array of resource names!";
                        throw ParseError();
                    }
                    if (!loadSearchEngine(value.toString())) {
                        qCritical() << "Failed to parse search engine data.";
                        qCritical() << "Stopped loading extension.";
                        throw ParseError();
                    }
                }
            }
        }
    } catch (ParseError &e) {
        return false;
    }
    return true;
}

bool ExtensionParser::loadTheme(const QString resourceName)
{
    try {
        QByteArray themeData = readResource(resourceName);
        if(!parseTheme(themeData)) {
            return false;
        }
    }
    catch (ResourceError &e) {
        return false;
    }

    qDebug() << "Theme" << resourceName << "loaded.";
    return true;
}

bool ExtensionParser::parseTheme(const QByteArray jsonData)
{
    try {
        QJsonDocument doc(QJsonDocument::fromJson(jsonData));
        if (doc.isEmpty()) {
            qWarning() << "Error: Invalid JSON document.";
            throw ParseError();
        }
        QJsonObject themeObject = doc.object();

        // Parse schema
        assertField(themeObject, "schema", Type::Object);
        QJsonObject schema = themeObject["schema"].toObject();

        assertField(schema, "type", Type::String);
        QString schemaType = schema["type"].toString();
        if (schemaType != "lbx/theme") {
            qWarning() << "Error: Invalid schema type" << schemaType << "detected. Expected \"lbx/theme\".";
            throw ParseError();
        }

        assertField(schema, "version", Type::String);
        QString schemaVersion = schema["version"].toString();
        if (schemaVersion != "0.1.0") {
            qWarning() << "Error: Invalid schema version" << schemaVersion << "detected.";
            throw ParseError();
        }

        // Parse meta
        assertField(themeObject, "meta", Type::Object);
        QJsonObject meta = themeObject["meta"].toObject();

        assertField(meta, "name", Type::String);
        QString themeName = meta["name"].toString();
        if (!themeName.startsWith(m_extension->name() + ".")) {
            qWarning() << "Error: Theme name" << themeName << "doesn't match package name.";
            throw ParseError();
        }

        assertField(meta, "title", Type::String);
        QString themeTitle = meta["title"].toString();

        assertField(meta, "summary", Type::String);
        QString themeSummary = meta["summary"].toString();

        assertField(meta, "description", Type::String);
        QString themeDescription = meta["description"].toString();

        // Parse palette
        assertField(themeObject, "palette", Type::Object);
        QJsonObject palette = themeObject["palette"].toObject();

        assertField(palette, "dark", Type::Bool);
        bool paletteDark = palette["dark"].toBool();

        assertField(palette, "adapt_website_theme", Type::Bool);
        bool paletteAdaptWebsiteTheme = palette["adapt_website_theme"].toBool();

        assertField(palette, "accent", Type::String);
        QColor paletteAccent(palette["accent"].toString());

        assertField(palette, "primary", Type::String);
        QColor palettePrimary(palette["primary"].toString());

        assertField(palette, "foreground", Type::String);
        QColor paletteForeground(palette["foreground"].toString());

        assertField(palette, "background", Type::String);
        QColor paletteBackground(palette["background"].toString());

        // Create instance and store
        ExtensionTheme* theme = new ExtensionTheme(this->extension());
        theme->setExtensionName(m_extension->name());
        theme->setName(themeName);
        theme->setTitle(themeTitle);
        theme->setSummary(themeSummary);
        theme->setDescription(themeDescription);
        theme->setDark(paletteDark);
        theme->setAdaptWebsiteTheme(paletteAdaptWebsiteTheme);
        theme->setAccent(paletteAccent);
        theme->setPrimary(palettePrimary);
        theme->setForeground(paletteForeground);
        theme->setBackground(paletteBackground);
        m_extension->m_extensionThemes.append(theme);
    }
    catch(ParseError &e) {
        return false;
    }
    return true;
}

bool ExtensionParser::loadSearchEngine(const QString resourceName)
{
    try {
        QByteArray data = readResource(resourceName);
        if(!parseSearchEngine(data)) {
            return false;
        }
    }
    catch (ResourceError &e) {
        return false;
    }

    qDebug() << "Search engine" << resourceName << "loaded.";
    return true;
}

bool ExtensionParser::parseSearchEngine(const QByteArray jsonData)
{
    try {
        QJsonDocument doc(QJsonDocument::fromJson(jsonData));
        if (doc.isEmpty()) {
            qWarning() << "Error: Invalid JSON document.";
            throw ParseError();
        }
        QJsonObject root = doc.object();

        // Parse schema
        assertField(root, "schema", Type::Object);
        QJsonObject schema = root["schema"].toObject();

        assertField(schema, "type", Type::String);
        QString schemaType = schema["type"].toString();
        if (schemaType != "lbx/search") {
            qWarning() << "Error: Invalid schema type" << schemaType << "detected. Expected \"lbx/search\".";
            throw ParseError();
        }

        assertField(schema, "version", Type::String);
        QString schemaVersion = schema["version"].toString();
        if (schemaVersion != "0.1.0") {
            qWarning() << "Error: Invalid schema version" << schemaVersion << "detected.";
            throw ParseError();
        }

        // Parse meta
        assertField(root, "meta", Type::Object);
        QJsonObject meta = root["meta"].toObject();

        assertField(meta, "name", Type::String);
        QString searchName = meta["name"].toString();
        if (!searchName.startsWith(m_extension->name() + ".")) {
            qWarning() << "Error: search name" << searchName << "doesn't match package name.";
            throw ParseError();
        }

        assertField(meta, "title", Type::String);
        QString searchTitle = meta["title"].toString();

        assertField(meta, "summary", Type::String);
        QString searchSummary = meta["summary"].toString();

        assertField(meta, "description", Type::String);
        QString searchDescription = meta["description"].toString();

        // Parse url
        assertField(root, "url", Type::Object);
        QJsonObject urlObject = root["url"].toObject();

        assertField(urlObject, "base", Type::String);
        QString urlBase = urlObject["base"].toString();

        assertField(urlObject, "params", Type::Array);
        QJsonArray params = urlObject["params"].toArray();

        // Create instance
        ExtensionSearchEngine* searchEngine = new ExtensionSearchEngine(this->extension());
        searchEngine->setExtensionName(m_extension->name());
        searchEngine->setName(searchName);
        searchEngine->setTitle(searchTitle);
        searchEngine->setSummary(searchSummary);
        searchEngine->setDescription(searchDescription);
        searchEngine->setUrlBase(urlBase);

        for (QJsonValue value : params) {
            if (!value.isObject()) {
                qWarning() << "Error: Field \"params\" must be an array of objects";
                searchEngine->deleteLater();
                throw ParseError();
            }
            QJsonObject paramObject = value.toObject();

            assertField(paramObject, "name", Type::String);
            QString paramName = paramObject["name"].toString();

            assertField(paramObject, "value", Type::String);
            QString paramValue = paramObject["value"].toString();

            assertField(paramObject, "context", Type::Array);
            QJsonArray contextArray = paramObject["context"].toArray();

            ExtensionSearchEngineParameter::ContextFlag paramContext;

            for (QJsonValue contextValue: contextArray) {
                if (!contextValue.isString()) {
                    qWarning() << "Error: Field \"context\" must be an array of strings";
                    searchEngine->deleteLater();
                    throw ParseError();
                }
                QString contextString = contextValue.toString();

                if (contextString == "search")
                    paramContext |= ExtensionSearchEngineParameter::Search;
                else if (contextString == "homepage")
                    paramContext |= ExtensionSearchEngineParameter::Homepage;
                else {
                    qWarning() << "Error: Unknown context value" << contextString << "detected.";
                    searchEngine->deleteLater();
                    throw ParseError();
                }
            }
            ExtensionSearchEngineParameter* searchParameter = new ExtensionSearchEngineParameter(searchEngine);
            searchParameter->setName(paramName);
            searchParameter->setValue(paramValue);
            searchParameter->setContext(paramContext);
            searchEngine->parameters()->append(searchParameter);
        }
        m_extension->m_extensionSearchEngines.append(searchEngine);
    }
    catch(ParseError &e) {
        return false;
    }
    return true;
}

ExtensionParser::FieldStatus ExtensionParser::fieldStatus(QJsonObject object, QString fieldName, Type fieldType)
{
    if (!object.contains(fieldName))
        return FieldStatus::NonExistent;

    QJsonValue value = object[fieldName];

    if (value.type() != fieldType)
        return FieldStatus::WrongType;
    else if (value.type() == Type::String && value.toString() == "")
        return FieldStatus::Empty;

    return FieldStatus::Valid;
}

void ExtensionParser::assertField(QJsonObject object, QString fieldName, Type fieldType)
{
    FieldStatus status = fieldStatus(object, fieldName, fieldType);
    switch (status) {
        case FieldStatus::NonExistent:
            qWarning() << "Error: Required field" << fieldName << "does not exist.";
            throw ParseError();
        case FieldStatus::WrongType:
            qWarning() << "Error: Field" << fieldName << "must be of type" << typeName(fieldType) << "by requirement.";
            throw ParseError();
        case FieldStatus::Empty:
            qWarning() << "Error: Required field" << fieldName << "is empty";
            throw ParseError();
        case FieldStatus::Valid:
            break;
    }
}

QString ExtensionParser::typeName(Type type)
{
    switch (type) {
        case Type::Array:
            return "Array";
        case Type::Bool:
            return "Bool";
        case Type::Double:
            return "Double";
        case Type::Null:
            return "Null";
        case Type::Object:
            return "Object";
        case Type::String:
            return "String";
        case Type::Undefined:
            return "Undefined";
        default:
            return "Unknown";
    }
}
