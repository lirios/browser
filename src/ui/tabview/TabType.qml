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

pragma Singleton
import QtQuick 2.7
import "../utils"

QtObject {
    property int unknown: -1
    property int webview: 0
    property int settings: 1
    property int extensions: 2
    property int about: 3

    function fromUrl(url) {
        if (UrlUtils.isLiriUrl(url)) {
            if (url == "liri://settings")
                return settings;
            else if (url == "liri://extensions")
                return extensions;
            else if (url == "liri://about")
                return about;
        } else {
            return webview;
        }
        return unknown;
    }
}
