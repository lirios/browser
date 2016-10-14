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
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import Fluid.Controls 1.0


BaseListItem {
    property var downloadsModel
    property var download: downloadsModel.get(index)

    implicitHeight: 74

    RowLayout {
        anchors {
            fill: parent
            topMargin: 8
            bottomMargin: 8
        }

        spacing: 8

        Icon {
            name: "file/file_download"
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            Label {
                Layout.fillWidth: true
                text: download.path
                elide: Text.ElideLeft
            }

            ProgressBar {
                Layout.fillWidth: true
                visible: !download.finished
                height: download.finished ? 0 : implicitHeight
                value: download.progress/100
            }

            CaptionLabel {
                text: download.finished ? "Finished" : "%1%".arg(download.progress.toString())
            }
        }

        IconButton {
            iconName: "navigation/cancel"
            onClicked: {
                // Cancel engine download
                download.cancel();
                // Remove from model
                downloadsModel.remove(index)
            }
        }
    }

    onClicked: {
        // Open download file externally
        if (download.finished)
            Qt.openUrlExternally("file://" + download.path);
    }
}
