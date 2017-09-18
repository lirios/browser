/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb <dev@timsueberkrueb.io>
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

import QtQuick 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Fluid.Effects 1.0 as FluidEffects

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
    layer.effect: FluidEffects.Elevation { elevation: 2 }

    implicitWidth: 300
    height: 0

    color: Material.background

    NumberAnimation {
        id: openAnimation
        target: baseOverlay
        property: "height"
        from: 0
        to: 48
        duration: Units.mediumDuration
        easing.type: Easing.InOutQuad
        onStopped: opened()
    }

    NumberAnimation {
        id: closeAnimation
        target: baseOverlay
        property: "height"
        from: 48
        to: 0
        duration: Units.mediumDuration
        easing.type: Easing.InOutQuad
        onStopped: closed()
    }
}
