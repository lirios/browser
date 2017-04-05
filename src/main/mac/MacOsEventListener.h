/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2016 Ivan Fateev <johnny.j.poision@gmail.com>
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

#ifndef MACOSEVENTLISTENER_H
#define MACOSEVENTLISTENER_H

#include <string>
#include <functional>
#include <QObject>

enum class TabEvent {
    ShiftTab,
    CtrlShiftTab
};
class MacOsEventListener;

MacOsEventListener* initMacOsEventListener(MacOsEventListener *parent);

class MacOsEventListener : public QObject {
    Q_OBJECT

public:
    MacOsEventListener(QObject* parent = 0) : QObject(parent) {}

signals:
    void ctrlShiftTabPressed();
    void ctrlTabPressed();

};

#endif // MACOSEVENTLISTENER_H
