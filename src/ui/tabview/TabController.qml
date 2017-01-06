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
import SlimeEngine 0.2
import core 1.0
import ".."

QtObject {
    id: tabController
    property TabBar tabBar
    property TabContentView tabContentView
    property TabsModel tabsModel
    property WebProfile profile
    property int lastUID: 0
    property int webengine

    property Component tabPageComponent: Component { TabPage {} }
    property Component actionManagerComponent: Component { TabActionManager {} }

    property QtObject internal: QtObject {
        property Connections tabBarCloseConnections: Connections {
            target: tabBar
            onTabCloseRequested: {
                removeTab(uid);
            }
        }
    }

    signal newWindowRequested(var request)

    function openUrl(url, background) {
        addTab(TabType.fromUrl(url), {
            url: url,
            background: background
        });
    }

    function openNewViewRequest(request) {
        addTab(TabType.webview, {
            background: request.destination === NewViewRequest.NewViewInBackgroundTab,
            properties: {
                profile: profile,
                request: request
            }
        });
    }

    function addTab(type, data) {
        // Register new unique id
        var uid = lastUID++;
        var page;

        if (!("properties" in data))
            data["properties"] = {};

        // Add tab model representation
        tabsModel.add(uid);

        // Create page
        page = tabPageComponent.createObject(tabContentView.container, {
            profile: profile,
            tab: tabsModel.byUID(uid),
            webengine: webengine
        });

        // Load page
        page.load(type, data);

        // Create an action manager for this tab
        page.actionManager = actionManagerComponent.createObject(page, {});
        page.actionManager.internal.tabController = tabController;
        page.actionManager.internal.uid = uid;
        // Register page to content view
        tabContentView.registerPage(uid, page);

        // Set the page active if wanted
        if (!data.background) {
            tabsModel.setActive(uid);
        }
    }

    function removeTab(uid) {
        // Remove model representation
        if (!tabsModel.remove(uid))
            return false;
        // Unregister page
        tabContentView.unregisterPage(uid);
        // Set previous tab active
//        tabsModel.setPreviousTabActive();
        return true;
    }

    function tabByUID(uid) {
        return tabsModel.byUID(uid);
    }

    function pageByUID(uid) {
        return tabContentView.pageByUID(uid);
    }
}
