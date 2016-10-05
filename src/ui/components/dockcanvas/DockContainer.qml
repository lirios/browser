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

Rectangle {
    id: dockContainer
    implicitHeight: 64
    implicitWidth: 64

    border.width: 2
    border.color: "white"
    color: dropArea.containsDrag ? "white" : "transparent"

    DropArea {
        id: dropArea
        anchors.fill: parent

        onDropped: {
            drop.source.container = dockContainer;
        }
    }
}
