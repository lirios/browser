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
 import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Fluid.Material 1.0
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

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            id: toolbarContainer

            z: 5
            Layout.fillWidth: true
            Material.primary: "white"
            Material.elevation: 0

            ColumnLayout {
                id: headColumn
                anchors.fill: parent
                spacing: 0

                TabBar {
                    id: tabBar

                    Layout.fillWidth: true

                    tabController: tabController
                    tabsModel: tabController.tabsModel
                }

                Toolbar {
                    id: toolbar

                    Layout.fillWidth: true

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
        }

        Item {
            // Body
            Layout.fillWidth: true
            Layout.fillHeight: true

            TabContentView {
                id: tabContentView
                anchors.fill: parent
                tabsModel: tabController.tabsModel
            }

            SearchOverlay {
                id: searchOverlay

                anchors {
                    top: parent.top
                    right: parent.right
                    margins: 4
                }

                // list of tabs that where searched
                property var tabsList: []

                z: 5
                searchEnabled: !tabController.tabsModel.empty
                onSearchRequested: {
                    var activeTab = tabController.tabsModel.active;
                    activeTab.findText(text, backwards, false);
                    if (tabsList.indexOf(activeTab) === -1) {
                        tabsList.push(activeTab);
                    }
                }
                onClosed: {
                    // Undo search in all affected tabs
                    for (var tabIndex in tabsList) {
                        var tab = tabsList[tabIndex];
                        tab.findText("", false, false);
                    }
                    tabsList = [];
                }

                Connections {
                    target: tabController.tabsModel
                    onBeforeTabRemoved: {
                        // Remove tab from list of searched tabs one close
                        for (var tabIndex in searchOverlay.tabsList) {
                            var t = searchOverlay.tabsList[tabIndex];
                            if (t == tab) {
                                searchOverlay.tabsList.splice(tabIndex, 1);
                            }
                        }
                    }
                }
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
            text: "Find in page"
            onClicked: {
                searchOverlay.open();
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
            target: window
            enabled: toolbarActionsOverflowMenu.visible
            onWidthChanged: {
                // Close the menu on window width change
                if (toolbarActionsOverflowMenu.visible)
                    toolbarActionsOverflowMenu.close();
            }
        }
    }
}
