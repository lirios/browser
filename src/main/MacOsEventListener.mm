#include "MacOsEventListener.h"
#import <Foundation/Foundation.h>
#import <AppKit/NSEvent.h>

static const unsigned short kTabKey = 0x30;


MacOsEventListener* initMacOsEventListener(MacOsEventListener* ev)
{
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

}
