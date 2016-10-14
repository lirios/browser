/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb <tim.sueberkrueb@web.de>
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
import dperini.regexweburl 1.0

QtObject {
    function isWebUrl(url) {
        return url.match(RegexWebUrl.re_weburl) !== null;
    }

    function validUrl(url) {
        // Valid web url
        var httpedUrl = (url.indexOf("http://") === 0 || url.indexOf("https://") === 0) ? url : "http://%1".arg(url);
        if (isWebUrl(httpedUrl)) {
            return httpedUrl;
        }
        // Search term
        else {
            return "https://www.google.com/search?q=%1".arg(url);
        }
    }
}
