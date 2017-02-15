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
import Fluid.Controls 1.0
import Fluid.Core 1.0

Item {
    property string title: "Select theme"
    property string selectedName: ""

    signal selected(string name)

    implicitHeight: column.childrenRect.height

    Column {
        id: column

        width: parent.width

        RowLayout {
            spacing: Units.largeSpacing
            width: parent.width

            SubheadingLabel {
                text: title
            }

            Item { Layout.fillWidth: true } // Spacer

            TextField {
                id: searchField
                selectByMouse: true
                placeholderText: "Search"
            }
        }

        ListView {
            id: listView

            width: parent.width
            height: Math.min(256, childrenRect.height)
            clip: true

            ScrollBar.vertical: ScrollBar {}

            model: SortFilterProxyModel {
                sourceModel: Extensions.themesModel
                sortRoleName: "title"
                filterRoleName: "title"
                filterPattern: "*%1*".arg(searchField.text)
                filterPatternSyntax: SortFilterProxyModel.Wildcard
                filterCaseSensitivity: Qt.CaseInsensitive
            }

            delegate: ListItem {
                property bool isSelected: name === selectedName

                width: listView.width
                text: title
                subText: summary
                iconName: isSelected ? "navigation/check" : "action/search"
                onClicked: {
                    selected(selectedName = name);
                }
            }
        }
    }
}
