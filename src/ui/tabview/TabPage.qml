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

    property var tab
    property var actionManager
    property var profile
    property url previousUrl
    property bool loading: false

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

    function openUrl(url) {
        load(TabType.fromUrl(url), {
            url: url,
        });
    }

    function load(type, data) {
        if (loading)
            return false;
        loading = true;

        var newContent;

        // Destroy old content if necessary
        if (contentItem)
            contentItem.destroy();

        // Finalize properties
        if (!("properties" in data))
            data["properties"] = {};

        data.properties["anchors.fill"] = contentContainer;
        data.properties["profile"] = profile;
        data.properties["url"] = data.url;

        // Create content item
        switch(type) {
            case TabType.webview:
                newContent = webContentComponent.createObject(
                    contentContainer,
                    data.properties
                );
                break;
            case TabType.settings:
                newContent = settingsContentComponent.createObject(
                    contentContainer,
                    data.properties
                );
                break;
        }
        tabType = type;
        contentItem = newContent;
        loading = false;
        return true;
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
