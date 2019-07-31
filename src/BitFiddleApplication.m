

#import "BitFiddleApplication.h"
#import "ComplementWindowController.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIWindow.h"
#include "PreferencesWindow.h"
#include "ManderAppAbout.h"

@implementation BitFiddleApplication

- (id)init{
  self = [super init];
  [self setDelegate:self];
  switchingWindowMode = NA_FALSE;
  return self;
}



- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  asciiWindow = createASCIIWindow();
  preferencesWindow = createPreferencesWindow();

  [NSBundle loadNibNamed:@"ComplementWindow" owner:self];
  
  [complementwindowcontroller setMini:NA_FALSE];
  [minicomplementwindowcontroller setMini:NA_TRUE];
  
//  [NSMenu setMenuBarVisible:NO];
  
  swapEndianness = naGetPreferencesBool(BitPrefs[SwapEndianness]);
  conversiontype = (ConversionType)naGetPreferencesEnum(BitPrefs[SelectedComplementEncoding]);
  if(((int32)conversiontype < COMPUTE_UNSIGNED) || (int32)conversiontype > COMPUTE_TWOS_COMPLEMENT){conversiontype = 0;}
  usemini = naGetPreferencesBool(BitPrefs[UseMini]);
  
  NABool resetsettings = naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]);
  if(resetsettings){
    swapEndianness = NA_FALSE;
    conversiontype = COMPUTE_UNSIGNED;
  }
  
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
//  return switchingWindowMode ? FALSE : TRUE;
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
  naShowWindow(preferencesWindow);
}


- (void)update{
  NABool keepMaxiOnTop = naGetPreferencesBool(BitPrefs[KeepMaxiOnTop]);
  NABool keepMiniOnTop = naGetPreferencesBool(BitPrefs[KeepMiniOnTop]);

  [byteswapmenuitem setState:swapEndianness ? NSOnState : NSOffState];
  [unsignedmenuitem setState:(conversiontype == COMPUTE_UNSIGNED)?NSOnState:NSOffState];
  [onescomplementmenuitem setState:(conversiontype == COMPUTE_ONES_COMPLEMENT)?NSOnState:NSOffState];
  [twoscomplementmenuitem setState:(conversiontype == COMPUTE_TWOS_COMPLEMENT)?NSOnState:NSOffState];
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
  switchingWindowMode = NA_TRUE;
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
  switchingWindowMode = NA_FALSE;
}



- (IBAction)showASCII:(id)sender{
  naShowWindow(asciiWindow);
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
  swapEndianness = !swapEndianness;
  
  naSetPreferencesBool(BitPrefs[SwapEndianness], swapEndianness);
  [self update];
  [complementwindowcontroller update];
  [minicomplementwindowcontroller update];
}


- (void)updateConversionType{
  naSetPreferencesEnum(BitPrefs[SelectedComplementEncoding], conversiontype);
  [self update];
  [complementwindowcontroller update];
  [minicomplementwindowcontroller update];
}


- (IBAction)switchToUnsigned:(id)sender{
  conversiontype = COMPUTE_UNSIGNED;
  [self updateConversionType];
}
- (IBAction)switchToOnesComplement:(id)sender{
  conversiontype = COMPUTE_ONES_COMPLEMENT;
  [self updateConversionType];
}
- (IBAction)switchToTwosComplement:(id)sender{
  conversiontype = COMPUTE_TWOS_COMPLEMENT;
  [self updateConversionType];
}


- (NABool)byteswap{
  return swapEndianness;
}


- (ConversionType)conversiontype{
  return conversiontype;
}

@end
