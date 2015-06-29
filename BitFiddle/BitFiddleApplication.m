

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


- (void)applicationWillTerminate:(NSNotification *)notification{
  naStopRuntime();
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender{
  return TRUE;
}



- (IBAction) showPreferences:(id)sender{
  [preferenceswindowcontroller showDialog];
}



- (IBAction) showComplement:(id)sender{
  [complementwindowcontroller showDialog];
}



- (IBAction) showASCII:(id)sender{
  [asciiwindowcontroller showDialog];
}

@end
