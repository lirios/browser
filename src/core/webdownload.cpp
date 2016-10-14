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

#include "webdownload.h"

WebDownload::WebDownload(QObject *parent) : QObject(parent)
{
    m_finished = false;
}

void WebDownload::setPath(QString path)
{
    pathChanged(m_path = path);
}

QString WebDownload::path() const
{
    return m_path;
}

void WebDownload::setMimeType(QString mimeType)
{
    mimeTypeChanged(m_mimeType = mimeType);
}

QString WebDownload::mimeType() const
{
    return m_mimeType;
}

void WebDownload::setProgress(int progress)
{
    progressChanged(m_progress = progress);
}

int WebDownload::progress() const
{
    return m_progress;
}

void WebDownload::setFinished(bool finished)
{
    finishedChanged(m_finished = finished);
}

bool WebDownload::finished() const
{
    return m_finished;
}

void WebDownload::setInvalid(bool invalid)
{
    invalidChanged(m_invalid = invalid);
}

bool WebDownload::invalid() const
{
    return m_invalid;
}
