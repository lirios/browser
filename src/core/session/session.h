/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Ivan Fateev <johnny.j.poison@gmail.com>
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

#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QVariantList>
#include <QList>

class TabsModel;
class TabState;

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = 0);

    Q_INVOKABLE void save(TabsModel* tabs);
    Q_INVOKABLE QVariantList getTabsToRestore();
    Q_PROPERTY(int activeTab READ getActiveTab)

    int getActiveTab() { return m_activeTab; }
private:
    void load();
    QByteArray json(TabsModel *tabs);

private:
    QList<TabState*> m_tabs;
    int m_activeTab;
};

#endif // SESSION_H
