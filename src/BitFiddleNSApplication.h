
#import <Cocoa/Cocoa.h>
#import "ManderApp.h"
#include "NAUI.h"
#include "BitFiddleCommon.h"

@class BitFiddlePreferencesController;
@class ConverterControllerController;

@interface BitFiddleNSApplication : NSApplication <NSApplicationDelegate> {
  IBOutlet NSMenuItem* byteswapmenuitem;
  IBOutlet NSMenuItem* unsignedmenuitem;
  IBOutlet NSMenuItem* onescomplementmenuitem;
  IBOutlet NSMenuItem* twoscomplementmenuitem;
}

- (IBAction)openHelp:(NSMenuItem*)sender;

- (IBAction)switchByteSwap:(id)sender;
- (IBAction)switchToUnsigned:(id)sender;
- (IBAction)switchToOnesComplement:(id)sender;
- (IBAction)switchToTwosComplement:(id)sender;

@end
