//
//  BitFiddlePreferencesController.h
//  Bit Fiddle
//
//  Created by Manderim on 07.08.13.
//
//

#import <Cocoa/Cocoa.h>

@interface BitFiddlePreferencesController : NSWindowController{
  IBOutlet NSButton* showasciiwindowatstartupcheckbox;
  IBOutlet NSButton* donebutton;
}

- (void)awakeFromNib;
- (void)showDialog;
- (IBAction)pressDone:(id)sender;
- (IBAction)changeShowASCIIatStartup:(id)sender;


@end
