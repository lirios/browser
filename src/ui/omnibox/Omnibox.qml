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
import ".."

Item {
    id: omnibox
    property TabController tabController
    property TabsModel tabsModel
    property string searchUrl
    property alias editingUrl: showUrlField.editActive
    property color defaultAccentColor

    function focusUrlField() {
        showUrlField.forceActiveFocus();
        textField.selectAll();
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

                property bool editActive: false

                Layout.fillHeight: true
                Layout.fillWidth: true

                Material.foreground: Material.color(Material.Grey, Material.Shade900)
                Material.accent: Color.lightDark(Material.background, defaultAccentColor, Material.background)

                bottomPadding: Units.smallSpacing

                background: Item {}
                font.pixelSize: 14
                readOnly: true

                onActiveFocusChanged: {
                    if (activeFocus) {
                        editActive = true;
                        textField.forceActiveFocus();
                        textField.selectAll();
                    }
                }

                TextField {
                    id: textField

                    anchors.fill: parent
                    visible: showUrlField.editActive

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
                            var reload = (tabsModel.active.url == url);
                            tabsModel.active.url = url;
                            if (reload)
                                tabsModel.active.reload();
                        }
                        showUrlField.editActive = false;
                    }

                    onActiveFocusChanged: {
                        if (!activeFocus) {
                            showUrlField.editActive = false;
                        }
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

        Binding {
            when: tabsModel.active.valid
            target: showUrlField
            property: "text"
            value: tabsModel.active.url
        }

        Binding {
            when: !showUrlField.editActive
            target: textField
            property: "text"
            value: tabsModel.active.url
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

        Connections {
            enabled: tabsModel.active.valid
            target: tabsModel
            onActiveChanged: {
                showUrlField.editActive = false;
            }
        }

        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }
}
