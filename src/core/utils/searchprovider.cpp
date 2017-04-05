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

#include <QDebug>

#include "searchprovider.h"

SearchProvider::SearchProvider(QObject *parent)
    : QObject(parent)
{
    m_defaultSearchEngine = "default.duckduckgo";
    m_reDynamicValue = QRegularExpression(R"(\$\(\s?([A-Za-z_.]+)\s?\))");
}

QString SearchProvider::searchUrl(QString query, QString engine, ExtensionTheme* theme) const
{
    return url(ExtensionSearchEngineParameter::Search, query, engine, theme);
}

QString SearchProvider::homepage(QString engine, ExtensionTheme *theme) const
{
    return url(ExtensionSearchEngineParameter::Homepage, "", engine, theme);
}

QString SearchProvider::url(ExtensionSearchEngineParameter::SearchContext context, QString query, QString engine, ExtensionTheme *theme) const
{
    ExtensionSearchEngine* searchEngine = m_model->get(engine);
    if (!searchEngine->valid()) {
        qWarning() << "Search engine" << engine << "was requested but could not not be found.";
        qWarning() << "Falling back to default search engine" << m_defaultSearchEngine << "...";
        searchEngine = m_model->get(m_defaultSearchEngine);
    }
    QList<ExtensionSearchEngineParameter*>* params = searchEngine->parameters();
    QString urlString;
    if (context == ExtensionSearchEngineParameter::Search) {
        urlString = searchEngine->urlBaseSearch();
    } else if (context == ExtensionSearchEngineParameter::Homepage) {
        urlString = searchEngine->urlBaseHomepage();
    }
    QString getString;
    QString hashbangString;
    bool firstGet = true;
    bool uniqueHashbang = true;
    for (int i=0; i<params->count(); i++) {
        ExtensionSearchEngineParameter* param = params->at(i);
        if (param->context() & context) {
            ExtensionSearchEngineParameter::Type type = param->type();
            if (type == ExtensionSearchEngineParameter::Get) {
                QString name = param->name();
                QString value = param->value();
                QString paramString;
                if (firstGet)
                    paramString += QString('?') + name + '=';
                else
                    paramString += QString('&') + name + '=';
                paramString.append(dynamicValue(value, query, theme));
                firstGet = false;
                getString.append(paramString);
            } else if (type == ExtensionSearchEngineParameter::Hashbang) {
                if (uniqueHashbang) {    // Only allow one hashbang per context
                    QString value = param->value();
                    hashbangString = "#!";
                    hashbangString.append(dynamicValue(value, query, theme));
                    uniqueHashbang = false;
                }
            }
        }
    }
    urlString.append(getString + hashbangString);
    return urlString;
}

QString SearchProvider::dynamicValue(QString value, QString query, ExtensionTheme* theme) const
{
    QRegularExpressionMatchIterator matchIterator = m_reDynamicValue.globalMatch(value);
    int offset = 0;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch dynamicValueMatch = matchIterator.next();
        if (dynamicValueMatch.hasMatch()) {
            QString fieldName = dynamicValueMatch.captured(1);
            QString replaceValue;
            if (fieldName == "search.query") {
                replaceValue = query;
            } else if (fieldName == "theme.background") {
                replaceValue = theme->background().name().replace("#", "%23");
            }
            value.replace(dynamicValueMatch.capturedStart() + offset, dynamicValueMatch.capturedLength(), replaceValue);
            offset += (replaceValue.length() - dynamicValueMatch.capturedLength());
        }
    }
    return value;
}
