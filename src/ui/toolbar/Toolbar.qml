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
import Fluid.Controls 1.0
import core 1.0
import ".."

Rectangle {
    id: toolbar
    property TabController tabController
    property TabsModel tabsModel

    property list<Action> leftActions
    property list<Action> rightActions

    readonly property ActionBar leftActionBar: leftActionBar
    readonly property ActionBar rightActionBar: rightActionBar

    implicitHeight: 56
    implicitWidth: 256

    RowLayout {
        anchors {
            fill: parent
            rightMargin: 16
        }

        Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter

        ActionBar {
            id: leftActionBar
            actions: leftActions
        }

        Omnibox {
            Layout.fillWidth: true
            tabsModel: toolbar.tabsModel
            tabController: toolbar.tabController
        }

        ActionBar {
            id: rightActionBar
            actions: rightActions
        }
    }
}
