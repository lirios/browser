/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Tim Süberkrüb (https://github.com/tim-sueberkrueb)
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
 */

import QtQuick 2.7
import Fluid.Controls 1.0
import core 1.0
import "../.."

DrawerContentItem {
    title: "Downloads"

    property DownloadsModel downloadsModel

    ListView {
        anchors {
            fill: parent
            margins: 16
        }
        model: downloadsModel
        delegate: ListItem {
            property var download: downloadsModel.get(index)

            iconName: "file/file_download"
            text: download.path
            subText: download.finished ? "Finished" : "%1%".arg(download.progress)
            rightItem: IconButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    leftMargin: 8
                }
                iconName: "navigation/cancel"
                onClicked: {
                    // Cancel engine download
                    download.cancel();
                    // Remove from model
                    downloadsModel.remove(index)
                }
            }

            onClicked: {
                // Open download file externally
                if (download.finished)
                    Qt.openUrlExternally("file://" + download.path);
            }
        }
    }
}
