
#import <Carbon/Carbon.h>

#import "BitFiddleNSApplication.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIController.h"
#include "ConverterController.h"
#include "PreferencesController.h"
#include "ManderAppAbout.h"
#include "BitFiddleApplication.h"

OSStatus MyHotKeyHandler(EventHandlerCallRef nextHandler, EventRef theEvent, void *userData){
  NSLog(@"The hot key was pressed.");
//  CalcApplication* app = (CalcApplication*)userData;
//  [app show];
  return noErr;
}

CGEventRef onKeyDown(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
  NSLog(@"DOWN (%lli)", CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
  // When it matches, I return CGEventCreate(NULL) to stop the event
  return event;
}

@implementation BitFiddleNSApplication


- (id)init{
  self = [super init];
  [self setDelegate:self];
  return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  
  // We provide a hotkey: ctrl-option-command-I because I is the numerical key
  // for the number 5.
  EventHotKeyRef gMyHotKeyRef;
  EventHotKeyID gMyHotKeyID;
  EventTypeSpec eventType;
  eventType.eventClass=kEventClassKeyboard;
  eventType.eventKind=kEventHotKeyPressed;
  InstallApplicationEventHandler(&MyHotKeyHandler, 1, &eventType, self, NULL);
  gMyHotKeyID.signature='bitf';
  gMyHotKeyID.id=1;
//  RegisterEventHotKey(kVK_ANSI_B, cmdKey+optionKey+controlKey, gMyHotKeyID, GetApplicationEventTarget(), 0, &gMyHotKeyRef); //global
  RegisterEventHotKey(kVK_ANSI_B, cmdKey+optionKey+controlKey, gMyHotKeyID, GetEventDispatcherTarget(), 0, &gMyHotKeyRef);  //app

//  CFMachPortRef downEventTap = CGEventTapCreate(kCGSessionEventTap,kCGHeadInsertEventTap,kCGEventTapOptionDefault,CGEventMaskBit(kCGEventKeyDown),&onKeyDown,(__bridge void *)(self));
//  CFRunLoopSourceRef downSourceRef = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, downEventTap, 0); //<-- Crash exc_bad_access: downEventTap = 0x0,downSourceRef= 0x0
//  CFRelease(downEventTap);
//  CFRunLoopAddSource(CFRunLoopGetCurrent(), downSourceRef, kCFRunLoopDefaultMode);
//  CFRelease(downSourceRef);
}



- (void)applicationWillTerminate:(NSNotification *)notification{
  bitClearApp();
  naStopApplication();
  naStopRuntime();
}


- (IBAction)openHelp:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  NSString* language = [[NSLocale currentLocale] languageCode];
  if([language isEqualToString:@"de"]){
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://manderc.com/apps/bitfiddle/help/index.php"]];
  }else{
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://manderc.com/apps/bitfiddle/help/index_en.php"]];
  }
}



- (IBAction)switchByteSwap:(id)sender{
  bitSetEndiannessSwap(!bitGetEndiannessSwap());
}
- (IBAction)switchToUnsigned:(id)sender{
  bitSetConversionType(COMPUTE_UNSIGNED);
}
- (IBAction)switchToOnesComplement:(id)sender{
  bitSetConversionType(COMPUTE_ONES_COMPLEMENT);
}
- (IBAction)switchToTwosComplement:(id)sender{
  bitSetConversionType(COMPUTE_TWOS_COMPLEMENT);
}


@end
