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

#include "extensiontheme.h"

ExtensionTheme::ExtensionTheme(QObject *parent)
    : ExtensionElement(parent)
{

}

ExtensionTheme *ExtensionTheme::clone(QObject *parent)
{
    ExtensionTheme* newTheme = new ExtensionTheme(parent);
    newTheme->setValid(valid());
    newTheme->setExtensionName(extensionName());
    newTheme->setName(name());
    newTheme->setTitle(title());
    newTheme->setSummary(summary());
    newTheme->setDescription(description());
    newTheme->setDark(dark());
    newTheme->setAdaptWebsiteTheme(adaptWebsiteTheme());
    newTheme->setAccent(accent());
    newTheme->setPrimary(primary());
    newTheme->setForeground(foreground());
    newTheme->setBackground(background());
    return newTheme;
}
