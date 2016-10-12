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

#ifndef WEBDOWNLOAD_H
#define WEBDOWNLOAD_H

#include <QObject>

class WebDownload : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString mimeType READ mimeType  WRITE setMimeType  NOTIFY mimeTypeChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(bool finished READ finished WRITE setFinished NOTIFY finishedChanged)
    Q_PROPERTY(bool invalid READ invalid WRITE setInvalid NOTIFY invalidChanged)
public:
    explicit WebDownload(QObject *parent = 0);

    void setPath(QString path);
    QString path() const;

    void setMimeType(QString mimeType);
    QString mimeType() const;

    void setProgress(int progress);
    int progress() const;

    void setFinished(bool finished);
    bool finished() const;

    void setInvalid(bool invalid);
    bool invalid() const;


signals:
    void pathChanged(QString path);
    void mimeTypeChanged(QString mimeType);
    void progressChanged(int progress);
    void finishedChanged(bool finished);
    void invalidChanged(bool invalid);

    void cancel();

private:
    QString m_path;
    QString m_mimeType;
    int m_progress;
    bool m_invalid;
    bool m_finished;


public slots:
};

#endif // WEBDOWNLOAD_H
