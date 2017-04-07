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
import QtWebEngine 1.4
import SlimeEngine 0.2
import "../.."

TabContent {
    id: content

    property var webview: webview
    property alias url: webview.url
    property alias profile: webview.profile
    property int webengine

    property bool hasThemeColor: false
    property color themeColor

    WebEngineView {
        id: webview
        anchors.fill: parent
        onFullScreenRequested: {
            actionManager.fullScreenRequested(request);
        }

        onNewViewRequested: {
            actionManager.newWebViewRequested(request);
        }
        onWindowCloseRequested: {
            actionManager.closeRequested();
        }
        onLoadingChanged: {
            if (loadRequest.status === WebEngineView.LoadSucceededStatus) {
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
    }

    Binding {
        target: content.tab
        property: "title"
        value: webview.title
    }

    Binding {
        target: content.tab
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
        target: content.tab
        property: "adaptIconColor"
        value: false
    }

    Binding {
        target: content.tab
        property: "url"
        value: webview.url
    }

    Binding {
        target: content.tab
        property: "canGoBack"
        value: webview.canGoBack
    }

    Binding {
        target: content.tab
        property: "canGoForward"
        value: webview.canGoForward
    }

    Binding {
        target: content.tab
        property: "loading"
        value: content.tab && content.tab.url.toString().length > 0 && webview.loadProgress < 100
    }

    Binding {
        target: content.tab
        property: "loadProgress"
        value: webview.loadProgress
    }

    Binding {
        target: content.tab
        property: "hasThemeColor"
        value: hasThemeColor
    }

    Binding {
        target: content.tab
        property: "themeColor"
        value: themeColor
    }

    Binding {
        target: content.tab
        property: "canReload"
        value: true
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
            if (webview.url !== url)
                webview.url = url;
        }
        onStop: {
            webview.stop();
        }
        onReload: {
            webview.reload();
        }
        onFindText: {
            var flags;
            if (backwards)
                flags = WebEngineView.FindBackward
            if (caseSensitive)
                flags |= WebEngineView.FindCaseSensitively
            webview.findText(text, flags);
        }
    }
}
