
#import "BitFiddlePreferencesController.h"
#import "BitFiddleApplication.h"
#include "BitFiddleTranslations.h"

@implementation BitFiddlePreferencesController


- (void)awakeFromNib{

  // Set the button text
  [atapplicationstartuplabel setStringValue:[NSString stringWithUTF8String:naTranslate(TR_BITFIDDLE, BitFiddlePrefsAtStartup)]];
  [showasciiwindowatstartupcheckbox setTitle:[NSString stringWithUTF8String:naTranslate(TR_BITFIDDLE, BitFiddlePrefsShowASCII)]];
  [resetsettingsatstartupcheckbox setTitle:[NSString stringWithUTF8String:naTranslate(TR_BITFIDDLE, BitFiddlePrefsResetSettings)]];

  [keepinfrontlabel setStringValue:[NSString stringWithUTF8String:naTranslate(TR_BITFIDDLE, BitFiddlePrefsKeepOnTop)]];
  [keepmaxiontopcheckbox setTitle:[NSString stringWithUTF8String:naTranslate(TR_BITFIDDLE, BitFiddlePrefsMaxiVersion)]];
  [keepminiontopcheckbox setTitle:[NSString stringWithUTF8String:naTranslate(TR_BITFIDDLE, BitFiddlePrefsMiniVersion)]];

  [donebutton setTitle:[[NSBundle mainBundle] localizedStringForKey:@"MandDone" value:nil table:@"MandApplication"]];


  NSInteger showascii = mandGetUserDefaultInt("showascii");
  [showasciiwindowatstartupcheckbox setState:(showascii==1)?NSOnState:NSOffState];
  NSInteger resetsettings = mandGetUserDefaultInt("resetsettings");
  [resetsettingsatstartupcheckbox setState:(resetsettings==1)?NSOnState:NSOffState];
  NSInteger keepmaxi = mandGetUserDefaultInt("keepmaxiontop");
  [keepmaxiontopcheckbox setState:keepmaxi?NSOnState:NSOffState];
  NSInteger keepmini = mandGetUserDefaultInt("keepminiontop");
  [keepminiontopcheckbox setState:keepmini?NSOnState:NSOffState];
}

- (void)showDialog{
  [[self window] makeKeyAndOrderFront:self];
}



- (IBAction)changeShowASCIIatStartup:(id)sender{
  mandSetUserDefaultInt(([sender state] == NSOnState)?1:0, "showascii");
}


- (IBAction)changeResetSettingsAtStartup:(id)sender{
  mandSetUserDefaultInt(([sender state] == NSOnState)?1:0, "resetsettings");
}


- (IBAction)changeKeepWindowOnTop:(id)sender{
  NABool keepmaxi = ([keepmaxiontopcheckbox state] == NSOnState)?NA_TRUE:NA_FALSE;
  NABool keepmini = ([keepminiontopcheckbox state] == NSOnState)?NA_TRUE:NA_FALSE;
  
  mandSetUserDefaultInt((NSInteger)keepmaxi, "keepmaxiontop");
  mandSetUserDefaultInt((NSInteger)keepmini, "keepminiontop");
  [(BitFiddleApplication*)NSApp update];
}


@end
