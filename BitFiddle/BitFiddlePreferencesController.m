
#import "BitFiddlePreferencesController.h"
#import "BitFiddleApplication.h"


@implementation BitFiddlePreferencesController


- (void)awakeFromNib{

  // Set the button text
  [atapplicationstartuplabel setStringValue:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsAtStartup" value:nil table:nil]];
  [showasciiwindowatstartupcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsShowASCII" value:nil table:nil]];
  [resetsettingsatstartupcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsResetSettings" value:nil table:nil]];

  [keepinfrontlabel setStringValue:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsKeepOnTop" value:nil table:nil]];
  [keepmaxiontopcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsMaxiVersion" value:nil table:nil]];
  [keepminiontopcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsMiniVersion" value:nil table:nil]];

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
