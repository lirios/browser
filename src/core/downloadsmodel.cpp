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

#include "downloadsmodel.h"

DownloadsModel::DownloadsModel(QObject* parent) : QAbstractListModel(parent)
{
    m_invalid_download = new WebDownload(this);
    m_invalid_download->setInvalid(true);
}

int DownloadsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_downloads_list.length();
}

QHash<int, QByteArray> DownloadsModel::roleNames()
{
    QHash<int, QByteArray> roles;
    roles[Path] = "path";
    roles[MimeType] = "mimeType";
    roles[Progress] = "progress";
    return roles;
}

QVariant DownloadsModel::data(const QModelIndex &index, int role) const
{
    WebDownload* download = get(index.row());
    switch (role) {
        case Path:
            return QVariant(download->path());
        case MimeType:
            return QVariant(download->mimeType());
        case Progress:
            return QVariant(download->progress());
        case Invalid:
            return QVariant(download->invalid());
    }
    return QVariant();
}

int DownloadsModel::count() const
{
    return m_downloads_list.length();
}

WebDownload* DownloadsModel::add()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    WebDownload* download = new WebDownload(this);
    m_downloads_list.append(download);
    countChanged(count());
    endInsertRows();
    return download;
}

WebDownload* DownloadsModel::get(const int index) const
{
    if (index < 0 || index > rowCount()) {
        return m_invalid_download;
    }
    return m_downloads_list.at(index);
}

bool DownloadsModel::remove(WebDownload *download)
{
    int index = m_downloads_list.indexOf(download);
    if (index == -1)
        return false;
    beginRemoveRows(QModelIndex(), index, index);
    m_downloads_list.removeAt(index);
    countChanged(count());
    endRemoveRows();
    return true;
}

bool DownloadsModel::remove(const int index)
{
    return remove(get(index));
}
