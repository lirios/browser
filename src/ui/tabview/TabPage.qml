/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb <dev@timsueberkrueb.io>
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
import ".."

Item {
    id: tabPage

    // binding. should be set from outside
    property var activeTab
    property var tab
    property var actionManager
    property var profile
    property url previousUrl
    property bool loading: false
    property bool loadContent: true
    property var contentFabric
    property var initData

    property Component settingsContentComponent: Component {
        SettingsContent {
            tab: tabPage.tab
            actionManager: tabPage.actionManager
        }
    }

    property Component webContentComponent: Component {
        WebContent {
            tab: tabPage.tab
            actionManager: tabPage.actionManager
        }
    }

    property var contentItem
    property int tabType

    function tryCreateContent() {
        if (contentItem || !loadContent || tab !== activeTab)
            return;

        contentFabric();
    }

    function openUrl(url) {
        load(TabType.fromUrl(url), {
            url: url,
        });
    }

    function load(type, data) {
        if (loading)
            return false;
        loading = true;
        initData = data;

        contentFabric = function() {
            var newContent;

            // Destroy old content if necessary
            if (contentItem)
                contentItem.destroy();

            // Finalize properties
            if (!("properties" in initData))
                initData["properties"] = {};

            initData.properties["anchors.fill"] = contentContainer;
            initData.properties["profile"] = profile;
            initData.properties["url"] = initData.url;

            // Create content item
            switch(type) {
                case TabType.webview:
                    newContent = webContentComponent.createObject(
                        contentContainer,
                        initData.properties
                    );
                    break;
                case TabType.settings:
                    newContent = settingsContentComponent.createObject(
                        contentContainer,
                        initData.properties
                    );
                    break;
            }

            tabType = type;
            contentItem = newContent;
            loading = false;
        };

        if (loadContent) {
            contentFabric();
        }

        return true;
    }

    onActiveTabChanged: {
        tryCreateContent();
    }

    onLoadContentChanged: {
        if (contentFabric)
            tryCreateContent();
    }

    Binding {
        target: tab
        property: "title"
        value: initData ? initData['title'] : ""
        when: !contentItem
    }

    Binding {
        target: tab
        property: "url"
        value: initData ? initData['url'] : ""
        when: !contentItem
    }

    Binding {
        target: tab
        property: "iconUrl"
        value: initData ? initData['iconUrl'] : ""
        when: !contentItem
    }

    Connections {
        target: tab
        onUrlChanged: {
            if (url !== previousUrl) {
                var newType = TabType.fromUrl(url);
                if (newType !== tabType) {
                    load(newType, {url: url});
                }
                previousUrl = url;
            }
        }
    }

    Item {
        id: contentContainer
        anchors.fill: parent
    }
}
