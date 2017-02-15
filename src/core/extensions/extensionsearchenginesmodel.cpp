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

#include "extensionsearchenginesmodel.h"

ExtensionSearchEnginesModel::ExtensionSearchEnginesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_invalidSearchEngine = new ExtensionSearchEngine(this);
    m_invalidSearchEngine->setValid(false);
}

int ExtensionSearchEnginesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_searchEngines.length();
}

QHash<int, QByteArray> ExtensionSearchEnginesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ExtensionName] = "extensionName";
    roles[Name] = "name";
    roles[Title] = "title";
    roles[Summary] = "summary";
    roles[Description] = "description";
    return roles;
}

QVariant ExtensionSearchEnginesModel::data(const QModelIndex &index, int role) const
{
    ExtensionSearchEngine* searchEngine = get(index.row());
    switch (role) {
        case ExtensionName:
            return searchEngine->extensionName();
        case Name:
            return searchEngine->name();
        case Title:
            return searchEngine->title();
        case Summary:
            return searchEngine->summary();
        case Description:
            return searchEngine->description();
    }
    return QVariant();
}

int ExtensionSearchEnginesModel::count() const
{
    return m_searchEngines.length();
}

void ExtensionSearchEnginesModel::add(ExtensionSearchEngine *searchEngine)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_searchEngines.append(searchEngine);
    countChanged(count());
    endInsertRows();
}

void ExtensionSearchEnginesModel::remove(const int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ExtensionSearchEngine* searchEngine = m_searchEngines.at(row);
    m_searchEngines.removeAt(row);
    searchEngine->deleteLater();
    countChanged(count());
    endRemoveRows();
}

void ExtensionSearchEnginesModel::remove(ExtensionSearchEngine *searchEngine)
{
    int index = m_searchEngines.indexOf(searchEngine);
    if (index != -1)
        remove(index);
}

void ExtensionSearchEnginesModel::removeFromExtensionName(const QString extensionName)
{
    for (ExtensionSearchEngine* searchEngine : m_searchEngines) {
        if (searchEngine->extensionName() == extensionName) {
            remove(searchEngine);
        }
    }
}

ExtensionSearchEngine *ExtensionSearchEnginesModel::get(const int row) const
{
    if (row < 0 || row >= count()) {
        return m_invalidSearchEngine;
    }
    return m_searchEngines.at(row);
}

ExtensionSearchEngine *ExtensionSearchEnginesModel::get(const QString name) const
{
    for (ExtensionSearchEngine* searchEngine : m_searchEngines) {
        if (searchEngine->name() == name)
            return searchEngine;
    }
    return m_invalidSearchEngine;
}
