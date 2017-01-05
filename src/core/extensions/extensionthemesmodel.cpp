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

#include "extensionthemesmodel.h"

ExtensionThemesModel::ExtensionThemesModel(QObject *parent) : QAbstractListModel(parent)
{
    m_invalidTheme = new ExtensionTheme(this);
    m_invalidTheme->setValid(false);
}

int ExtensionThemesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_themes.length();
}

QHash<int, QByteArray> ExtensionThemesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[ExtensionName] = "extensionName";
    roles[Title] = "title";
    roles[Summary] = "summary";
    roles[Description] = "description";
    roles[Accent] = "accent";
    roles[Primary] = "primary";
    roles[Foreground] = "foreground";
    roles[Background] = "background";
    return roles;
}

QVariant ExtensionThemesModel::data(const QModelIndex &index, int role) const
{
    ExtensionTheme* theme = get(index.row());
    switch (role) {
        case Name:
            return QVariant(theme->name());
        case ExtensionName:
            return QVariant(theme->extensionName());
        case Title:
            return QVariant(theme->title());
        case Summary:
            return QVariant(theme->summary());
        case Description:
            return QVariant(theme->description());
        case Accent:
            return QVariant(theme->accent());
        case Primary:
            return QVariant(theme->primary());
        case Foreground:
            return QVariant(theme->foreground());
        case Background:
            return QVariant(theme->background());
    }
    return QVariant();
}

int ExtensionThemesModel::count() const
{
    return m_themes.length();
}

void ExtensionThemesModel::add(ExtensionTheme *theme)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_themes.append(theme);
    countChanged(count());
    endInsertRows();
}

void ExtensionThemesModel::remove(const int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ExtensionTheme* theme = m_themes.at(row);
    m_themes.removeAt(row);
    theme->deleteLater();
    countChanged(count());
    endRemoveRows();
}

void ExtensionThemesModel::remove(ExtensionTheme* theme)
{
    int row = m_themes.indexOf(theme);
    if (row != -1)
        remove(row);
}

void ExtensionThemesModel::removeFromExtensionName(const QString extensionName)
{
    for (ExtensionTheme* theme : m_themes) {
        if (theme->extensionName() == extensionName) {
            remove(theme);
        }
    }
}

ExtensionTheme *ExtensionThemesModel::get(const int row) const
{
    if (row < 0 || row > count()) {
        return m_invalidTheme;
    }
    return m_themes.at(row);
}

ExtensionTheme *ExtensionThemesModel::get(const QString name) const
{
    for (ExtensionTheme* theme : m_themes) {
        if (theme->name() == name)
            return theme;
    }
    return m_invalidTheme;
}
