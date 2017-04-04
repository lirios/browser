#include "MacOsEventListener.h"
#import <Foundation/Foundation.h>
#import <AppKit/NSEvent.h>

void initMacOsEventListener(std::function<void (Event)> listener)
{
     [NSEvent addLocalMonitorForEventsMatchingMask:
                (NSLeftMouseDownMask | NSRightMouseDownMask | NSOtherMouseDownMask | NSKeyDownMask)
                handler:^NSEvent*(NSEvent *incomingEvent) {
        return nil;
//            NSEvent *result = incomingEvent;
//            NSWindow *targetWindowForEvent = [incomingEvent window];
//            if (targetWindowForEvent != _window) {
//                [self _closeAndSendAction:NO];
//            } else if ([incomingEvent type] == NSKeyDown) {
//                if ([incomingEvent keyCode] == 53) {
//                    // Escape
//                    [self _closeAndSendAction:NO];
//                    result = nil; // Don't process the event
//                } else if ([incomingEvent keyCode] == 36) {
//                    // Enter
//                    [self _closeAndSendAction:YES];
//                    result = nil;
//                }
//            }
//            return result;
        }];

}
