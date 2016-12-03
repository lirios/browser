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
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import Fluid.Controls 1.0
import ".."

Rectangle {
    property string title
    property url iconSource
    property bool active: false
    property color backgroundColor: "white"
    property color foregroundColor: "#212121"

    signal closeRequested()

    implicitWidth: 200
    color: backgroundColor

    RowLayout {
        anchors {
            fill: parent
            leftMargin: Units.smallSpacing
        }

        spacing: Units.smallSpacing

        Image {
            Layout.preferredHeight: Units.iconSizes.small
            Layout.preferredWidth: Units.iconSizes.small
            source: iconSource
            clip: true
            sourceSize.width: 16
            sourceSize.height: 16
        }

        Label {
            Layout.fillWidth: true
            text: title
            color: active ? foregroundColor : ColorUtils.shadeColor(foregroundColor, 0.5)
            elide: Text.ElideRight

            Behavior on color {
                ColorAnimation {
                    duration: Units.mediumDuration
                    easing.type: Easing.InOutQuad
                }
            }
        }

        IconButton {
            iconName: "navigation/close"
            onClicked: closeRequested()
            iconColor: active ? foregroundColor : ColorUtils.shadeColor(foregroundColor, 0.5)
            iconSize: 18
            implicitHeight: 24
            implicitWidth: 24

            Behavior on iconColor {
                ColorAnimation {
                    duration: Units.mediumDuration
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
