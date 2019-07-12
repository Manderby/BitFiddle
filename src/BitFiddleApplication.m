

#import "BitFiddleApplication.h"
#import "MandAboutWindowController.h"
#import "ComplementWindowController.h"
#import "ASCIIWindowController.h"

#include "BitFiddleTranslations.h"
#include "NAUI.h"

@implementation BitFiddleApplication

- (id)init{
  self = [super init];
  [self setDelegate:self];
  switchingWindowMode = NA_FALSE;
  return self;
}



- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  NA_UNUSED(notification);

  translatorGroup = naRegisterTranslatorGroup();
  #include "BitFiddleStrings_eng.h"
  #include "BitFiddleStrings_deu.h"

  // Set the translator languages.
  NAInt lang = (NAInt)[[NSLocale preferredLanguages] count] - 1;
  while(lang >= 0){
    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)lang];
    NALanguageCode3 langcode = naGetLanguageCode([language UTF8String]);
    naSetTranslatorLanguagePreference(langcode);
    lang--;
  }

  [self setApplicationDescription:naTranslate(translatorGroup, BitFiddleApplicationDescription)];

  [NSBundle loadNibNamed:@"ComplementWindow" owner:self];
  [NSBundle loadNibNamed:@"Preferences" owner:self];
  
  [complementwindowcontroller setMini:NA_FALSE];
  [minicomplementwindowcontroller setMini:NA_TRUE];
  
  byteswap = mandGetUserDefaultBool("byteswap");
  conversiontype = (ConversionType)mandGetUserDefaultEnum("complementencoding");
  if(((int32)conversiontype < COMPUTE_UNSIGNED) || (int32)conversiontype > COMPUTE_TWOS_COMPLEMENT){conversiontype = 0;}
  usemini = mandGetUserDefaultBool("usemini");
  
  NABool resetsettings = mandGetUserDefaultBool("resetsettings");
  if(resetsettings){
    byteswap = NA_FALSE;
    conversiontype = COMPUTE_UNSIGNED;
  }
  
  [self update];
  [self showComplement:self];

  NABool showascii = mandGetUserDefaultBool("showascii");
  if(showascii){[self showASCII:self];}

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
  
  [preferenceswindowcontroller release];
  [complementwindowcontroller release];
  [minicomplementwindowcontroller release];
  [asciiwindowcontroller release];

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
  [preferenceswindowcontroller showDialog];
}


- (void)update{
  NABool keepmaxiontop = mandGetUserDefaultBool("keepmaxiontop");
  NABool keepminiontop = mandGetUserDefaultBool("keepminiontop");

  [byteswapmenuitem setState:byteswap ? NSOnState : NSOffState];
  [unsignedmenuitem setState:(conversiontype == COMPUTE_UNSIGNED)?NSOnState:NSOffState];
  [onescomplementmenuitem setState:(conversiontype == COMPUTE_ONES_COMPLEMENT)?NSOnState:NSOffState];
  [twoscomplementmenuitem setState:(conversiontype == COMPUTE_TWOS_COMPLEMENT)?NSOnState:NSOffState];
  [minimenuitem setState:usemini ? NSOnState : NSOffState];
  if(keepmaxiontop){
    [[complementwindowcontroller window] setLevel:NSFloatingWindowLevel];
  }else{
    [[complementwindowcontroller window] setLevel:NSNormalWindowLevel];
  }
  if(keepminiontop){
    [[minicomplementwindowcontroller window] setLevel:NSFloatingWindowLevel];
  }else{
    [[minicomplementwindowcontroller window] setLevel:NSNormalWindowLevel];
  }
}

- (NAInt)getTranslatorGroup{
  return translatorGroup;
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
  [asciiwindowcontroller showDialog];
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
