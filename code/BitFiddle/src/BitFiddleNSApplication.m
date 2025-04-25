
#include "NABase/NABase.h"

#if NA_OS == NA_OS_MAC_OS_X

#import <Cocoa/Cocoa.h>

#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"



@interface BitFiddleNSApplication : NSApplication <NSApplicationDelegate> {
}
@end

@implementation BitFiddleNSApplication

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
