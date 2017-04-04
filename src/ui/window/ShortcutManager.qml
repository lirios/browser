/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Ivan Fateev <johnny.j.poison@gmail.com>
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
import QtQuick.Window 2.0
import core 1.0
import ".."

Item {
    id: shortcutManager

    property var window
    property Toolbar toolbar
    property TabBar tabBar
    property SearchOverlay searchOverlay
    property var tabsModel: shortcutManager.window.tabsModel

    Connections {
        target: Qt.platform.os === "osx" ? MacEvents : null
        enabled:Qt.platform.os === "osx"
        ignoreUnknownSignals: true

        onCtrlShiftTabPressed: {
            tabsModel.setPreviousTabActive();
        }

        onCtrlTabPressed: {
            tabsModel.setNextTabActive();
        }
    }


    function setActiveTabByIndex(idx) {
        if (idx < 0 || idx >= tabsModel.count) {
            return;
        }

        tabsModel.setActive(tabsModel.get(idx));
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.Back
        onActivated: {
            tabsModel.active.goBack();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.Forward
        onActivated: {
            tabsModel.active.goForward();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.Close
        onActivated: {
            tabBar.tabCloseRequested(tabsModel.active.uid);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+l"
        onActivated: {
            toolbar.omnibox.focusUrlField();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.Refresh
        onActivated: {
            tabsModel.active.reload();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.AddTab
        onActivated: {
            tabBar.newTab();
        }
    }

    Shortcut {
        autoRepeat: false
        context: Qt.ApplicationShortcut
        // for Mac it's handled via macEvents
        // due to bug https://bugreports.qt.io/browse/QTBUG-8596
        sequence: "Ctrl+Shift+Tab"
        onActivated: {
            tabsModel.setPreviousTabActive();
        }
    }

    Shortcut {
        autoRepeat: false
        // for Mac it's handled via macEvents
        // due to bug https://bugreports.qt.io/browse/QTBUG-8596
        sequence: "Meta+Tab"
        onActivated: {
            tabsModel.setNextTabActive();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+1"
        onActivated: {
            setActiveTabByIndex(0);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+2"
        onActivated: {
            setActiveTabByIndex(1);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+3"
        onActivated: {
            setActiveTabByIndex(2);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+4"
        onActivated: {
            setActiveTabByIndex(3);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+5"
        onActivated: {
            setActiveTabByIndex(4);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+6"
        onActivated: {
            setActiveTabByIndex(5);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+7"
        onActivated: {
            setActiveTabByIndex(6);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+8"
        onActivated: {
            setActiveTabByIndex(7);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+9"
        onActivated: {
            setActiveTabByIndex(8);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Alt+0"
        onActivated: {
            setActiveTabByIndex(tabsModel.count - 1);
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Esc"
        onActivated: {
            if (toolbar.omnibox.editingUrl) {
                toolbar.forceActiveFocus();
            } else if (tabsModel.active.loading) {
                tabsModel.active.stop();
            } else if (searchOverlay.showing) {
                searchOverlay.close();
            }
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+f"
        onActivated: {
            searchOverlay.open();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+n"
        onActivated: {
            var window = window.root.newWindow();
            window.showNormal();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+Shift+n"
        onActivated: {
            var window = window.root.newIncognitoWindow();
            window.showNormal();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: Qt.platform.os == "osx" ? "Ctrl+Meta+F" : "F11"
        onActivated: {
            window.toggleFullScreen();
        }
    }

}
