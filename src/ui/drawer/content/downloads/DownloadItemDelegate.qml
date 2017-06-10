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
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import Fluid.Controls 1.0
import QtWebEngine 1.1


BaseListItem {
    property var downloadsModel
    readonly property var download: downloadsModel.get(index)

    readonly property bool finished: download.state === WebEngineDownloadItem.DownloadCompleted
    readonly property real progress: download.receivedBytes / download.totalBytes

    implicitHeight: 74

    RowLayout {
        anchors {
            fill: parent
            topMargin: Units.smallSpacing
            bottomMargin: Units.smallSpacing
        }

        spacing: Units.smallSpacing

        Icon {
            name: "file/file_download"
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Units.smallSpacing

            Label {
                Layout.fillWidth: true
                text: download.path
                elide: Text.ElideLeft
            }

            ProgressBar {
                Layout.fillWidth: true
                visible: !finished
                height: finished ? 0 : implicitHeight
                value: progress
            }

            CaptionLabel {
                text: finished ? "Finished" : "%1%".arg(Math.round(progress * 100).toString())
            }
        }

        IconButton {
            iconName: "navigation/cancel"
            onClicked: {
                // Cancel engine download
                download.cancel();
                // Remove from model
                downloadsModel.remove(index);
            }
        }
    }

    onClicked: {
        // Open download file externally
        if (finished)
            Qt.openUrlExternally("file://" + download.path);
    }
}
