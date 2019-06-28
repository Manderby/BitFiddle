
#import <Cocoa/Cocoa.h>
#import "ManderApp.h"

@class MandAboutWindowController;
@class BitFiddlePreferencesController;
@class ComplementWindowController;
@class ASCIIWindowController;


typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT = 1,
  COMPUTE_TWOS_COMPLEMENT = 2,
} ConversionType;



@interface BitFiddleApplication : ManderApp {
  IBOutlet BitFiddlePreferencesController* preferenceswindowcontroller;
  IBOutlet ComplementWindowController* complementwindowcontroller;
  IBOutlet ComplementWindowController* minicomplementwindowcontroller;
  IBOutlet ASCIIWindowController* asciiwindowcontroller;
  
  IBOutlet NSMenuItem* byteswapmenuitem;
  IBOutlet NSMenuItem* unsignedmenuitem;
  IBOutlet NSMenuItem* onescomplementmenuitem;
  IBOutlet NSMenuItem* twoscomplementmenuitem;

  IBOutlet NSMenuItem* minimenuitem;

  NABool switchingWindowMode;
  NABool byteswap;
  ConversionType conversiontype;
  NABool usemini;
  NAInt translatorGroup;
}

- (id)init;

- (void)applicationDidFinishLaunching:(NSNotification *)notification;
- (void)update;

- (NAInt)getTranslatorGroup;

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
