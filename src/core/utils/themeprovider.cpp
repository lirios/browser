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

#include "themeprovider.h"

#include <QDebug>

ThemeProvider::ThemeProvider(QObject *parent)
    : QObject(parent)
{
    m_defaultName = "default.light";
    connect(this, &ThemeProvider::nameChanged,
            this, &ThemeProvider::update);
}

void ThemeProvider::setModel(ExtensionThemesModel *model)
{
    modelChanged(m_model = model);
    connect(m_model, &ExtensionThemesModel::countChanged,
            this, &ThemeProvider::update);
}

void ThemeProvider::update()
{
    // Get theme by name
    ExtensionTheme* theme = m_model->get(m_name);
    // Fallback to default theme if theme couldn't be found
    if (!theme->valid()) {
        qWarning() << "Theme" << m_name << "was requested but could not be found.";
        qWarning() << "Falling back to default theme" << m_defaultName << "...";
        theme = m_model->get(m_defaultName);
    }
    currentChanged(m_current = theme);
}
