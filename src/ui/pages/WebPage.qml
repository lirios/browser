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
import ".."

TabContentPage {
    id: page

    property var webview: webview
    property alias url: webview.url
    property alias profile: webview.profile
    property alias request: webview.request
    property int webengine

    WebView {
        id: webview
        engine: webengine
        anchors.fill: parent
        onNewViewRequested: {
            actionManager.newWebViewRequested(request);
        }
        onCloseRequested: {
            actionManager.closeRequested();
        }
    }

    Binding {
        target: page.tab
        property: "title"
        value: webview.title
    }

    Binding {
        target: page.tab
        property: "iconUrl"
        // Workaround: it looks like QtWebEngine's icon provider (image://favicon/)
        // is not 100% reliable
        value: {
            var s = webview.icon.toString();
            if (s.indexOf("image://favicon/") === 0)
                return Qt.resolvedUrl(s.slice(16));
            else return webview.icon;
        }
    }

    Binding {
        target: page.tab
        property: "url"
        value: webview.url
    }

    Binding {
        target: page.tab
        property: "canGoBack"
        value: webview.canGoBack
    }

    Binding {
        target: page.tab
        property: "canGoForward"
        value: webview.canGoForward
    }

    Binding {
        target: page.tab
        property: "loading"
        value: webview.loadProgress < 100
    }

    Binding {
        target: page.tab
        property: "loadProgress"
        value: webview.loadProgress
    }

    Connections {
        target: page.tab
        onGoBack: {
            webview.goBack()
        }
        onGoForward: {
            webview.goForward();
        }
        onUrlChanged: {
            webview.url = url;
        }
        onStop: {
            webview.stop();
        }
        onReload: {
            webview.reload();
        }
    }
}
