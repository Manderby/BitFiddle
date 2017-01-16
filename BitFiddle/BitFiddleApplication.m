

#import "BitFiddleApplication.h"
#import "ManderimAboutWindowController.h"
#import "ComplementWindowController.h"
#import "ASCIIWindowController.h"


@implementation BitFiddleApplication

- (id)init{
  self = [super init];
  [self setDelegate:self];
  return self;
}



- (void)applicationDidFinishLaunching:(NSNotification *)notification{

  [super applicationDidFinishLaunching:notification];

  [NSBundle loadNibNamed:@"ComplementWindow" owner:self];
  [NSBundle loadNibNamed:@"Preferences" owner:self];
  
  [complementwindowcontroller setMini:NA_FALSE];
  [minicomplementwindowcontroller setMini:NA_TRUE];
  
  NSURL* url = [[NSBundle mainBundle] URLForResource:@"index" withExtension:@"html" subdirectory:@"help"];
  [self setHelpDocument:url];

  
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  byteswap = [userdefaults integerForKey:@"byteswap"];
  conversiontype = (ConversionType)[userdefaults integerForKey:@"complementencoding"];
  usemini = [userdefaults integerForKey:@"usemini"];
  
  NABool resetsettings = (NABool)[userdefaults integerForKey:@"resetsettings"];
  if(resetsettings){
    byteswap = NA_FALSE;
    conversiontype = COMPUTE_UNSIGNED;
  }
  
  [self update];
  [self showComplement:self];

  NABool showascii = (NABool)[userdefaults integerForKey:@"showascii"];
  if(showascii){[self showASCII:self];}

  NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
//  if(1){
  if(![versionstring isEqualToString:[userdefaults stringForKey:@"lastrunningversion"]]){
    [userdefaults setObject:versionstring forKey:@"lastrunningversion"];
    NSAlert* alert = [[NSAlert alloc] init];
    alert.alertStyle = NSInformationalAlertStyle;
    alert.messageText = [NSString stringWithFormat:@"Welcome to Version %@", versionstring];
    alert.informativeText = @"Check out the new mini button!\nAnd have a look at the Preferences.";
//    [alert layout];
    [alert runModal];
    [alert release];
  }
}


- (void)applicationWillTerminate:(NSNotification *)notification{
  naStopRuntime();
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender{
  return TRUE;
}



- (IBAction) showPreferences:(id)sender{
  [preferenceswindowcontroller showDialog];
}


- (void)update{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  NABool keepmaxiontop = [userdefaults integerForKey:@"keepmaxiontop"];
  NABool keepminiontop = [userdefaults integerForKey:@"keepminiontop"];

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


- (IBAction) showComplement:(id)sender{
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



- (IBAction) showASCII:(id)sender{
  [asciiwindowcontroller showDialog];
}


- (IBAction) switchMini:(id)sender{
  usemini = !usemini;

  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  if(usemini){
    [userdefaults setInteger:1 forKey:@"usemini"];
  }else{
    [userdefaults setInteger:0 forKey:@"usemini"];
  }
  [complementwindowcontroller resetValue];
  [minicomplementwindowcontroller resetValue];
  [self update];
  [self showComplement:self];
}


- (IBAction)switchByteSwap:(id)sender{
  byteswap = !byteswap;
  
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:(NSInteger)byteswap forKey:@"byteswap"];
  [self update];
  [complementwindowcontroller update];
  [minicomplementwindowcontroller update];
}


- (void)updateConversionType{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:conversiontype forKey:@"complementencoding"];
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
