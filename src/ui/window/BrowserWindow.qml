/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <dev@timsueberkrueb.io>
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
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.0
import QtWebEngine 1.2
import Fluid.Core 1.0
import Fluid.Controls 1.0
import core 1.0
import ".."

ApplicationWindow {
    id: window

    property var root
    property WebEngineProfile profile
    property bool incognito: profile.offTheRecord
    property url startUrl: {
        if (incognito)
            return Settings.startConfig.incognitoStartUrl;
        else if (darkThemeActive)
            return Settings.startConfig.darkStartUrl;
        else
            return Settings.startConfig.primaryStartUrl;
    }
    property string searchUrl: Settings.searchConfig.searchUrl
    property bool openStartUrl: true
    property bool themeColorEnabled: Settings.themeConfig.themeColorEnabled
    property bool darkThemeActive: {
        if (Settings.themeConfig.darkThemeEnabled) {
            // always on if startTime == endTime (e.g. 00:00 == 00:00)
            var alwaysOn = (timeString(Settings.themeConfig.darkThemeStartTime)
                            === timeString(Settings.themeConfig.darkThemeEndTime));
            // dark theme active if either always on or current time in
            // active time span configured in the settings
            return alwaysOn || DarkThemeTimer.isActiveTime;
        }
        return false;
    }

    property TabsModel tabsModel: TabsModel {}
    property DownloadsModel downloadsModel

    property bool isWindowed: window.visibility === Window.Windowed
    property bool isMaximized: window.visibility === Window.Maximized
    property bool isFullScreen: window.visibility === Window.FullScreen
    property bool fullScreenUserChoice

    property TabController tabController: TabController {
        id: tabController
        tabBar: tabBar
        tabContentView: tabContentView
        tabsModel: window.tabsModel
        profile: window.profile
        onNewWindowRequested: {
            root.openWindowRequest(request);
        }
        onFullScreenRequested: {
            setFullScreen(fullScreenUserChoice || request.toggleOn)
            request.accept();
        }
        onOpenUrlInNewPrivateWindowRequested: {
            root.openUrl(url, true);
        }
    }

    function timeString(time) {
        return Qt.formatTime(time, "HH:mm");
    }

    function openRequest(request) {
        openUrl(request.requestedUrl);
    }

    function openUrl(url, background) {
        tabController.openUrl(url, background);
    }

    function toggleFullScreen() {
        setFullScreen(window.visibility !== Window.FullScreen);
        fullScreenUserChoice = isFullScreen;
    }

    function setFullScreen(fullscreen) {
        if (!fullscreen) {
            window.showNormal();
        } else {
            window.showFullScreen();
        }
        // required to preserve focus on Mac platform
        window.requestActivate();
    }

    width: 1024
    height: 640

    title: "%1 - Liri Browser %2".arg(tabController.tabsModel.empty ? qsTr("New window")
                                                                    : tabsModel.active.title
                                                                      // Use translation id to fix duplicate strings
                                                                      //= new-tab
                                                                      || qsTr("New tab"))
                                 .arg(incognito ? "(%1)".arg(qsTr("Private mode")) : "")

    Material.theme: darkThemeActive || incognito ? Material.Dark : Material.Light

    MouseArea {
        id: topAreaTrigger
        parent: window.overlay
        anchors.left: parent.left
        anchors.right: parent.right
        hoverEnabled: true

        z:100
        height: 15
        enabled: window.isFullScreen
        onClicked: mouse.accepted = false;
        onPressed: mouse.accepted = false;
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
        onContainsMouseChanged: {
            if (containsMouse) {
                isOnToolbarTrigger.enabled = true;
            }
        }
    }

    MouseArea {
        z:100
        id: isOnToolbarTrigger
        width: toolbarContainer.width
        height: headColumn.implicitHeight
        hoverEnabled: true
        parent: window.overlay
        onClicked: mouse.accepted = false;
        onPressed: mouse.accepted = false;
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
        onContainsMouseChanged: {
            if (!containsMouse)
            {
                enabled = false;
            }
        }
    }

    // Header
    header: ToolBar {
        id: toolbarContainer

        property color incognitoColor: "#263238"
        property color darkThemeColor: "#212121"

        property color backgroundColor: {
            if (incognito) {
                return incognitoColor;
            }
            else if (darkThemeActive) {
                return darkThemeColor
            }
            else if (!tabsModel.active.invalid && tabsModel.active.hasThemeColor && themeColorEnabled) {
                return tabsModel.active.themeColor;
            }
            else {
                return "white";
            }
        }
        property color foregroundColor: Utils.lightDark(backgroundColor, "#212121", "white")
        property color accentColor: Utils.lightDark(backgroundColor, defaultAccentColor, "white")
        property color defaultAccentColor: Material.color(Material.Pink)
        property bool hidden: isFullScreen
                              && !(topAreaTrigger.containsMouse || isOnToolbarTrigger.containsMouse)
                              && !toolbar.omnibox.editingUrl



        Layout.fillWidth: true
        Material.elevation: 0
        Material.primary: backgroundColor
        Material.background: backgroundColor
        Material.foreground: foregroundColor
        Material.accent: accentColor
        z: 5
        implicitHeight: toolbarWrapper.y + toolbarWrapper.implicitHeight

        Item {
            id: toolbarWrapper
            anchors {
                left: parent.left
                right: parent.right
            }
            y: toolbarContainer.hidden ? -height : 0
            implicitHeight: headColumn.implicitHeight
            height: implicitHeight

            ColumnLayout {
                id: headColumn
                anchors.fill: parent
                spacing: 0

                TabBar {
                    id: tabBar

                    Layout.fillWidth: true

                    tabController: tabController
                    tabsModel: tabController.tabsModel
                    newTabUrl: startUrl

                    onToggleMaximizeRequested: {
                        if (window.isMaximized || window.isFullScreen) {
                            window.showNormal();
                        } else if (window.isWindowed) {
                            window.showMaximized();
                        }
                        window.requestActivate();
                    }
                }

                Toolbar {
                    id: toolbar

                    Layout.fillWidth: true

                    tabController: tabController
                    tabsModel: tabController.tabsModel
                    searchUrl: window.searchUrl
                    leftActions: [
                        Action {
                            iconName: "navigation/arrow_back"
                            enabled: tabsModel.active.valid && tabsModel.active.canGoBack
                            onTriggered: tabsModel.active.goBack()
                        },
                        Action {
                            iconName: "navigation/arrow_forward"
                            enabled: tabsModel.active.valid && tabsModel.active.canGoForward
                            onTriggered: tabsModel.active.goForward()
                        }
                    ]
                    rightActions: [
                        Action {
                            enabled: tabsModel.active.valid && tabsModel.active.canReload
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
                            iconName: "navigation/fullscreen_exit"
                            visible: window.isFullScreen
                            onTriggered: {
                                toggleFullScreen()
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

            Behavior on y {
                PropertyAnimation {
                    duration: 300
                    easing.type: Easing.InOutCubic
                }
            }
        }

        Behavior on backgroundColor {
            ColorAnimation { duration: 100 }
        }

    }

    // Body
    Item {
        anchors.fill: parent

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

    ShortcutManager {
        id: shortcutManager
        window: window
        tabBar: tabBar
        toolbar: toolbar
        searchOverlay: searchOverlay
    }

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
            text: qsTr("New window")
            iconName: "action/open_in_new"
            onClicked: {
                var window = root.newWindow();
                window.showNormal();
            }
        }

        MenuItem {
            text: qsTr("Private Window")
            iconName: "hardware/security"
            onClicked: {
                var window = root.newIncognitoWindow();
                window.showNormal();
            }
        }

        MenuItem {
            text: qsTr("Find in page")
            iconName: "action/find_in_page"
            // Disable find in page overlay when there is no open tab
            enabled: !tabController.tabsModel.empty
            onClicked: {
                searchOverlay.open();
            }
        }

        MenuItem {
            text: qsTr("Downloads")
            iconName: "file/file_download"
            onClicked: {
                rightDrawer.loadContent(rightDrawer.downloads);
                rightDrawer.open();
            }
        }

        MenuItem {
            text: window.isFullScreen? qsTr("Exit fullscreen") : qsTr("Fullscreen")
            iconName: !window.isFullScreen ? "navigation/fullscreen" : "navigation/fullscreen_exit"
            onClicked: {
                toggleFullScreen();
            }
        }

        MenuItem {
            text: qsTr("Settings")
            iconName: "action/settings"
            onClicked: {
                tabController.openUrl("liri://settings");
            }
        }

        Connections {
            target: tabController.tabsModel
            onEmptyChanged: {
                // Hide find in page overlay when the last tab was closed
                if (empty && searchOverlay.showing)
                    searchOverlay.close();
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

    Connections {
        target: tabController.tabsModel
        onBeforeTabRemoved: {
            // Close the window if the last tab was closed
            if (tabsModel.count === 1)
                window.close();
        }
    }

    Component.onCompleted: {
        if (openStartUrl)
            tabController.openUrl(startUrl);
    }
}
