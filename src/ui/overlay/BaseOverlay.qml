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
import Fluid.Material 1.0

Rectangle {
    id: baseOverlay

    property bool showing: false

    signal opened()
    signal closed()

    function open() {
        showing = true;
        openAnimation.start();
    }

    function close() {
        showing = false;
        closeAnimation.start();
    }

    visible: height > 0

    layer.enabled: true
    layer.effect: ElevationEffect { elevation: 2 }

    implicitWidth: 300
    height: 0

    NumberAnimation {
        id: openAnimation
        target: baseOverlay
        property: "height"
        from: 0
        to: 48
        duration: 200
        easing.type: Easing.InOutQuad
        onStopped: opened()
    }

    NumberAnimation {
        id: closeAnimation
        target: baseOverlay
        property: "height"
        from: 48
        to: 0
        duration: 200
        easing.type: Easing.InOutQuad
        onStopped: closed()
    }
}
