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
    m_invalid_tab = new Tab(this, true);
    m_active_tab = m_invalid_tab;
    m_history_offset = 0;
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
        case Invalid:
            return QVariant(tab->invalid());
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
    roles[Invalid] = "invalid";
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

    int activeIdx = activeIndex();
    // Removed the active tab
    // which assumes changing the active tab to previous one
    bool switchToPrev = index > 0 && activeIdx == index && m_tabs_list.length() > 1;
    if (switchToPrev)
    {
        // Change to previous if we can
        activateTabRelativeToCurrent(-1);
    }

    // Call signal before removing and deleting tab
    beforeTabRemoved(tab);

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

    if (index == 0 && activeIdx == 0 && !empty()) {
        setActive(get(0));
    } else if (!switchToPrev) {
        return setActive(active(), false);
    }

    m_active_tab_history.removeAll(tab);
    m_history_offset = 0;

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
    return setActive(tab, true);
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
    return moveByHistory(1);
}

bool TabsModel::setNextTabActive()
{
    return moveByHistory(-1);
}

bool TabsModel::activateTabRelativeToCurrent(int offset)
{
    if (m_tabs_list.length() == 0) {
        activeChanged(m_active_tab = m_invalid_tab);
        activeIndexChanged(activeIndex());
        return false;
    }

    int currIndex = activeIndex();
    if (currIndex == -1)
    {
        setActive(m_tabs_list.last(), false);
        return true;
    }

    int newIndex = currIndex + offset;
    if (newIndex < 0 || newIndex >= m_tabs_list.length())
    {
        return false;
    }

    return setActive(m_tabs_list.at(newIndex), false);
}

bool TabsModel::moveByHistory(int offset)
{
    if (m_tabs_list.length() == 0) {
        activeChanged(m_active_tab = m_invalid_tab);
        activeIndexChanged(activeIndex());
        return false;
    }
    if (m_active_tab_history.length() == 0) {
        setActive(m_tabs_list.first(), false);
        return true;
    }

    int historyIndex = m_active_tab_history.length() + m_history_offset + offset - 1;
    if (historyIndex < 0 || historyIndex >= m_active_tab_history.length())
    {
        return false;
    }

    auto tab = m_active_tab_history.at(historyIndex);

    m_history_offset += offset;
    return setActive(tab, false);
}

void TabsModel::appendToHistory(Tab *tab)
{
    m_active_tab_history.append(tab);
    if (m_history_offset == 0)
    {
        return;
    }


    auto len = m_active_tab_history.length();
    auto lastIndex = std::max(0, len - 1 + m_history_offset);
    for(int i = len - 1; i >= lastIndex; --i)
    {
        m_active_tab_history.removeAt(i);
    }

    m_history_offset = 0;
}

bool TabsModel::setActive(Tab *tab, bool recordToHistory)
{
    if (tab->invalid())
        return false;

    m_active_tab = tab;

    // Add tab to active tab history
    if (recordToHistory)
    {
        appendToHistory(tab);
    }

    activeChanged(tab);
    activeIndexChanged(activeIndex());
    return true;
}
