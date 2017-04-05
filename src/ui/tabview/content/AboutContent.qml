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
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Fluid.Core 1.0
import "../.."

TabContent {
    id: content

    canReload: false
    canGoBack: false
    canGoForward: false
    loading: false
    loadProgress: 100
    iconUrl: Utils.getSourceForIconName("action/info_outline")
    adaptIconColor: true
    title: "About Liri Browser"
    url: "liri://about"
    hasThemeColor: false

    Column {
        anchors {
            fill: parent
            margins: Units.smallSpacing * 2
        }

        spacing: Units.smallSpacing

        HeadlineLabel {
            text: "Liri Browser"
        }

        TitleLabel {
            text: ApplicationVersion
        }

        BodyLabel {
            text: "Liri Browser is a cross-platform material design webbrowser."
            wrapMode: Text.WordWrap
            width: parent.width
        }

        BodyLabel {
            text: "Licensed under the terms of the GNU General Public License version 3 or, at your option, any later version."
            wrapMode: Text.WordWrap
            width: parent.width
        }

        SubheadingLabel {
            text: "Powered by Qt %1".arg(QtVersion)
        }

        SubheadingLabel {
            text: "Running on %1".arg(SystemInformation)
            wrapMode: Text.WordWrap
            width: parent.width
        }
    }
}
