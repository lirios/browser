/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <tim.sueberkrueb@web.de>
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
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Fluid.Core 1.0
import "../.."

TabContent {
    id: content

    canReload: false
    canGoBack: false
    canGoForward: false
    loading: false
    loadProgress: 100
    iconUrl: Utils.getSourceForIconName("action/extension")
    adaptIconColor: true
    title: "Extensions"
    url: "liri://extensions"
    hasThemeColor: false

    RowLayout {
        anchors {
            fill: parent
            margins: Units.smallSpacing * 2
        }

        Item {
            Layout.preferredWidth: 256
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent

                HeadlineLabel {
                    text: "Extensions"
                }

                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    property string selectedName: ""

                    clip: true
                    model: SortFilterProxyModel {
                        sourceModel: Extensions.extensionsModel
                        sortRoleName: "title"
                    }

                    delegate: ListItem {
                        property bool isSelected: listView.selectedName == name

                        iconName: isSelected ? "navigation/check" : "action/extension"
                        text: title
                        subText: summary

                        onClicked: {
                            listView.selectedName = name;
                        }
                    }
                }
            }
        }

        Item {
            id: propertiesPanel

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: Units.smallSpacing * 2

            property bool selected: listView.selectedName !== ""
            property var extension: selected ? Extensions.extensionsModel.get(listView.selectedName) : null

            visible: selected

            Flickable {
                anchors {
                    fill: parent
                    margins: Units.smallSpacing
                }

                contentHeight: propertiesColumn.childrenRect.height
                interactive: contentHeight > height

                ScrollBar.vertical: ScrollBar {}

                Column {
                    id: propertiesColumn

                    width: parent.width

                    spacing: Units.smallSpacing

                    RowLayout {
                        width: parent.width

                        TitleLabel {
                            text: propertiesPanel.extension ? propertiesPanel.extension.title : ""
                        }

                        Item { Layout.fillWidth: true }     // Spacer

                        BodyLabel {
                            text: "By %1".arg(propertiesPanel.extension ? propertiesPanel.extension.author : "")
                            color: Material.secondaryTextColor
                        }
                    }

                    SubheadingLabel {
                        text: propertiesPanel.extension ? propertiesPanel.extension.summary : ""
                    }

                    TitleLabel {
                        text: "Description"
                    }

                    BodyLabel {
                        text: propertiesPanel.extension ? propertiesPanel.extension.description : ""
                    }

                    TitleLabel {
                        text: "Themes"
                    }

                    ListView {
                        width: parent.width
                        height: 256

                        clip: true

                        model: SortFilterProxyModel {
                            sourceModel: Extensions.themesModel
                            filterRoleName: "extensionName"
                            sortRoleName: "title"
                            filterValue: propertiesPanel.extension ? propertiesPanel.extension.name : ""
                        }

                        delegate: ListItem {
                            width: parent.width
                            text: title
                            subText: summary
                            iconName: "image/color_lens"
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 1
                    }
                }
            }
        }
    }
}
