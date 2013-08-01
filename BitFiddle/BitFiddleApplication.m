

#import "BitFiddleApplication.h"
#import "AboutWindowController.h"
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
  BOOL showcomplement = [userdefaults integerForKey:@"showcomplement"];
  BOOL showascii = [userdefaults integerForKey:@"showascii"];
  if(!showcomplement && !showascii){showcomplement = YES;}
  if(showascii){[self showASCII:self];}
  if(showcomplement){[self showComplement:self];}
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender{
  return TRUE;
}

- (IBAction) showAbout:(id)sender{
  if(!aboutwindowcontroller){[NSBundle loadNibNamed:@"AboutWindow" owner:self];}
  [aboutwindowcontroller showDialog];
}

- (IBAction) showComplement:(id)sender{
  [complementwindowcontroller showDialog];
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:1 forKey:@"showcomplement"];
}

- (IBAction) showASCII:(id)sender{
  [asciiwindowcontroller showDialog];
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:1 forKey:@"showascii"];
}

@end
