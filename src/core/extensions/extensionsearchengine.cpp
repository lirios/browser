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

#include "extensionsearchengine.h"

ExtensionSearchEngine::ExtensionSearchEngine(QObject *parent)
    : ExtensionElement(parent)
{
    m_parameters = new QList<ExtensionSearchEngineParameter*>();
}

ExtensionSearchEngine *ExtensionSearchEngine::clone(QObject *parent)
{
    ExtensionSearchEngine* searchEngine = new ExtensionSearchEngine(parent);
    searchEngine->setName(m_name);
    searchEngine->setExtensionName(m_extensionName);
    searchEngine->setTitle(m_title);
    searchEngine->setSummary(m_summary);
    searchEngine->setDescription(m_description);
    searchEngine->setUrlBaseSearch(m_urlBaseSearch);
    searchEngine->setUrlBaseHomepage(m_urlBaseHomepage);
    for (int i=0; i<m_parameters->length(); i++) {
        searchEngine->parameters()->append(m_parameters->at(i)->clone());
    }
    return searchEngine;
}
