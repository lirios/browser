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

#ifndef THEMEPROVIDER_H
#define THEMEPROVIDER_H

#include <QObject>

#include "../extensions/extensionthemesmodel.h"
#include "../extensions/extensiontheme.h"

class ThemeProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(ExtensionTheme* current READ current NOTIFY currentChanged)
public:
    explicit ThemeProvider(QObject *parent = 0);

    QString name() const { return m_name; }
    void setName(QString name) { nameChanged(m_name = name); }

    QString defaultName() const { return m_defaultName; }
    void setDefaultName(QString defaultName) { m_defaultName = defaultName; }

    ExtensionThemesModel* model() { return m_model; }
    void setModel(ExtensionThemesModel* model);

    ExtensionTheme* current() { return m_current; }

signals:
    void nameChanged(QString name);
    void modelChanged(ExtensionThemesModel* model);
    void currentChanged(ExtensionTheme* current);

private:
    QString m_name;
    QString m_defaultName;
    ExtensionThemesModel* m_model;
    ExtensionTheme* m_current;

private slots:
    void update();

};

#endif // THEMEPROVIDER_H
