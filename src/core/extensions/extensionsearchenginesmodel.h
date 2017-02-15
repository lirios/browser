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

#ifndef EXTENSIONSEARCHENGINESMODEL_H
#define EXTENSIONSEARCHENGINESMODEL_H

#include <QAbstractListModel>

#include "extensionsearchengine.h"

class ExtensionSearchEnginesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit ExtensionSearchEnginesModel(QObject *parent = nullptr);

    enum Roles {
        ExtensionName = Qt::UserRole + 1,
        Name,
        Title,
        Summary,
        Description
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE int count() const;
    void add(ExtensionSearchEngine* searchEngine);
    void remove(const int row);
    void remove(ExtensionSearchEngine* searchEngine);
    void removeFromExtensionName(const QString extensionName);
    Q_INVOKABLE ExtensionSearchEngine *get(const int row) const;
    Q_INVOKABLE ExtensionSearchEngine *get(const QString name) const;

signals:
    void countChanged(int count);

private:
    QList<ExtensionSearchEngine*> m_searchEngines;
    ExtensionSearchEngine* m_invalidSearchEngine;
};

#endif // EXTENSIONSEARCHENGINESMODEL_H
