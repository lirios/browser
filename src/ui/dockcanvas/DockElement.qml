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
    id: dockElement

    property var container: undefined
    property Item content: Item {}
    property bool animationsEnabled: true
    property bool editActive: false

    property int minimumWidth
    property int minimumHeight

    readonly property bool dragging: state == "dragging"
    readonly property bool docked: state == "docked"
    readonly property bool idle: state == "idle"

    Drag.active: dragging

    height: minimumHeight
    width: minimumWidth

    state: "idle"

    states: [
        State {
            name: "idle"
            when: !dragArea.drag.active && typeof(dockElement.container) == "undefined"
        },
        State {
            name: "dragging"
            when: dragArea.drag.active
        },
        State {
            name: "docked"
            when: !dragArea.drag.active && typeof(dockElement.container) != "undefined"
            ParentChange {
                target: dockElement
                parent: dockElement.container
                x: 0
                y: 0
            }
            PropertyChanges {
                target: dockElement; height: dockElement.container.height; width: dockElement.container.width
            }
        }
    ]

    Item {
        id: contentContainer
        anchors.fill: parent
        children: content
        onChildrenChanged: {
            // Automatically anchor content
            children[0].anchors.fill = contentContainer;
        }
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        enabled: editActive
        drag.target: parent
        onReleased: {
            parent.Drag.drop();
        }
    }

    NumberAnimation {
        duration: 200; easing.type: Easing.InOutQuad
    }

    Behavior on width {
        enabled: animationsEnabled
        NumberAnimation {
            duration: 200; easing.type: Easing.InOutQuad
        }
    }

    Behavior on height {
        enabled: animationsEnabled
        NumberAnimation {
            duration: 200; easing.type: Easing.InOutQuad
        }
    }

    Behavior on x {
        enabled: docked && animationsEnabled
        NumberAnimation {
            duration: 200; easing.type: Easing.InOutQuad
        }
    }

    Behavior on y {
        enabled: docked && animationsEnabled
        NumberAnimation {
            duration: 200; easing.type: Easing.InOutQuad
        }
    }
}
