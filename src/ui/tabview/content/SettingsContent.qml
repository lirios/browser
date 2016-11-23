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
import Fluid.Core 1.0
import Fluid.Controls 1.0
import "../.."

TabContent {
    id: content

    Flickable {
        anchors {
            fill: parent
            margins: 16
        }

        ScrollBar.vertical: ScrollBar {}

        contentHeight: contentItem.childrenRect.height
        interactive: contentHeight > height

        Item {
            id: contentItem
            width: parent.width
            height: childrenRect.height

            Column {
                width: parent.width
                spacing: 16

                HeadlineLabel {
                    text: "Settings"
                }

                TitleLabel {
                    text: "Start"
                }

                RowLayout {
                    width: parent.width
                    spacing: 16

                    Label {
                        text: "Start url"
                    }

                    TextField {
                        Layout.minimumWidth: 256
                        text: Settings.startConfig.startUrl
                        onEditingFinished: {
                            if (Settings.startConfig.startUrl != text) {
                                Settings.startConfig.startUrl = text;
                                Settings.dirty = true;
                            }
                        }
                    }

                    Item { Layout.fillWidth: true } // Spacer
                }
            }
        }
    }

    Binding {
        target: content.tab
        property: "canReload"
        value: false
    }

    Binding {
        target: content.tab
        property: "iconUrl"
        value: Utils.getSourceForIconName("action/settings")
    }

    Binding {
        target: content.tab
        property: "canGoBack"
        value: false
    }

    Binding {
        target: content.tab
        property: "canGoForward"
        value: false
    }

    Binding {
        target: content.tab
        property: "loading"
        value: false
    }

    Binding {
        target: content.tab
        property: "loadProgress"
        value: 100
    }

    Binding {
        target: content.tab
        property: "url"
        value: "liri://settings"
    }

    Binding {
        target: content.tab
        property: "title"
        value: "Settings"
    }
}
