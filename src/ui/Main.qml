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
import QtWebEngine 1.4
import core 1.0
import "."

QtObject {
    id: root

    property WebEngineProfile defaultProfile: WebEngineProfile {
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

    property Component downloadWatcherComponent: Component {
        Item {
            id: downloadWatcher
            property var engineDownload
            property var downloadModelItem

             property real progress: (engineDownload.receivedBytes/engineDownload.totalBytes) * 100

            Connections {
                target: engineDownload
                onStateChanged: {
                    if (state === WebEngineDownloadItem.DownloadCompleted) {
                        downloadWatcher.downloadModelItem.finished = true;
                    }
                }
            }

            Connections {
                target: downloadModelItem
                onCancel: {
                    engineDownload.cancel();
                }
            }

            Binding {
                target: downloadModelItem
                property: "progress"
                value: progress
            }

            Binding {
                target: downloadModelItem
                property: "path"
                value: engineDownload.path
            }

            Binding {
                target: downloadModelItem
                property: "mimeType"
                value: engineDownload.mimeType
            }
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
        var modelItem = downloadsModel.add();
        var watcher = downloadWatcherComponent.createObject(null, {
            engineDownload: download,
            downloadModelItem: modelItem
        });
    }

    function openWindowRequest(request) {
        var window = newWindow(false, false);
        window.openRequest(request);
        window.showNormal();
    }

    function newWindow(incognito, openStartUrl) {
        var properties = {root: root}
        if (incognito)
            properties["profile"] = incognitoProfile;
        properties["openStartUrl"] = openStartUrl;
        var window = browserWindowComponent.createObject(root, properties);
        return window;
    }

    function newIncognitoWindow() {
        return newWindow(true);
    }

    function load() {
        // Update dark theme timer
        DarkThemeTimer.update();
        // Create the first window and show it
        newWindow().showNormal();
    }

    Component.onDestruction: {
        if (Settings.dirty) {
            console.log("Saving settings ...");
            Settings.save();
        }
    }
}
