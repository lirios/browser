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

#include "extensionsmodel.h"

#include <QDebug>

ExtensionsModel::ExtensionsModel(QObject *parent) : QAbstractListModel(parent)
{
    m_invalidExtension = new Extension();
    m_invalidExtension->setValid(false);
}

int ExtensionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_extensions.length();
}

QHash<int, QByteArray> ExtensionsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Title] = "title";
    roles[Description] = "description";
    roles[Summary] = "summary";
    roles[Author] = "author";
    roles[Email] = "email";
    return roles;
}

QVariant ExtensionsModel::data(const QModelIndex &index, int role) const
{
    Extension* extension = get(index.row());
    switch (role) {
        case Name:
            return QVariant(extension->name());
        case Title:
            return QVariant(extension->title());
        case Description:
            return QVariant(extension->description());
        case Summary:
            return QVariant(extension->summary());
        case Author:
            return QVariant(extension->author());
        case Email:
            return QVariant(extension->email());
    }
    return QVariant();
}

int ExtensionsModel::count() const
{
    return m_extensions.length();
}

void ExtensionsModel::add(Extension *extension)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_extensions.append(extension);
    countChanged(count());
    endInsertRows();
}

void ExtensionsModel::remove(const int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Extension* extension = m_extensions.at(row);
    m_extensions.removeAt(row);
    extension->deleteLater();
    countChanged(count());
    endRemoveRows();
}

void ExtensionsModel::remove(Extension *extension)
{
    int index = m_extensions.indexOf(extension);
    if (index != -1)
        remove(index);
}

Extension *ExtensionsModel::get(const int row) const
{
    if (row < 0 || row >= count()) {
        return m_invalidExtension;
    }
    return m_extensions.at(row);
}

Extension *ExtensionsModel::get(const QString name) const
{
    for (Extension* extension : m_extensions) {
        if (extension->name() == name)
            return extension;
    }
    return m_invalidExtension;
}

bool ExtensionsModel::hasName(const QString name) const
{
    for (Extension* extension : m_extensions) {
        if (extension->name() == name)
            return true;
    }
    return false;
}
