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
import core 1.0

Rectangle {
    id: tabBar
    height: tabHeight
    color: "white"

    property var tabController
    property TabsModel tabsModel
    property int tabWidth: 200
    property int tabHeight: 38
    property bool dragActive: false
    property color indicatorColor: "#2196f3"
    property url newTabUrl

    // Using flick might not be the best solution
    // for programmatically scrolling through the tabs
    // Animating contentX would be another solution.
    property int programmaticFlickVelocity: 850

    property list<Action> leftActions: [
        Action {
            visible: tabsListView.contentWidth > tabsListView.width
            enabled: tabsListView.contentX > 0
            iconName: "navigation/chevron_left"
            onTriggered: {
                tabsListView.flick(programmaticFlickVelocity, 0);
            }
        }
    ]
    property list<Action> rightActions: [
        Action {
            visible: tabsListView.contentWidth > tabsListView.width
            enabled: tabsListView.contentX + tabsListView.width < tabsListView.contentWidth
            iconName: "navigation/chevron_right"
            onTriggered: {
                tabsListView.flick(-programmaticFlickVelocity, 0);
            }
        },
        Action {
            iconName: "content/add"
            onTriggered: {
                tabController.openUrl(newTabUrl)
            }
        }
    ]

    signal tabCloseRequested(int uid)

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        TabBarActionBar {
            Layout.preferredWidth: childrenRect.width
            Layout.fillHeight: true
            actions: leftActions
        }

        ListView {
            id: tabsListView
            Layout.fillHeight: true
            Layout.fillWidth: true

            orientation: ListView.Horizontal

            model: tabsModel
            clip: true

            delegate: MouseArea {
                id: delegateMouseArea

                property bool dragActive: drag.active
                property bool active: tabsModel.active.uid == uid
                property int visualIndex: index

                width: tabBar.tabWidth
                height: tabBar.height
                drag.target: content
                drag.axis: Drag.XAxis

                acceptedButtons: Qt.AllButtons

                onPressed: {
                    // Tab pressed -> set tab active
                    tabsModel.setActive(uid);
                }

                onReleased: {
                    // Drag released (e.g. drag finished) -> set tab active
                    tabsModel.setActive(uid);
                }

                onClicked: {
                    // Remove tab on middle press
                    if (mouse.button === Qt.MiddleButton) {
                        tabCloseRequested(uid);
                    }
                }

                onDragActiveChanged: {
                    tabBar.dragActive = dragActive;
                    if (dragActive) {
                        // Drag started -> set tab active
                        tabsModel.setActive(uid);
                    }
                }

                Item {
                    id: content
                    anchors {
                        top: delegateMouseArea.top
                        left: delegateMouseArea.left
                    }

                    width: delegateMouseArea.width
                    height: delegateMouseArea.height

                    Drag.active: dragActive
                    Drag.source: delegateMouseArea
                    Drag.hotSpot.x: width/2
                    Drag.hotSpot.y: height/2

                    states: [
                        State {
                            when: content.Drag.active
                            ParentChange {
                                target: content
                                parent: tabBar
                            }
                            AnchorChanges {
                                target: content
                                anchors.top: undefined
                                anchors.left: undefined
                            }
                        }
                    ]

                    TabDelegate {
                        anchors.fill: parent
                        active: delegateMouseArea.active
                        title: tabsModel.byUID(uid).title
                        iconSource: tabsModel.byUID(uid).iconUrl
                        onCloseRequested: {
                            tabCloseRequested(uid);
                        }
                    }
                }

                DropArea {
                    anchors.fill: parent
                    onEntered: {
                        tabsModel.move(drag.source.visualIndex, delegateMouseArea.visualIndex);
                    }
                }
            }

            add: Transition {
                NumberAnimation {
                    property: "y"
                    from: tabBar.height
                    to: 0
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            displaced: Transition {
                NumberAnimation {
                    property: "x"
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }

            remove: Transition {
                NumberAnimation {
                    property: "y"
                    to: tabBar.height
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }

            TabIndicator {
                parent: tabsListView.contentItem
                z: 2
                anchors {
                    bottom: parent.bottom
                }
                visible: tabsModel.activeIndex > -1 && !tabBar.dragActive
                x: tabsModel.activeIndex * tabWidth
                width: tabBar.tabWidth
                color: indicatorColor

                Behavior on x {
                    NumberAnimation {
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }

                Behavior on anchors.bottomMargin {
                    NumberAnimation {
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }

        TabBarActionBar {
            Layout.preferredWidth: childrenRect.width
            Layout.fillHeight: true
            actions: rightActions
        }
    }

    Connections {
        enabled: !tabsModel.active.invalid
        target: tabsModel
        onActiveChanged: {
            // Ensure the active tab is fully visible
            if (!tab.invalid) {
                if (tabsModel.activeIndex !== -1) {
                    tabsListView.positionViewAtIndex(tabsModel.activeIndex, ListView.Contain);
                }
            }
        }
    }

    MouseArea {
        anchors.fill: rowLayout
        onWheel: {
            if (wheel.angleDelta.y > 0)
                tabsListView.flick(programmaticFlickVelocity, 0);
            else
                tabsListView.flick(-programmaticFlickVelocity, 0);
        }
        onPressed: mouse.accepted = false;
    }
}
