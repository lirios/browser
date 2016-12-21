/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Pierre Jacquier <pierrejacquier39@gmail.com>
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
import Fluid.Controls 1.0
import SlimeEngine 0.2
import ".."

FluidWindow {
    id: window
    visible: false
    width: 700
    height: 600
    title: 'Dev Tools - Liri Browser'

    WebView {
        id: engine
        anchors.fill: parent
        engine: WebEngine.QtWebEngine
    }

    function open(uid) {
//        engine.url = 'http://0.0.0.0:9992/devtools/inspector.html?ws=0.0.0.0:9992/devtools/page/' + uid;
        engine.url = 'http://0.0.0.0:9992';
        window.showNormal();
    }
}
