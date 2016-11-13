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

#ifndef DOWNLOADSMODEL_H
#define DOWNLOADSMODEL_H

#include <QAbstractListModel>
#include "webdownload.h"

class DownloadsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit DownloadsModel(QObject* parent = 0);

    enum DownloadRoles {
        Path,
        MimeType,
        Progress,
        Invalid
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames();
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE int count() const;
    Q_INVOKABLE WebDownload* add();
    Q_INVOKABLE WebDownload* get(const int index) const;
    Q_INVOKABLE bool remove(WebDownload* download);
    Q_INVOKABLE bool remove(const int index);

private:
    WebDownload* m_invalid_download;
    QList<WebDownload*> m_downloads_list;

signals:
    void countChanged(int count);
};

#endif // DOWNLOADSMODEL_H
