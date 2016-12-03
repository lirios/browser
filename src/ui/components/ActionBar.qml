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
import Fluid.Controls 1.0
import ".."

Rectangle {
    id: actionBar
    property list<Action> actions
    property color foregroundColor: "#212121"
    property bool animationsEnabled: true
    property int iconSize: Units.iconSizes.smallMedium

    function itemAt(index) {
        return actionRepeater.itemAt(index);
    }

    implicitHeight: childrenRect.height
    implicitWidth: childrenRect.width

    Row {
        Repeater {
            id: actionRepeater
            model: actions
            delegate: IconButton {
                id: iconButton
                visible: model.visible
                implicitHeight: 40
                implicitWidth: 40
                iconSource: model.iconSource
                iconSize: actionBar.iconSize
                enabled: model.enabled
                iconColor: enabled ? foregroundColor : ColorUtils.shadeColor(foregroundColor, 0.5)
                onClicked: actions[index].triggered(iconButton)

                Behavior on iconColor {
                    enabled: animationsEnabled
                    ColorAnimation {
                        duration: Units.mediumDuration
                    }
                }
            }
        }
    }
}
