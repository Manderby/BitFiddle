
#import <Cocoa/Cocoa.h>

@class AboutWindowController;
@class ComplementWindowController;
@class ASCIIWindowController;

@interface BitFiddleApplication : NSApplication <NSApplicationDelegate> {
  IBOutlet AboutWindowController* aboutwindowcontroller;
  IBOutlet ComplementWindowController* complementwindowcontroller;
  IBOutlet ASCIIWindowController* asciiwindowcontroller;
}

- (id)init;

- (void)applicationDidFinishLaunching:(NSNotification *)notification;

- (IBAction) showAbout:(id)sender;
- (IBAction) showComplement:(id)sender;
- (IBAction) showASCII:(id)sender;

@end
