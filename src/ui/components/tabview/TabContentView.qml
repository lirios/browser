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
import Liri.Browser.Core 1.0

Item {
    id: tabContentView
    property TabsModel tabsModel
    property ListModel pages: ListModel {}
    readonly property Item container: contentContainer

    function registerPage(uid, item) {
        // Make item invisible for now
        item.visible = false;
        // Reparent and anchor item
        item.parent = contentContainer;
        item.anchors.fill = contentContainer;
        // Create visibility binding
        item.visible = Qt.binding(function(){ return tabsModel.active.uid === uid });
        // Append to pages model
        pages.append({
            uid: uid,
            item: item
        });
    }

    function unregisterPage(uid) {
        var page = pageByUID(uid);
        if (!page)
            return false;
        // Make item invisible
        page.item.visible = false;
        // Destroy item
        page.item.destroy();
        // Remove page from model
        pages.remove(indexByUID(uid));
        return true;
    }

    function activatePage(uid) {

    }

    function pageByUID(uid) {
        for (var i=0; i<pages.count; i++) {
            if (pages.get(i).uid === uid)
                return pages.get(i);
        }
        return false;
    }

    function indexByUID(uid) {
        for (var i=0; i<pages.count; i++) {
            if (pages.get(i).uid === uid)
                return i;
        }
        return -1;
    }

    Item {
        id: contentContainer
        anchors.fill: parent
    }
}
