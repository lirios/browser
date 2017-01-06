import QtQuick 2.7
import core 1.0
import ".."

Item {
    property TabsModel tabsModel
    property Toolbar toolbar
    property TabBar tabBar

    function setActiveTabByIndex(idx) {
        if (idx < 0 || idx >= tabsModel.count) {
            return;
        }

        var tab = tabsModel.get(idx);
        if (tab) {
            tabsModel.setActive(tab);
        }
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
        // it's a hack since StandardKey.PreviousChild not working for current Qt 5.7
        // TODO: for mac it should be forced to Ctrl+Shift+Tab somehow
        sequence: "Ctrl+Shift+Tab"
        onActivated: {
            tabsModel.setPreviousTabActive();
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.NextChild
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
            }
        }
    }
}
