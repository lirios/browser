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

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import Fluid.Controls 1.0

BaseOverlay {
    id: searchOverlay

    property bool searchEnabled: true

    signal searchRequested(string text, bool backwards)

    onOpened: {
        searchField.forceActiveFocus();
    }

    implicitWidth: childrenRect.width

    RowLayout {
        id: rowLayout

        anchors {
            top: parent.top
            bottom: parent.bottom
        }

        Item { implicitWidth: 16 }  // Spacer

        TextField {
            id: searchField
            selectByMouse: true
            enabled: searchEnabled
            placeholderText: qsTr("Find in page")
            onAccepted: {
                searchRequested(searchField.text, false);
            }
        }

        IconButton {
            enabled: searchEnabled
            implicitHeight: 40
            implicitWidth: 40
            iconName: "hardware/keyboard_arrow_up"
            onClicked: {
                searchRequested(searchField.text, true);
            }
        }

        IconButton {
            enabled: searchEnabled
            implicitHeight: 40
            implicitWidth: 40
            iconName: "hardware/keyboard_arrow_down"
            onClicked: {
                searchRequested(searchField.text, false);
            }
        }

        IconButton {
            implicitHeight: 40
            implicitWidth: 40
            iconName: "navigation/close"
            onClicked: {
                searchOverlay.close();
            }
        }

        Item { implicitWidth: 8 }  // Spacer
    }
}
