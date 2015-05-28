

#import "BitFiddleApplication.h"
#import "ManderimAboutWindowController.h"
#import "ComplementWindowController.h"
#import "ASCIIWindowController.h"

@implementation BitFiddleApplication

- (id)init{
  self = [super init];
  NAInt* price = malloc(sizeof(NAInt));
  *price = 2;
  [super setMetaData:price forIndex:MANDERIM_METADATA_PRICE];
  [self setDelegate:self];
  return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  NSInteger showascii = [userdefaults integerForKey:@"showascii"];
  if(!showascii){
    showascii = 1;
    [userdefaults setInteger:1 forKey:@"showcomplement"];
    [userdefaults synchronize];
  }
  if(showascii == 1){[self showASCII:self];}
  [self showComplement:self];
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender{
  return TRUE;
}


- (IBAction) showPreferences:(id)sender{
  [preferenceswindowcontroller showDialog];
}

- (IBAction) showComplement:(id)sender{
  [complementwindowcontroller showDialog];
//  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
//  [userdefaults setInteger:1 forKey:@"showcomplement"];
}

- (IBAction) showASCII:(id)sender{
  [asciiwindowcontroller showDialog];
//  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
//  [userdefaults setInteger:1 forKey:@"showascii"];
}

@end
