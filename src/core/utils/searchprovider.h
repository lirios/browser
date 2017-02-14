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

#ifndef SEARCHPROVIDER_H
#define SEARCHPROVIDER_H

#include <QObject>
#include <QList>

#include "../extensions/extensiontheme.h"
#include "../extensions/extensionsearchengine.h"
#include "../extensions/extensionsearchengineparameter.h"
#include "../extensions/extensionsearchenginesmodel.h"

class SearchProvider : public QObject
{
    Q_OBJECT
public:
    explicit SearchProvider(QObject *parent = nullptr);

    ExtensionSearchEnginesModel* model() const { return m_model; }
    void setModel(ExtensionSearchEnginesModel* model) { modelChanged(m_model = model); }

    Q_INVOKABLE QString searchUrl(QString query, QString engine, ExtensionTheme* theme) const;
    Q_INVOKABLE QString homepage(QString engine, ExtensionTheme* theme) const;

signals:
    void modelChanged(ExtensionSearchEnginesModel* model);

private:
    QString m_engine;
    ExtensionSearchEnginesModel* m_model;
};

#endif // SEARCHPROVIDER_H
