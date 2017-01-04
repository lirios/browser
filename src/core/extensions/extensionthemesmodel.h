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

#ifndef EXTENSIONTHEMESMODEL_H
#define EXTENSIONTHEMESMODEL_H

#include <QAbstractListModel>

#include "extensiontheme.h"

class ExtensionThemesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit ExtensionThemesModel(QObject *parent = nullptr);

    enum ExtensionRoles {
        Name = Qt::UserRole + 1,
        ExtensionName,
        Title,
        Summary,
        Description,
        Dark,
        Accent,
        Primary,
        Foreground,
        Background
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE int count() const;
    void add(ExtensionTheme* theme);
    void remove(const int index);
    void remove(ExtensionTheme* theme);
    void removeFromExtension(const QString extensionName);
    Q_INVOKABLE ExtensionTheme* get(const int index) const;
    Q_INVOKABLE ExtensionTheme* get(const QString name) const;

signals:
    void countChanged(int count);

private:
    QList<ExtensionTheme*> m_themes;
    ExtensionTheme* m_invalidTheme;
};

#endif // EXTENSIONTHEMESMODEL_H
