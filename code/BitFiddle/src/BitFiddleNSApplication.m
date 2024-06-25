
#include "NABase/NABase.h"

#if NA_OS == NA_OS_MAC_OS_X

#import <Carbon/Carbon.h>

#import "BitFiddleNSApplication.h"

#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

#include "ASCIIController.h"
#include "ConverterController.h"
#include "PreferencesController.h"
#include "BitFiddleApplication.h"
#include "BitFiddleAboutController.h"


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



#endif // NA_OS == NA_OS_MAC_OS_X



// Copyright(c) Tobias Stamm
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this softwareand associated documentation files(the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and /or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
