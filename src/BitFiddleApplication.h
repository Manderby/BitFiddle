
#import <Cocoa/Cocoa.h>
#import "ManderApp.h"
#include "NAUI.h"

@class MandAboutWindowController;
@class BitFiddlePreferencesController;
@class ComplementWindowController;


typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT = 1,
  COMPUTE_TWOS_COMPLEMENT = 2,
} ConversionType;



@interface BitFiddleApplication : ManderApp {
  IBOutlet ComplementWindowController* complementwindowcontroller;
  IBOutlet ComplementWindowController* minicomplementwindowcontroller;
  
  IBOutlet NSMenuItem* byteswapmenuitem;
  IBOutlet NSMenuItem* unsignedmenuitem;
  IBOutlet NSMenuItem* onescomplementmenuitem;
  IBOutlet NSMenuItem* twoscomplementmenuitem;

  IBOutlet NSMenuItem* minimenuitem;

  NABool switchingWindowMode;
  NABool byteswap;
  ConversionType conversiontype;
  NABool usemini;

  NAWindow* asciiWindow;
  NAWindow* preferencesWindow;
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

- (NABool)byteswap;
- (ConversionType)conversiontype;
@end
