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
import QtQuick.Controls 2.0
import QtWebEngine 1.4
import "../.."

TabContent {
    id: content

    property var webview: webview
    property alias url: webview.url
    property alias profile: webview.profile
    property WebEngineNewViewRequest request

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
        onContextMenuRequested: {
            var contextMenu;
            if (request.linkUrl != "" || request.selectedText != "" || request.isContentEditable) {
                contextMenu = textContextMenu;
            } else if (mediaContextMenu.isSupported(request.mediaType)) {
                contextMenu = mediaContextMenu;
            } else {
                contextMenu = defaultContextMenu;
            }
            contextMenu.request = request;
            contextMenu.x = request.x;
            contextMenu.y = request.y;
            contextMenu.open();
            request.accepted = true;
        }
    }

    Menu {
        id: defaultContextMenu

        property var request

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        MenuItem {
            enabled: webview.canGoBack
            text: qsTr("Back")
            onTriggered: webview.goBack()
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            enabled: webview.canGoForward
            text: qsTr("Forward")
            onTriggered: webview.goForward()
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            text: qsTr("Reload")
            onTriggered: webview.reload()
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            text: qsTr("View page source")
            onTriggered: webview.triggerWebAction(WebEngineView.ViewSource)
            Keys.onReturnPressed: triggered()
        }

        Connections {
            target: webview
            onScrollPositionChanged: {
                defaultContextMenu.close();
            }
        }
    }

    Menu {
        id: textContextMenu

        property var request
        property bool isLink: textContextMenu.request ? textContextMenu.request.linkUrl != "" : false
        property bool isTextSelected: textContextMenu.request ? textContextMenu.request.selectedText !== "" : false
        property bool isEditable: textContextMenu.request ? textContextMenu.request.isContentEditable : false

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        MenuItem {
            visible: textContextMenu.isLink
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Open in new tab")
            onTriggered: webview.triggerWebAction(WebEngineView.OpenLinkInNewTab)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isLink
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Open in new window")
            onTriggered: webview.triggerWebAction(WebEngineView.OpenLinkInNewWindow)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isLink
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Open in  private window")
            onTriggered: actionManager.openUrlInNewPrivateWindowRequested(textContextMenu.request.linkUrl)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isLink
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Copy link location")
            onTriggered: webview.triggerWebAction(WebEngineView.CopyLinkToClipboard)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isLink
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Download link location")
            onTriggered: webview.triggerWebAction(WebEngineView.DownloadLinkToDisk)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isEditable
            enabled: textContextMenu.isTextSelected
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Cut")
            onTriggered: webview.triggerWebAction(WebEngineView.Cut)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isTextSelected || textContextMenu.isEditable
            enabled: textContextMenu.isTextSelected
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Copy")
            onTriggered: webview.triggerWebAction(WebEngineView.Copy)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isEditable
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Paste")
            onTriggered: webview.triggerWebAction(WebEngineView.Paste)
            Keys.onReturnPressed: triggered()
        }

        MenuItem {
            visible: textContextMenu.isTextSelected || textContextMenu.isEditable
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Select all")
            onTriggered: webview.triggerWebAction(WebEngineView.SelectAll)
            Keys.onReturnPressed: triggered()
        }

        Connections {
            target: webview
            onScrollPositionChanged: {
                textContextMenu.close();
            }
        }
    }

    Menu {
        id: mediaContextMenu

        property var request

        property bool isImage: typeof mediaContextMenu.request !== "undefined"
                               && mediaContextMenu.request.mediaType === ContextMenuRequest.MediaTypeImage

        function isSupported(mediaType) {
            return [
                ContextMenuRequest.MediaTypeImage,
                ContextMenuRequest.MediaTypeVideo,
                ContextMenuRequest.MediaTypeAudio,
            ].indexOf(mediaType) !== -1;
        }

        function mediaTypeToString(mediaType) {
            switch(mediaType) {
                case ContextMenuRequest.MediaTypeImage:
                    return qsTr("image");
                case ContextMenuRequest.MediaTypeVideo:
                    return qsTr("video");
                case ContextMenuRequest.MediaTypeAudio:
                    return qsTr("audio");
                default:
                    return qsTr("unknown");
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        MenuItem {
            text: {
                return qsTr("Open %1 in new tab").arg(
                    mediaContextMenu.request ? mediaContextMenu.mediaTypeToString(mediaContextMenu.request.mediaType)
                                             : ""
                )
            }
            onClicked: {
                actionManager.openUrlInNewTabRequested(mediaContextMenu.request.mediaUrl);
            }
        }

        MenuItem {
            text: {
                return qsTr("Copy %1 location").arg(
                    mediaContextMenu.request ? mediaContextMenu.mediaTypeToString(mediaContextMenu.request.mediaType)
                                             : ""
                )
            }
            onClicked: {
                if (mediaContextMenu.isImage) {
                    webview.triggerWebAction(WebEngineView.CopyImageUrlToClipboard);
                } else {
                    webview.triggerWebAction(WebEngineView.CopyMediaUrlToClipboard);
                }
            }
        }

        MenuItem {
            visible: mediaContextMenu.isImage
            height: visible ? MenuItem.implicitHeight : 0
            text: qsTr("Copy image")
            onClicked: {
                webview.triggerWebAction(WebEngineView.CopyImageToClipboard);
            }
        }

        MenuItem {
            text: {
                return qsTr("Download %1").arg(
                    mediaContextMenu.request ? mediaContextMenu.mediaTypeToString(mediaContextMenu.request.mediaType)
                                             : ""
                )
            }
            onClicked: {
                webview.triggerWebAction(WebEngineView.DownloadMediaToDisk);
            }
        }

        Connections {
            target: webview
            onScrollPositionChanged: {
                mediaContextMenu.close();
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
        value: webview.icon
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

    Component.onCompleted: {
        if (request !== null) {
            request.openIn(webview);
        }
    }
}
