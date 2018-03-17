/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <dev@timsueberkrueb.io>
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
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Fluid.Core 1.0
import dperini.regexweburl 1.0
import core 1.0
import QtQml.StateMachine 1.0 as DSM
import ".."

Item {
    id: omnibox
    property TabController tabController
    property TabsModel tabsModel
    property string searchUrl
    property alias editingUrl: editing.active
    property color defaultAccentColor
    property bool editingCompleted: idle.active

    signal emptyTabActivated

    function focusUrlField() {
        showUrlField.forceActiveFocus();
        textField.selectAll();
    }

    function checkForEmptyTab() {
        if (tabsModel.active.url.toString().length === 0) {
            emptyTabActivated();
        }
    }

    implicitHeight: 40
    implicitWidth: 256

    Rectangle {
        id: container
        anchors.fill: parent

        radius: 2
        color: Color.lightDark(Material.background, Material.color(Material.Grey, Material.Shade200), "white")

        RowLayout {
            anchors {
                fill: parent
                leftMargin: Units.smallSpacing
                rightMargin: Units.smallSpacing
            }

            TextField {
                id: showUrlField

                text: editingCompleted ? tabsModel.active.url.toString() : textField.text

                Layout.fillHeight: true
                Layout.fillWidth: true

                Material.foreground: Material.color(Material.Grey, Material.Shade900)
                Material.accent: Color.lightDark(Material.background, defaultAccentColor, Material.background)

                bottomPadding: Units.smallSpacing

                background: Item {}
                font.pixelSize: 14
                readOnly: true

                TextField {
                    id: textField

                    anchors.fill: parent
                    visible: editingUrl

                    selectionColor: Material.accent
                    selectByMouse: true
                    bottomPadding: Units.smallSpacing
                    background: Rectangle { color: container.color }
                    font.pixelSize: 14

                    onAccepted: {
                        var url = UrlUtils.validUrl(text, searchUrl);
                        if (!tabsModel.active.valid) {
                            tabController.openUrl(url);
                        }
                        else {
                            var reload = (tabsModel.active.url === url);
                            tabsModel.active.url = url;
                            if (reload)
                                tabsModel.active.reload();
                        }
                        textField.focus = false;
                    }
                }
            }
        }

        Rectangle {
            visible: tabsModel.active.valid && width < parent.width
            anchors {
                bottom: parent.bottom
                left: parent.left
            }

            height: 2
            width: parent.width * (tabsModel.active.loadProgress / 100)
            color: Color.lightDark(Material.background, defaultAccentColor, Material.background)

            Behavior on width {
                enabled: tabsModel.active.loading
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Connections {
            target: tabsModel
            onActiveChanged: {
                textField.text = tabsModel.active.url.toString();
                checkForEmptyTab();
            }
        }
        Connections {
            target: tabsModel.active
            onUrlChanged: {
                textField.text = tabsModel.active.url;
            }
        }

        Binding {
            when: tabsModel.empty
            target: showUrlField
            property: "text"
            value: ""
        }

        Binding {
            when: tabsModel.empty
            target: textField
            property: "text"
            value: ""
        }

        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }

    DSM.StateMachine {
        id: stateMachine
        initialState: idle
        running: true

        DSM.State {
            id: idle

            onEntered: {
                checkForEmptyTab();
                textField.text = showUrlField.text;
            }
            DSM.SignalTransition {
                signal: showUrlField.activeFocusChanged
                guard: showUrlField.activeFocus === true
                targetState: editing
            }
            DSM.SignalTransition {
                signal: emptyTabActivated
                targetState: editing
            }
        }
        DSM.State {
            id: editing

            onEntered: {
                textField.forceActiveFocus();
            }

            DSM.SignalTransition {
                signal: tabsModel.activeChanged
                targetState: idle
            }
            DSM.SignalTransition {
                signal: textField.activeFocusChanged
                guard: textField.activeFocus === false
                targetState: canceledEditing
            }
        }
        DSM.State {
            id: canceledEditing
            onEntered: {
                checkForEmptyTab();
            }
            DSM.SignalTransition {
                signal: emptyTabActivated
                targetState: editing
            }
            DSM.SignalTransition {
                signal: showUrlField.activeFocusChanged
                guard: showUrlField.activeFocus === true
                targetState: editing
            }
        }
    }
}
