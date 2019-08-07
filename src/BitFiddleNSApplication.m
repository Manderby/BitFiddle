

#import "BitFiddleNSApplication.h"
#import "ComplementWindowController.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIWindow.h"
#include "ComplementWindow.h"
#include "PreferencesWindow.h"
#include "ManderAppAbout.h"
#include "BitFiddleApplication.h"

@implementation BitFiddleNSApplication

- (id)init{
  self = [super init];
  [self setDelegate:self];
  return self;
}



- (void)applicationDidFinishLaunching:(NSNotification *)notification{

  [NSBundle loadNibNamed:@"ComplementWindow" owner:self];
  
  [complementwindowcontroller setMini:NA_FALSE];
  [minicomplementwindowcontroller setMini:NA_TRUE];
    
  usemini = naGetPreferencesBool(BitPrefs[UseMini]);
    
  [self update];
  [self showComplement:self];

  NABool showASCIIOnStartup = naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]);
  if(showASCIIOnStartup){[self showASCII:self];}
}


- (void)applicationWillTerminate:(NSNotification *)notification{
  
  [complementwindowcontroller release];
  [minicomplementwindowcontroller release];

  naStopApplication();
  naStopRuntime();
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender{
  return FALSE;
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



- (IBAction)showPreferences:(id)sender{
  bitShowPreferencesWindow();
}


- (void)update{
  NABool keepMaxiOnTop = naGetPreferencesBool(BitPrefs[KeepMaxiOnTop]);
  NABool keepMiniOnTop = naGetPreferencesBool(BitPrefs[KeepMiniOnTop]);

//  [byteswapmenuitem setState:swapEndianness ? NSOnState : NSOffState];
//  [unsignedmenuitem setState:(conversiontype == COMPUTE_UNSIGNED)?NSOnState:NSOffState];
//  [onescomplementmenuitem setState:(conversiontype == COMPUTE_ONES_COMPLEMENT)?NSOnState:NSOffState];
//  [twoscomplementmenuitem setState:(conversiontype == COMPUTE_TWOS_COMPLEMENT)?NSOnState:NSOffState];
  [minimenuitem setState:usemini ? NSOnState : NSOffState];
  if(keepMaxiOnTop){
    [[complementwindowcontroller window] setLevel:NSFloatingWindowLevel];
  }else{
    [[complementwindowcontroller window] setLevel:NSNormalWindowLevel];
  }
  if(keepMiniOnTop){
    [[minicomplementwindowcontroller window] setLevel:NSFloatingWindowLevel];
  }else{
    [[minicomplementwindowcontroller window] setLevel:NSNormalWindowLevel];
  }
}

- (IBAction)showComplement:(id)sender{
  if(usemini == 1){
    NSRect curframe = [[complementwindowcontroller window] frame];
    [[minicomplementwindowcontroller window] setFrameTopLeftPoint:NSMakePoint(curframe.origin.x, curframe.origin.y + curframe.size.height)];
    [minicomplementwindowcontroller showDialog];
    [complementwindowcontroller hideDialog];
  }else{
    NSRect curframe = [[minicomplementwindowcontroller window] frame];
    [[complementwindowcontroller window] setFrameTopLeftPoint:NSMakePoint(curframe.origin.x, curframe.origin.y + curframe.size.height)];
    [complementwindowcontroller showDialog];
    [minicomplementwindowcontroller hideDialog];
  }
}



- (IBAction)showASCII:(id)sender{
  bitShowASCIIWindow();
}


- (IBAction) switchMini:(id)sender{
  usemini = !usemini;

  naSetPreferencesBool(BitPrefs[UseMini], usemini);
  [complementwindowcontroller resetValue];
  [minicomplementwindowcontroller resetValue];
  [self update];
  [self showComplement:self];
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
