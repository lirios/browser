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
    property var webUrlRegex: RegexWebUrl.re_weburl

    function isChromeUrl(url) {
        return url.indexOf("chrome://") == 0;
    }

    function isViewSourceUrl(url) {
        return url.indexOf("view-source:") == 0;
    }

    function isWebUrl(url) {
        return url.match(webUrlRegex) !== null;
    }

    function isLiriUrl(url) {
        return url.toString().indexOf("liri://") === 0;
    }

    function validUrl(url, searchEngine, currentTheme) {
        // Valid web url
        var isHttped = (url.indexOf("http://") === 0 || url.indexOf("https://") === 0);
        var httpedUrl = isHttped ? url : "http://%1".arg(url);
        if (isHttped || isWebUrl(httpedUrl)) {
            return httpedUrl;
        }
        // Liri url
        else if (isLiriUrl(url)) {
            return url;
        }
        // Chrome url
        else if (isChromeUrl(url)) {
            return url;
        }
        // View source url
        else if (isViewSourceUrl(url)) {
            return url;
        }
        // Search term
        else {
            return Search.searchUrl(url, searchEngine, currentTheme);
        }
    }
}
