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

Rectangle {
    id: expansionBar

    property list<Component> contentComponents
    property int currentContentIndex: -1

    signal closed()

    function loadContent(index) {
        currentContentIndex = index;
    }

    Loader {
        id: contentLoader
        anchors {
            fill: parent
            margins: 16
            topMargin: 8
            bottomMargin: 8
        }
        sourceComponent: currentContentIndex >= 0 ? contentComponents[currentContentIndex] : null
    }

    Connections {
        enabled: currentContentIndex >= 0 && contentLoader.status === Loader.Ready
        target: contentLoader.item
        onClosed: {
            expansionBar.closed();
            currentContentIndex = -1;
        }
    }
}
