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
import Fluid.Material 1.0
import dperini.regexweburl 1.0
import core 1.0
import ".."

Item {
    id: omnibox
    property TabController tabController
    property TabsModel tabsModel

    implicitHeight: 40
    implicitWidth: 256

    Rectangle {
        anchors.fill: parent

        layer.enabled: true
        layer.effect: ElevationEffect {
            elevation: 0
            fullWidth: true
            fullHeight: true
        }

        radius: 2

        RowLayout {
            anchors {
                fill: parent
                leftMargin: 8
                rightMargin: 8
            }

            TextField {
                id: showUrlField

                property bool editActive: false

                Layout.fillHeight: true
                Layout.fillWidth: true

                bottomPadding: 8
                background: Item {}
                font.pixelSize: 14
                readOnly: true

                onActiveFocusChanged: {
                    if (activeFocus) {
                        editActive = true;
                        textField.forceActiveFocus();
                        textField.select(showUrlField.selectionStart, showUrlField.selectionEnd);
                    }
                }

                TextField {
                    id: textField

                    anchors.fill: parent
                    visible: showUrlField.editActive

                    bottomPadding: 8
                    background: Rectangle {}
                    font.pixelSize: 14

                    onAccepted: {
                        var url = UrlUtils.validUrl(text);
                        if (tabsModel.active.invalid) {
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

                    onEditingFinished: {
                        showUrlField.editActive = false;
                    }
                }
            }
        }

        Rectangle {
            visible: !tabsModel.active.invalid && width < parent.width
            anchors {
                bottom: parent.bottom
                left: parent.left
            }

            height: 2
            width: parent.width * (tabsModel.active.loadProgress / 100)
            color: "red"

            Behavior on width {
                enabled: tabsModel.active.loading
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Binding {
            when: !tabsModel.active.invalid
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
            enabled: !tabsModel.active.invalid
            target: tabsModel
            onActiveChanged: {
                showUrlField.editActive = false;
            }
        }
    }
}
