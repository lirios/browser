/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb (https://github.com/tim-sueberkrueb)
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
 */

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import Fluid.Controls 1.0
import SlimeEngine 0.2
import Liri.Browser.Components 1.0
import Liri.Browser.Core 1.0

FluidWindow {
    id: window

    property var root
    property WebProfile profile

    property TabController tabController: TabController {
        id: tabController
        tabBar: tabBar
        tabContentView: tabContentView
        tabsModel: TabsModel {}
        profile: window.profile
        webengine: root.webengine
        webviewComponent: Component {
            WebPage {}
        }

        onNewWindowRequested: {
            root.openWindowRequest(request);
        }
    }

    function openRequest(request) {
        request.destination = NewViewRequest.NewViewInTab;
        tabController.openNewViewRequest(request);
    }

    width: 1024
    height: 640

    DockCanvas {
        id: dockCanvas
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: parent.layoutSpacing

            DockContainer {
                id: tabBarContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 48
            }

            DockContainer {
                id: toolbarContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 64
            }

            DockContainer {
                id: tabContentContainer
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        DockElement {
            editActive: dockCanvas.editActive
            container: tabBarContainer
            minimumHeight: content.tabHeight
            minimumWidth: content.tabWidth
            content: TabBar {
                id: tabBar
                tabController: tabController
                tabsModel: tabController.tabsModel
            }
        }

        DockElement {
            editActive: dockCanvas.editActive
            container: toolbarContainer
            minimumHeight: 64
            minimumWidth: 256
            content: Toolbar {
                tabController: tabController
                tabsModel: tabController.tabsModel
            }
        }

        DockElement {
            editActive: dockCanvas.editActive
            container: tabContentContainer
            minimumHeight: 200
            minimumWidth: 300
            content: TabContentView {
                id: tabContentView
                tabsModel: tabController.tabsModel
            }
        }
    }
}
