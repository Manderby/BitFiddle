
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NABase.h)

#if NA_OS == NA_OS_MAC_OS_X
#if defined __OBJC__
#if (NA_COMPILE_GUI == 1)

#import <Cocoa/Cocoa.h>

@interface BitFiddleNSApplication : NSApplication <NSApplicationDelegate> {
}

@end


#endif // (NA_COMPILE_GUI == 1)
#endif // defined __OBJC__
#endif // NA_OS == NA_OS_MAC_OS_X
