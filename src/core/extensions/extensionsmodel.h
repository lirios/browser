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

#ifndef EXTENSIONSMODEL_H
#define EXTENSIONSMODEL_H

#include <QAbstractListModel>

#include "extension.h"

class ExtensionsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit ExtensionsModel(QObject *parent = nullptr);

    enum Roles {
        Name = Qt::UserRole + 1,
        Title,
        Description,
        Summary,
        Author,
        Email
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE int count() const;
    void add(Extension* extension);
    void remove(const int index);
    void remove(Extension* extension);
    Q_INVOKABLE Extension *get(const int index) const;
    Q_INVOKABLE Extension *get(const QString name) const;
    bool hasName(const QString name) const;

signals:
    void countChanged(int count);

private:
    QList<Extension*> m_extensions;
    Extension* m_invalidExtension;
};

#endif // EXTENSIONSMODEL_H
