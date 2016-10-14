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

    implicitHeight: 64
    implicitWidth: 256

    RowLayout {
        anchors {
            fill: parent
            rightMargin: 16
        }

        ActionBar {
            actions: [
                Action {
                    iconName: "navigation/arrow_back"
                    enabled: !tabsModel.active.invalid && tabsModel.active.canGoBack
                    onTriggered: tabsModel.active.goBack()
                },
                Action {
                    iconName: "navigation/arrow_forward"
                    enabled: !tabsModel.active.invalid && tabsModel.active.canGoForward
                    onTriggered: tabsModel.active.goForward()
                }
            ]
        }

        Omnibox {
            Layout.preferredHeight: 48
            Layout.fillWidth: true
            tabsModel: toolbar.tabsModel
            tabController: toolbar.tabController
        }

        ActionBar {
            actions: [
                Action {
                    enabled: !tabsModel.active.invalid
                    iconName: tabsModel.active.loading ? "navigation/close" : "navigation/refresh"
                    onTriggered: {
                        if (tabsModel.active.loading)
                            tabsModel.active.stop();
                        else
                            tabsModel.active.reload();
                    }
                }
            ]
        }
    }
}
