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

}

QString SearchProvider::searchUrl(QString query, QString engine, ExtensionTheme* theme) const
{
    ExtensionSearchEngine* searchEngine = m_model->get(engine);
    QList<ExtensionSearchEngineParameter*>* params = searchEngine->parameters();
    QString urlString = searchEngine->urlBase() + '?';
    bool first = true;
    for (int i=0; i<params->count(); i++) {
        ExtensionSearchEngineParameter* param = params->at(i);
        if (param->context() & ExtensionSearchEngineParameter::Search) {
            QString name = param->name();
            QString value = param->value();
            QString paramString = first ? name + '=' : QString('&') + name + '=';
            if (value == "search.query") {
                paramString.append(query);
            }
            else if (value == "theme.background") {
                paramString.append(theme->background().name().replace("#", "%23"));
            }
            else {
                paramString.append(value);
            }
            urlString.append(paramString);
            first = false;
        }
    }
    return urlString;
}

QString SearchProvider::homepage(QString engine, ExtensionTheme *theme) const
{
    ExtensionSearchEngine* searchEngine = m_model->get(engine);
    QList<ExtensionSearchEngineParameter*>* params = searchEngine->parameters();
    QString urlString = searchEngine->urlBase() + '?';
    bool first = true;
    for (int i=0; i<params->count(); i++) {
        ExtensionSearchEngineParameter* param = params->at(i);
        if (param->context() & ExtensionSearchEngineParameter::Homepage) {
            QString name = param->name();
            QString value = param->value();
            QString paramString = first ? name + '=' : QString('&') + name + '=';
            if (value == "theme.background") {
                paramString.append(theme->background().name().replace("#", "%23"));
            }
            else {
                paramString.append(value);
            }
            urlString.append(paramString);
            first = false;
        }
    }
    return urlString;
}
