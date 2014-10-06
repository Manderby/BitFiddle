
#import <Cocoa/Cocoa.h>
#import "ManderimApplication.h"

@class AboutWindowController;
@class BitFiddlePreferencesController;
@class ComplementWindowController;
@class ASCIIWindowController;

@interface BitFiddleApplication : ManderimApplication {
  IBOutlet BitFiddlePreferencesController* preferenceswindowcontroller;
  IBOutlet ComplementWindowController* complementwindowcontroller;
  IBOutlet ASCIIWindowController* asciiwindowcontroller;
}

- (id)init;

- (void)applicationDidFinishLaunching:(NSNotification *)notification;

- (IBAction) showPreferences:(id)sender;
- (IBAction) showComplement:(id)sender;
- (IBAction) showASCII:(id)sender;



@end
