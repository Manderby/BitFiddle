
#import <Carbon/Carbon.h>

#import "BitFiddleNSApplication.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIController.h"
#include "ConverterController.h"
#include "PreferencesController.h"
#include "BitFiddleApplication.h"
#include BIT_MANDERAPP_PATH(ManderAppAbout.h)
#include BIT_MANDERAPP_PATH(ManderApp.h)


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



void prestartup(void* arg){
  NA_UNUSED(arg);
  mandInitManderApp();
  bitStartApplication();
}



void poststartup(void* arg){
  NA_UNUSED(arg);

  mandCreateUI();
  mandSetAboutWindowStorageTag(BIT_WINDOW_TAG_ABOUT);

  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
  bitCreateUI();
}



int main(int argc, char *argv[]){
  NA_UNUSED(argc);
  NA_UNUSED(argv);
  naStartRuntime();
  [BitFiddleNSApplication sharedApplication];
  naStartApplication(prestartup, poststartup, NA_NULL);
  return 0;
}
