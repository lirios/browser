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

import QtQuick 2.7
import core 1.0

Item {
    id: content

    property var tab
    property var actionManager

    property url iconUrl
    property bool adaptIconColor
    property url url
    property bool canGoBack
    property bool canGoForward
    property bool canReload
    property bool loading
    property int loadProgress
    property bool hasThemeColor
    property color themeColor
    property string title

    Binding {
        target: content.tab
        property: "title"
        value: title
    }

    Binding {
        target: content.tab
        property: "iconUrl"
        value: iconUrl
    }

    Binding {
        target: content.tab
        property: "adaptIconColor"
        value: adaptIconColor
    }

    Binding {
        target: content.tab
        property: "url"
        value: url
    }

    Binding {
        target: content.tab
        property: "canGoBack"
        value: canGoBack
    }

    Binding {
        target: content.tab
        property: "canGoForward"
        value: canGoForward
    }

    Binding {
        target: content.tab
        property: "loading"
        value: loading
    }

    Binding {
        target: content.tab
        property: "loadProgress"
        value: loadProgress
    }

    Binding {
        target: content.tab
        property: "hasThemeColor"
        value: hasThemeColor
    }

    Binding {
        target: content.tab
        property: "themeColor"
        value: themeColor
    }

    Binding {
        target: content.tab
        property: "canReload"
        value: canReload
    }
}
