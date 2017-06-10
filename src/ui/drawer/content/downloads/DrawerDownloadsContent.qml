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

import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import Fluid.Controls 1.0
import core 1.0
import "../../.."

DrawerContentItem {
    id: drawerContentItem
    title: qsTr("Downloads")

    property DownloadsModel downloadsModel

    ListView {
        anchors.fill: parent
        clip: true
        model: downloadsModel
        delegate: DownloadItemDelegate {
            downloadsModel: drawerContentItem.downloadsModel
        }
    }

    Label {
        id: lblNoDownloads
        anchors.top: parent.top

        visible: downloadsModel.count === 0
        text: qsTr("No downloads for this session")
        wrapMode: Text.WordWrap
        width: Math.min(noDownloadsMetrics.width, parent.width)

        TextMetrics {
            id: noDownloadsMetrics
            font: lblNoDownloads.font
            text: lblNoDownloads.text
        }
    }
}
