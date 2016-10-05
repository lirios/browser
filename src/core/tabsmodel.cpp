/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb (https://github.com/tim-sueberkrueb)
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
 */

#include "tabsmodel.h"
#include "tab.h"
#include <QDebug>

TabsModel::TabsModel(QObject *parent) : QAbstractListModel(parent)
{
    m_invalid_tab = new Tab(this, true);
    m_active_tab = m_invalid_tab;
}

int TabsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_tabs_list.length();
}

QVariant TabsModel::data(const QModelIndex &index, int role) const
{
    Tab* tab = get(index.row());
    switch(role) {
        case Uid:
            return QVariant(tab->uid());
            break;
    }
    return QVariant();
}

QHash<int, QByteArray> TabsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Uid] = "uid";
    return roles;
}

Tab* TabsModel::get(const int index) const
{
    if (index < 0 || index > rowCount()) {
        return m_invalid_tab;
    }
    return m_tabs_list.at(index);
}

void TabsModel::add(unsigned int uid)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Tab* tab = new Tab(this);
    tab->setUid(uid);
    m_tabs_list.append(tab);
    countChanged(count());
    emptyChanged(false);
    endInsertRows();
}

bool TabsModel::move(int fromRow, int toRow)
{
    if (fromRow == toRow)
        return false;
    if (fromRow >= rowCount() || toRow >= rowCount())
        return false;

    // QList's and QAbstractItemModel's move implementation differ when moving an item up the list :/
    // While QList needs the index in the resulting list, beginMoveRows expects it to be in the current list
    // adjust the model's index by +1 in case we're moving upwards
    // Thanks to Michael Zanetti
    int newToRow = toRow > fromRow ? toRow + 1 : toRow;

    beginMoveRows(QModelIndex(), fromRow, fromRow, QModelIndex(), newToRow);
    m_tabs_list.move(fromRow, toRow);

    // Update active index
    activeIndexChanged(activeIndex());

    endMoveRows();
    return true;
}

int TabsModel::count() const
{
    return rowCount();
}

bool TabsModel::remove(unsigned int uid)
{
    return remove(byUID(uid));
}

bool TabsModel::remove(Tab *tab)
{
    int index = m_tabs_list.indexOf(tab);
    if (index == -1)
        return false;
    beginRemoveRows(QModelIndex(), index, index);
    m_tabs_list.removeAt(index);

    // Remove tab from active tab history
    m_active_tab_history.removeAll(tab);
    tab->deleteLater();

    // Update count and empty
    countChanged(count());
    emptyChanged(empty());

    // Active tab is replaced by invalid dummy tab
    if (empty()) {
        m_active_tab = m_invalid_tab;
    }

    endRemoveRows();
    return true;
}

bool TabsModel::empty() const
{
    return count() == 0;
}

Tab *TabsModel::byUID(unsigned int uid)
{
    for (Tab* tab: m_tabs_list) {
        if (tab->uid() == uid)
            return tab;
    }
    return m_invalid_tab;
}

bool TabsModel::setActive(unsigned int uid)
{
    return setActive(byUID(uid));
}

bool TabsModel::setActive(Tab *tab)
{
    if (tab->invalid())
        return false;

    m_active_tab = tab;

    // Add tab to active tab history
    if (m_active_tab_history.lastIndexOf(tab) != m_active_tab_history.length())
        m_active_tab_history.append(tab);

    activeChanged(tab);
    activeIndexChanged(activeIndex());
    return true;
}

Tab *TabsModel::active() const
{
    return m_active_tab;
}

int TabsModel::activeIndex() const
{
    return m_tabs_list.indexOf(m_active_tab);
}

void TabsModel::setInactive(Tab *tab)
{
    if (m_active_tab == tab)
        setInactive(tab);
}

void TabsModel::setInactive()
{
    setInactive(m_active_tab);
}

bool TabsModel::setPreviousTabActive()
{
    if (m_tabs_list.length() == 0) {
        activeChanged(m_active_tab = m_invalid_tab);
        activeIndexChanged(activeIndex());
        return false;
    }
    if (m_active_tab_history.length() == 0) {
        setActive(m_tabs_list.first());
        return true;
    }
    setActive(m_active_tab_history.last());
    return true;
}
