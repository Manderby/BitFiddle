//
//  BitFiddlePreferencesController.h
//  Bit Fiddle
//
//  Created by Manderim on 07.08.13.
//
//

#import <Cocoa/Cocoa.h>

@interface BitFiddlePreferencesController : NSWindowController{
  IBOutlet NSTextField* atapplicationstartuplabel;
  IBOutlet NSButton* showasciiwindowatstartupcheckbox;
  IBOutlet NSButton* resetsettingsatstartupcheckbox;

  IBOutlet NSTextField* keepinfrontlabel;
  IBOutlet NSButton* keepmaxiontopcheckbox;
  IBOutlet NSButton* keepminiontopcheckbox;

  IBOutlet NSButton* donebutton;
}

- (void)awakeFromNib;
- (void)showDialog;
- (IBAction)changeShowASCIIatStartup:(id)sender;


@end
