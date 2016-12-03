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

#ifndef TABSMODEL_H
#define TABSMODEL_H

#include <QAbstractListModel>
#include "tab.h"

class TabsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged)
    Q_PROPERTY(Tab* active READ active NOTIFY activeChanged)
    Q_PROPERTY(int activeIndex READ activeIndex NOTIFY activeIndexChanged)
public:
    explicit TabsModel(QObject *parent = 0);

    enum TabRoles {
        Uid,
        Url,
        Title,
        IconUrl,
        CanGoBack,
        CanGoForward,
        Loading,
        LoadProgress,
        Invalid
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE Tab* get(const int index) const;
    Q_INVOKABLE void add(unsigned int uid);
    Q_INVOKABLE bool move(int fromRow, int toRow);
    Q_INVOKABLE int count() const;
    Q_INVOKABLE bool remove(unsigned int uid);
    Q_INVOKABLE bool remove(Tab* tab);
    Q_INVOKABLE bool empty() const;

    Q_INVOKABLE Tab* byUID(unsigned int uid);

    Q_INVOKABLE bool setActive(unsigned int uid);
    Q_INVOKABLE bool setActive(Tab* tab);
    Q_INVOKABLE Tab* active() const;
    Q_INVOKABLE int activeIndex() const;

    Q_INVOKABLE void setInactive(Tab* tab);
    Q_INVOKABLE void setInactive();

    Q_INVOKABLE bool setPreviousTabActive();

signals:
    void countChanged(int count);
    void emptyChanged(bool empty);
    void activeChanged(Tab* tab);
    void activeIndexChanged(int index);
    void beforeTabRemoved(Tab* tab);

public slots:

private:
    QList<Tab*> m_tabs_list;
    QList<Tab*> m_active_tab_history;
    Tab* m_active_tab;
    Tab* m_invalid_tab;
};

#endif // TABSMODEL_H
