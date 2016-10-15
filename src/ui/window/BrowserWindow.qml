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
import Fluid.Controls 1.0
import SlimeEngine 0.2
import core 1.0
import ".."

FluidWindow {
    id: window

    property var root
    property WebProfile profile
    property TabsModel tabsModel: TabsModel {}
    property DownloadsModel downloadsModel

    property TabController tabController: TabController {
        id: tabController
        tabBar: tabBar
        tabContentView: tabContentView
        tabsModel: window.tabsModel
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

    Drawer {
        id: rightDrawer

        property int downloads: 0

        edge: Qt.RightEdge
        height: parent.height
        contentComponents: [
            Component {
                DrawerDownloadsContent {
                    downloadsModel: window.downloadsModel
                }
            }
        ]
        currentContentIndex: 0
    }

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
                id: expansionBarContainer
                Layout.fillWidth: true
                Layout.preferredHeight: expansionBar.showing && !dockCanvas.editActive ? 64 : 0
            }

            DockContainer {
                id: tabContentContainer
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        DockElement {
            // tabbar
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
            // toolbar
            editActive: dockCanvas.editActive
            container: toolbarContainer
            minimumHeight: 64
            minimumWidth: 256
            content: Toolbar {
                id: toolbar
                tabController: tabController
                tabsModel: tabController.tabsModel
                leftActions: [
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
                rightActions: [
                    Action {
                        enabled: !tabsModel.active.invalid
                        iconName: tabsModel.active.loading ? "navigation/close" : "navigation/refresh"
                        onTriggered: {
                            if (tabsModel.active.loading)
                                tabsModel.active.stop();
                            else
                                tabsModel.active.reload();
                        }
                    },
                    Action {
                        visible: downloadsModel.count > 0
                        iconName: "file/file_download"
                        onTriggered: {
                            rightDrawer.loadContent(rightDrawer.downloads);
                            rightDrawer.open();
                        }
                    },
                    Action {
                        id: toolbarOverflowAction
                        iconName: "navigation/more_vert"
                        onTriggered: {
                            toolbarActionsOverflowMenu.open();
                        }
                    }
                ]
            }
        }

        DockElement {
            // expansion bar
            editActive: dockCanvas.editActive
            container: expansionBarContainer
            minimumHeight: 64
            minimumWidth: 256

            content: ExpansionBar {
                id: expansionBar

                property bool showing: false
                property int searchPage: 0

                function show() { showing = true; }

                contentComponents: [
                    Component {
                        SearchPageBar {
                            // list of tabs that where searched
                            property var tabsList: []

                            searchEnabled: !tabController.tabsModel.empty
                            onSearchRequested: {
                                var activeTab = tabController.tabsModel.active;
                                activeTab.findText(text, backwards, false);
                                if (tabsList.indexOf(activeTab) === -1) {
                                    tabsList.push(activeTab);
                                }
                            }
                            onClosed: {
                                // Undo search:
                                // Not an optimal solution
                                // Better: remove tab from SearchPageBar.tabsList
                                // when it gets closed.
                                for (var tabIndex in tabsList) {
                                    var tab = tabsList[tabIndex];
                                    try {
                                        tab.findText("", false, false);
                                    }
                                    catch (e) {
                                        if (e instanceof TypeError) {
                                            // Ignore, tab could be closed in the meantime.
                                        }
                                        else {
                                            throw e;
                                        }
                                    }
                                }
                                tabsList = [];
                            }
                        }
                    }
                ]

                onClosed: {
                    showing = false;
                }
            }
        }

        DockElement {
            // content view
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

    Menu {
        id: toolbarActionsOverflowMenu
        onAboutToShow: {
            // Set menu popup position before showing
            var offset = Qt.point(-width/2, 0);
            var actionDelegate = toolbar.rightActionBar.itemAt(toolbarOverflowAction.index);
            var popupPosition = actionDelegate.mapToItem(parent, 0, 0);
            x = popupPosition.x + offset.x;
            y = popupPosition.y + offset.y;
        }

        MenuItem {
            text: "Search in page"
            onClicked: {
                expansionBar.loadContent(expansionBar.searchPage);
                expansionBar.show();
            }
        }

        MenuItem {
            text: "Downloads"
            onClicked: {
                rightDrawer.loadContent(rightDrawer.downloads);
                rightDrawer.open();
            }
        }

        Connections {
            enabled: toolbarActionsOverflowMenu.visible
            target: window
            onWidthChanged: {
                // Close the menu on window width change
                toolbarActionsOverflowMenu.close();
            }
        }
    }
}
