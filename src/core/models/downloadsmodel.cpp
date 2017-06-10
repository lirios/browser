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

#include "downloadsmodel.h"

DownloadsModel::DownloadsModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int DownloadsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_downloads_list.length();
}

int DownloadsModel::count() const
{
    return m_downloads_list.length();
}

void DownloadsModel::add(QQuickWebEngineDownloadItem* download)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_downloads_list.append(download);
    countChanged(count());
    endInsertRows();
}

QQuickWebEngineDownloadItem* DownloadsModel::get(const int index) const
{
    if (index < 0 || index >= rowCount()) {
        return nullptr;
    }
    return m_downloads_list.at(index);
}

bool DownloadsModel::remove(QQuickWebEngineDownloadItem *download)
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
