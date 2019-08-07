
#import <Cocoa/Cocoa.h>
#import "ManderApp.h"
#include "NAUI.h"
#include "BitFiddleCommon.h"

@class BitFiddlePreferencesController;
@class ComplementWindowController;

@interface BitFiddleNSApplication : NSApplication <NSApplicationDelegate> {
  IBOutlet ComplementWindowController* complementwindowcontroller;
  IBOutlet ComplementWindowController* minicomplementwindowcontroller;
  
  IBOutlet NSMenuItem* byteswapmenuitem;
  IBOutlet NSMenuItem* unsignedmenuitem;
  IBOutlet NSMenuItem* onescomplementmenuitem;
  IBOutlet NSMenuItem* twoscomplementmenuitem;

  IBOutlet NSMenuItem* minimenuitem;

  NABool usemini;
}

- (id)init;

- (void)applicationDidFinishLaunching:(NSNotification *)notification;
- (void)update;

- (IBAction)openHelp:(NSMenuItem*)sender;
- (IBAction)showPreferences:(id)sender;
- (IBAction)showComplement:(id)sender;
- (IBAction)showASCII:(id)sender;

- (IBAction) switchMini:(id)sender;

- (IBAction)switchByteSwap:(id)sender;
- (IBAction)switchToUnsigned:(id)sender;
- (IBAction)switchToOnesComplement:(id)sender;
- (IBAction)switchToTwosComplement:(id)sender;

@end
