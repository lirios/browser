/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <tim.sueberkrueb@web.de>
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
import "../.."

TabContent {
    id: content

    property var webview: webview
    property alias profile: webview.profile
    property alias request: webview.request
    property int webengine

    canReload: true
    canGoBack: webview.canGoBack
    canGoForward: webview.canGoForward
    loading: webview.loadProgress < 100
    loadProgress: webview.loadProgress
    iconUrl: webview.icon
    adaptIconColor: false
    title: webview.title
    hasThemeColor: false

    WebView {
        id: webview
        engine: webengine
        anchors.fill: parent
        onFullScreenRequested: {
            actionManager.fullScreenRequested(request);
        }

        onNewViewRequested: {
            actionManager.newWebViewRequested(request);
        }
        onCloseRequested: {
            actionManager.closeRequested();
        }
        onLoadStatusChanged: {
            if (loadStatus === LoadStatus.LoadSucceeded) {
                // Search for theme color
                runJavaScript("
                    function getThemeColor () {
                        var metaTags = document.getElementsByTagName('meta');
                        for (i=0; i<metaTags.length; i++) {
                            if (metaTags[i].getAttribute('name') === 'theme-color') {
                                return metaTags[i].getAttribute('content');
                            }
                        }
                        return '';
                    }
                    getThemeColor();
                ", function callback(content){
                    if (content) {
                        themeColor = content;
                        hasThemeColor = true;
                    }
                    else {
                        hasThemeColor = false;
                    }
                });
            }
        }

        // WebView url needs to be handled this way
        // to prevent a binding loop.
        onUrlChanged: {
            if (url != "about:blank" && url != content.url)
                content.url = url;
        }
    }

    Connections {
        target: content.tab
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
        onFindText: {
            webview.findText(text, backwards, caseSensitive);
        }
    }
}
