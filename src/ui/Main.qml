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
import "."

QtObject {
    id: root

    property int webengine

    property WebProfile defaultProfile
    property WebProfile incognitoProfile

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

            Connections {
                target: engineDownload
                onFinished: {
                    console.log("Download finished")
                    downloadWatcher.downloadModelItem.finished = true;
                }
                onFailed: {
                    console.log("Download failed");
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
                value: engineDownload.progress
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

    property Component webProfileComponent: Component {
        WebProfile {
            engine: webengine
            onDownloadRequested: {
                // Accept download request, returns SlimeEngine Download item
                var engineItem = request.accept();
                // Create model representation
                var modelItem = downloadsModel.add();
                // Create download watcher
                var watcher = downloadWatcherComponent.createObject(null, {
                    engineDownload: engineItem,
                    downloadModelItem: modelItem
                });
            }
        }
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
        // Create the default profile after the webengine to use has been decided on the C++ side
        defaultProfile = webProfileComponent.createObject(root, {});
        // Create an incognito profile
        incognitoProfile = webProfileComponent.createObject(root, {incognito: true});
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
