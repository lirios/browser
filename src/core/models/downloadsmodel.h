/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <dev@timsueberkrueb.io>
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

#ifndef DOWNLOADSMODEL_H
#define DOWNLOADSMODEL_H

#include <QAbstractListModel>
#include <QtWebEngine/QQuickWebEngineProfile>

class DownloadsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit DownloadsModel(QObject* parent = nullptr);

    enum DownloadRoles {
        Path,
        MimeType,
        Progress,
        Status
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames() { return QHash<int, QByteArray>(); }
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const {
        Q_UNUSED(index)
        Q_UNUSED(role)
        return QVariant();
    }

    Q_INVOKABLE int count() const;
    Q_INVOKABLE void add(QQuickWebEngineDownloadItem* download);
    Q_INVOKABLE QQuickWebEngineDownloadItem* get(const int index) const;
    Q_INVOKABLE bool remove(QQuickWebEngineDownloadItem* download);
    Q_INVOKABLE bool remove(const int index);

private:
    QList<QQuickWebEngineDownloadItem*> m_downloads_list;

signals:
    void countChanged(int count);
};

#endif // DOWNLOADSMODEL_H
