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

#include "extensionsearchengineparameter.h"

ExtensionSearchEngineParameter::ExtensionSearchEngineParameter(QObject *parent)
    : QObject(parent)
{

}

ExtensionSearchEngineParameter *ExtensionSearchEngineParameter::clone(QObject *parent)
{
    ExtensionSearchEngineParameter* param = new ExtensionSearchEngineParameter(parent);
    param->setName(m_name);
    param->setValue(m_value);
    param->setContext(m_context);
    return param;
}
