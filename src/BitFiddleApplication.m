

#import "BitFiddleApplication.h"
#import "ComplementWindowController.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIWindow.h"
#include "PreferencesWindow.h"

@implementation BitFiddleApplication

- (id)init{
  self = [super init];
  [self setDelegate:self];
  switchingWindowMode = NA_FALSE;
  return self;
}



- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  [super applicationDidFinishLaunching:notification];

  [self setApplicationDescription:naTranslate(translatorGroup, BitFiddleApplicationDescription)];

  asciiWindow = createASCIIWindow();
  preferencesWindow = createPreferencesWindow();

  [NSBundle loadNibNamed:@"ComplementWindow" owner:self];
  
  [complementwindowcontroller setMini:NA_FALSE];
  [minicomplementwindowcontroller setMini:NA_TRUE];
  
  byteswap = mandGetUserDefaultBool("byteswap");
  conversiontype = (ConversionType)mandGetUserDefaultEnum("complementencoding");
  if(((int32)conversiontype < COMPUTE_UNSIGNED) || (int32)conversiontype > COMPUTE_TWOS_COMPLEMENT){conversiontype = 0;}
  usemini = mandGetUserDefaultBool("usemini");
  
  NABool resetsettings = naGetPreferencesBool(bitPrefResetConversionOnStartup);
  if(resetsettings){
    byteswap = NA_FALSE;
    conversiontype = COMPUTE_UNSIGNED;
  }
  
  [self update];
  [self showComplement:self];

  NABool showASCIIOnStartup = naGetPreferencesBool(bitPrefShowASCIIOnStartup);
  if(showASCIIOnStartup){[self showASCII:self];}

  NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
  NAString* lastrunversion = mandNewUserDefaultString("lastrunningversion");
  if(![versionstring isEqualToString:[NSString stringWithUTF8String:naGetStringUTF8Pointer(lastrunversion)]]){
    NAString* curversionstring = naNewStringWithFormat("%s", [versionstring UTF8String]);
    mandSetUserDefaultString(curversionstring, "lastrunningversion");
    naDelete(curversionstring);
    
    NSAlert* alert = [[NSAlert alloc] init];
    alert.alertStyle = NSInformationalAlertStyle;
    alert.messageText = [NSString stringWithFormat:@"Welcome to Version %@", versionstring];
    alert.informativeText = @"Enjoy the new dark mode on the latest macOS!\nSwitch to Mini and back using the green button.";
    [alert runModal];
    [alert release];
  }
  naDelete(lastrunversion);
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
  NABool keepMaxiOnTop = naGetPreferencesBool(bitPrefKeepMaxiOnTop);
  NABool keepMiniOnTop = naGetPreferencesBool(bitPrefKeepMiniOnTop);

  [byteswapmenuitem setState:byteswap ? NSOnState : NSOffState];
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

  if(usemini){
    mandSetUserDefaultInt(1, "usemini");
  }else{
    mandSetUserDefaultInt(0, "usemini");
  }
  [complementwindowcontroller resetValue];
  [minicomplementwindowcontroller resetValue];
  [self update];
  [self showComplement:self];
}


- (IBAction)switchByteSwap:(id)sender{
  byteswap = !byteswap;
  
  mandSetUserDefaultInt((NSInteger)byteswap, "byteswap");
  [self update];
  [complementwindowcontroller update];
  [minicomplementwindowcontroller update];
}


- (void)updateConversionType{
  mandSetUserDefaultEnum(conversiontype, "complementencoding");
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
  return byteswap;
}


- (ConversionType)conversiontype{
  return conversiontype;
}



@end
