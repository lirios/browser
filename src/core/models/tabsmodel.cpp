/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb <tim.sueberkrueb@web.de>
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

#include "tabsmodel.h"
#include "tab.h"
#include <QDebug>

TabsModel::TabsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_invalidTab = new Tab(this, false);
    m_activeTab = m_invalidTab;
}

int TabsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_tabsList.length();
}

QVariant TabsModel::data(const QModelIndex &index, int role) const
{
    Tab* tab = get(index.row());
    switch(role) {
        case Uid:
            return QVariant(tab->uid());
        case Url:
            return QVariant(tab->url());
        case Title:
            return QVariant(tab->title());
        case IconUrl:
            return QVariant(tab->iconUrl());
        case CanGoBack:
            return QVariant(tab->canGoBack());
        case CanGoForward:
            return QVariant(tab->canGoForward());
        case Loading:
            return QVariant(tab->loading());
        case LoadProgress:
            return QVariant(tab->loadProgress());
        case Valid:
            return QVariant(tab->valid());
    }
    return QVariant();
}

QHash<int, QByteArray> TabsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Uid] = "uid";
    roles[Url] = "url";
    roles[Title] = "title";
    roles[IconUrl] = "iconUrl";
    roles[CanGoBack] = "canGoBack";
    roles[CanGoForward] = "canGoForward";
    roles[Loading] = "loading";
    roles[LoadProgress] = "loadProgress";
    roles[Valid] = "valid";
    return roles;
}

Tab* TabsModel::get(const int index) const
{
    if (index < 0 || index > rowCount()) {
        return m_invalidTab;
    }
    return m_tabsList.at(index);
}

void TabsModel::add(unsigned int uid)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Tab* tab = new Tab(this);
    tab->setUid(uid);
    m_tabsList.append(tab);
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
    m_tabsList.move(fromRow, toRow);

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
    int index = m_tabsList.indexOf(tab);
    if (index == -1)
        return false;

    int activeIdx = activeIndex();

    // Call signal before removing and deleting tab
    beforeTabRemoved(tab);

    beginRemoveRows(QModelIndex(), index, index);
    m_tabsList.removeAt(index);

    // Remove tab from active tab history
    m_activeTabHistory.removeAll(tab);
    tab->deleteLater();

    // Update count and empty
    countChanged(count());
    emptyChanged(empty());

    // Active tab is replaced by invalid dummy tab
    if (empty()) {
        m_activeTab = m_invalidTab;
    }

    endRemoveRows();

    if (index == activeIdx) {
        setActive(!m_activeTabHistory.empty()
                        ? m_activeTabHistory.last()
                        : m_tabsList.first(),
                  false);
    } else if (index < activeIdx) {
        return setActive(active(), false);
    }

    m_activeTabHistory.removeAll(tab);

    return true;
}

bool TabsModel::empty() const
{
    return count() == 0;
}

Tab *TabsModel::byUID(unsigned int uid)
{
    for (Tab* tab: m_tabsList) {
        if (tab->uid() == uid)
            return tab;
    }
    return m_invalidTab;
}

bool TabsModel::setActive(unsigned int uid)
{
    return setActive(byUID(uid));
}

bool TabsModel::setActive(Tab *tab)
{
    return setActive(tab, true);
}

Tab *TabsModel::active() const
{
    return m_activeTab;
}

int TabsModel::activeIndex() const
{
    return m_tabsList.indexOf(m_activeTab);
}

void TabsModel::setInactive(Tab *tab)
{
    if (m_activeTab == tab)
        setInactive(tab);
}

void TabsModel::setInactive()
{
    setInactive(m_activeTab);
}

bool TabsModel::setPreviousTabActive()
{
    if (m_activeTab == m_tabsList.first() && m_tabsList.length() > 1) {
        return setActive(m_tabsList.last(), true);
    }
    return activateTabRelativeToCurrent(-1);
}

bool TabsModel::setNextTabActive()
{
    if (m_activeTab == m_tabsList.last() && m_tabsList.length() > 1) {
        return setActive(m_tabsList.first(), true);
    }
    return activateTabRelativeToCurrent(1);
}

bool TabsModel::activateTabRelativeToCurrent(int offset)
{
    if (m_tabsList.length() == 0) {
        activeChanged(m_activeTab = m_invalidTab);
        activeIndexChanged(activeIndex());
        return false;
    }

    int currIndex = activeIndex();
    if (currIndex == -1) {
        setActive(m_tabsList.last(), true);
        return true;
    }

    int newIndex = currIndex + offset;
    if (newIndex < 0 || newIndex >= m_tabsList.length()) {
        return false;
    }

    return setActive(m_tabsList.at(newIndex), true);
}

bool TabsModel::setActive(Tab *tab, bool recordToHistory)
{
    if (!tab->valid())
        return false;

    // Adding previously active tab to history
    // so currently active tab is not a last member of history
    if (recordToHistory) {
        m_activeTabHistory.append(m_activeTab);
    }

    m_activeTab = tab;


    activeChanged(tab);
    activeIndexChanged(activeIndex());
    return true;
}
