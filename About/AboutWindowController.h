
#import <Cocoa/Cocoa.h>


@interface AboutWindowController : NSWindowController {
  IBOutlet NSTextField* version;
  IBOutlet NSTextField* websitelink;
  IBOutlet NSTextField* manderclink;
}
- (void)awakeFromNib;
- (void)showDialog;
//- (IBAction)pressDone:(NSButton*)sender;
- (IBAction)goToWebsite:(id)sender;

@end


