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
import QtWebEngine 1.1
import core 1.0
import "."

QtObject {
    id: root

    property var windows: []

    property WebEngineProfile defaultProfile: WebEngineProfile {
        persistentCookiesPolicy: Settings.startConfig.persistentCookies
                                        ? WebEngineProfile.ForcePersistentCookies
                                        : WebEngineProfile.NoPersistentCookies
        storageName: "liri-browser"
        onDownloadRequested: {
            __handleDownloadRequest(download);
        }
    }

    property WebEngineProfile incognitoProfile: WebEngineProfile {
        offTheRecord: true
        onDownloadRequested: {
            __handleDownloadRequest(download);
        }
    }

    property DownloadsModel downloadsModel: DownloadsModel {}

    property Component browserWindowComponent: Component {
        BrowserWindow {
            profile: root.defaultProfile
            downloadsModel: root.downloadsModel
        }
    }

    property Binding darkStartTimeBinding: Binding {
        target: DarkThemeTimer
        property: "startTime"
        value: Settings.themeConfig.darkThemeStartTime
    }

    property Binding darkEndTimeBinding: Binding {
        target: DarkThemeTimer
        property: "endTime"
        value: Settings.themeConfig.darkThemeEndTime
    }

    function __handleDownloadRequest(download) {
        download.accept();
        downloadsModel.add(download);
    }

    function openWindowRequest(request) {
        var window = newWindow(false, false);
        window.openRequest(request);
        window.showNormal();
    }

    function openUrl(url, incognito) {
        for (var i=0; i<windows.length; i++) {
            var window = windows[i];
            if (incognito === window.incognito) {
                window.openUrl(url, false);
                return;
            }
        }
        openUrlInNewWindow(url, incognito);
    }

    function openUrlInNewWindow(url, incognito) {
        var window = newWindow(incognito, false);
        window.openUrl(url, false);
        window.showNormal();
    }

    function newWindow(incognito, openStartUrl) {
        var properties = {root: root}
        if (incognito)
            properties["profile"] = incognitoProfile;
        properties["openStartUrl"] = openStartUrl;
        var window = browserWindowComponent.createObject(root, properties);
        windows.push(window);
        return window;
    }

    function newIncognitoWindow() {
        return newWindow(true);
    }

    function openNewWindow(incognito, openStartUrl) {
        newWindow(incognito, openStartUrl).showNormal();
    }

    function destroyWindow(window) {
        windows.splice(windows.indexOf(window), 1);
        window.destroy();
    }

    Component.onCompleted: {
        // Update dark theme timer
        DarkThemeTimer.update();
    }

    Component.onDestruction: {
        if (Settings.dirty) {
            console.log("Saving settings ...");
            Settings.save();
        }
    }
}
