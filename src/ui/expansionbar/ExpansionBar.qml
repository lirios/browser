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

Item {
    id: expansionBar

    property list<Component> contentComponents
    property int currentContentIndex: -1
    property bool showing: false

    signal opened()
    signal closed()

    function loadContent(index) {
        currentContentIndex = index;
    }

    function open() {
        showing = true;
        showAnimation.start();
    }

    function close() {
        showing = false;
        closeAnimation.start();
    }

    clip: true

    Rectangle {
        id: contentContainer
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        height: 48

        Loader {
            id: contentLoader
            anchors {
                fill: parent
                leftMargin: 16
                rightMargin: 16
            }
            sourceComponent: currentContentIndex >= 0 ? contentComponents[currentContentIndex] : null
        }
    }

    NumberAnimation {
        id: showAnimation
        target: expansionBar
        property: "implicitHeight"
        duration: 200
        easing.type: Easing.InOutQuad
        to: 56
        onStopped: opened()
    }

    NumberAnimation {
        id: closeAnimation
        target: expansionBar
        property: "implicitHeight"
        duration: 200
        easing.type: Easing.InOutQuad
        to: 0
        onStopped: closed()
    }

    Connections {
        enabled: currentContentIndex >= 0 && contentLoader.status === Loader.Ready
        target: contentLoader.item
        onClosed: {
            expansionBar.close();
            currentContentIndex = -1;
        }
    }
}
