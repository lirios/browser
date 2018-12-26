/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Ivan Fateev <johnny.j.poision@gmail.com>
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

#include "MacOsEventListener.h"
#import <Foundation/Foundation.h>
#import <AppKit/NSEvent.h>

#include <AvailabilityMacros.h>

#ifndef MAC_OS_X_VERSION_10_12
#  define MAC_OS_X_VERSION_10_12 101200
#endif

static const unsigned short kTabKey = 0x30;

MacOsEventListener* initMacOsEventListener(MacOsEventListener* ev)
{
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_12
    [NSEvent addLocalMonitorForEventsMatchingMask:
                (NSEventMaskLeftMouseDown | NSEventMaskRightMouseDown | NSEventMaskOtherMouseDown | NSEventMaskKeyDown)
                handler:^NSEvent*(NSEvent *incomingEvent) {
                    NSUInteger flags = [incomingEvent modifierFlags] & NSEventModifierFlagDeviceIndependentFlagsMask;
                    if ([incomingEvent type] == NSEventTypeKeyDown && ([incomingEvent keyCode] == kTabKey) && (flags & NSEventModifierFlagControl)) {
                        if(flags & NSEventModifierFlagShift) {
                            emit ev->ctrlShiftTabPressed();
                        } else {
                            emit ev->ctrlTabPressed();
                        }
                    }
                    return incomingEvent;
        }];
#else
    [NSEvent addLocalMonitorForEventsMatchingMask:
                (NSLeftMouseDownMask | NSRightMouseDownMask | NSOtherMouseDownMask | NSKeyDownMask)
                handler:^NSEvent*(NSEvent *incomingEvent) {
                    NSUInteger flags = [incomingEvent modifierFlags] & NSDeviceIndependentModifierFlagsMask;
                    if ([incomingEvent type] == NSKeyDown && ([incomingEvent keyCode] == kTabKey) && (flags & NSControlKeyMask)) {
                        if(flags & NSShiftKeyMask) {
                            emit ev->ctrlShiftTabPressed();
                        } else {
                            emit ev->ctrlTabPressed();
                        }
                    }
                    return incomingEvent;
        }];
#endif
}
