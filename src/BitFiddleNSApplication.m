
#import <Carbon/Carbon.h>

#import "BitFiddleNSApplication.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIController.h"
#include "ConverterController.h"
#include "PreferencesController.h"
#include "ManderAppAbout.h"
#include "BitFiddleApplication.h"


@implementation BitFiddleNSApplication



// This method makes sure the notification methods will be called.
- (id)init{
  self = [super init];
  [self setDelegate:self];
  return self;
}



// This method will still be called. Use it if necessary.
- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  NA_UNUSED(notification);
}



// This method is used to properly shut down the app and NALib runtime.
- (void)applicationWillTerminate:(NSNotification *)notification{
  NA_UNUSED(notification);
  bitStopApplication();
}



// There are certain macOS specific menu items which should always be the same.
// Use a standard XIP file, connect the appropriate IBOutlets to these actions
// and all works fine. 

- (IBAction)openPreferences:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  bitShowPreferencesController();
}

- (IBAction)openAbout:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  mandShowAboutController();
}

- (IBAction)openHelp:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  naOpenURLInBrowser(bitTranslate(BitFiddleApplicationHelpURL));
}

@end
