
#import <Carbon/Carbon.h>

#import "BitFiddleNSApplication.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIController.h"
#include "ConverterController.h"
#include "PreferencesController.h"
#include "BitFiddleAboutController.h"
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



// There are certain macOS specific menu items which should always be the same.
// Use a standard XIP file, connect the appropriate IBOutlets to these actions
// and all works fine. 

- (IBAction)openPreferences:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  bitShowApplicationPreferencesController();
}

- (IBAction)openAbout:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  bitShowApplicationAboutController();
}

- (IBAction)openHelp:(NSMenuItem*)sender{
  NA_UNUSED(sender);
  naOpenURLInBrowser(bitTranslate(BitFiddleApplicationHelpURL));
}

@end



void preStartup(void* arg){
  NA_UNUSED(arg);
  bitStartApplication();
}



void postStartup(void* arg){
  NA_UNUSED(arg);

  naLoadNib("MainMenu", NA_NULL);
  bitCreateUI();
}



int main(int argc, char *argv[]){
  NA_UNUSED(argc);
  NA_UNUSED(argv);
  naStartRuntime();
    [BitFiddleNSApplication sharedApplication];
    naStartApplication(
      preStartup,
      postStartup,
      bitStopApplication,
      NA_NULL);
  naStopRuntime();
  return 0;
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
