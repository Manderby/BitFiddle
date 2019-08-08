

#import "BitFiddleNSApplication.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIWindow.h"
#include "ConverterWindow.h"
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

  [self showComplement:self];

  NABool showASCIIOnStartup = naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]);
  if(showASCIIOnStartup){[self showASCII:self];}
}


- (void)applicationWillTerminate:(NSNotification *)notification{
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


- (IBAction)showComplement:(id)sender{
  bitShowConverterWindow();
}



- (IBAction)showASCII:(id)sender{
  bitShowASCIIWindow();
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
