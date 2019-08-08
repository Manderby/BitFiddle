
#import <Cocoa/Cocoa.h>
#import "ManderApp.h"
#include "NAUI.h"
#include "BitFiddleCommon.h"

@class BitFiddlePreferencesController;
@class ConverterWindowController;

@interface BitFiddleNSApplication : NSApplication <NSApplicationDelegate> {
  IBOutlet NSMenuItem* byteswapmenuitem;
  IBOutlet NSMenuItem* unsignedmenuitem;
  IBOutlet NSMenuItem* onescomplementmenuitem;
  IBOutlet NSMenuItem* twoscomplementmenuitem;
}

- (id)init;

- (void)applicationDidFinishLaunching:(NSNotification *)notification;

- (IBAction)openHelp:(NSMenuItem*)sender;
- (IBAction)showPreferences:(id)sender;
- (IBAction)showComplement:(id)sender;
- (IBAction)showASCII:(id)sender;

- (IBAction)switchByteSwap:(id)sender;
- (IBAction)switchToUnsigned:(id)sender;
- (IBAction)switchToOnesComplement:(id)sender;
- (IBAction)switchToTwosComplement:(id)sender;

@end
