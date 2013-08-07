
#import <Cocoa/Cocoa.h>

@class AboutWindowController;
@class BitFiddlePreferencesController;
@class ComplementWindowController;
@class ASCIIWindowController;

@interface BitFiddleApplication : NSApplication <NSApplicationDelegate> {
  IBOutlet AboutWindowController* aboutwindowcontroller;
  IBOutlet BitFiddlePreferencesController* preferenceswindowcontroller;
  IBOutlet ComplementWindowController* complementwindowcontroller;
  IBOutlet ASCIIWindowController* asciiwindowcontroller;
}

- (id)init;

- (void)applicationDidFinishLaunching:(NSNotification *)notification;

- (IBAction) showAbout:(id)sender;
- (IBAction) showPreferences:(id)sender;
- (IBAction) showComplement:(id)sender;
- (IBAction) showASCII:(id)sender;



@end
