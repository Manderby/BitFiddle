//
//  BitFiddlePreferencesController.m
//  Bit Fiddle
//
//  Created by Manderim on 07.08.13.
//
//

#import "BitFiddlePreferencesController.h"
#import "BitFiddleApplication.h"
#import "ManderimApplicationStringConstants.h"


@implementation BitFiddlePreferencesController


- (void)awakeFromNib{

  // Set the button text
  [atapplicationstartuplabel setStringValue:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsAtStartup" value:nil table:nil]];
  [showasciiwindowatstartupcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsShowASCII" value:nil table:nil]];
  [resetsettingsatstartupcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsResetSettings" value:nil table:nil]];

  [keepinfrontlabel setStringValue:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsKeepOnTop" value:nil table:nil]];
  [keepmaxiontopcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsMaxiVersion" value:nil table:nil]];
  [keepminiontopcheckbox setTitle:[[NSBundle mainBundle] localizedStringForKey:@"BitFiddlePrefsMiniVersion" value:nil table:nil]];

  [donebutton setTitle:[[NSBundle mainBundle] localizedStringForKey:MANDERIM_APPLICATION_DONE_KEY value:nil table:@"ManderimApplication"]];


  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];

  NSInteger showascii = [userdefaults integerForKey:@"showascii"];
  [showasciiwindowatstartupcheckbox setState:(showascii==1)?NSOnState:NSOffState];
  NSInteger resetsettings = [userdefaults integerForKey:@"resetsettings"];
  [resetsettingsatstartupcheckbox setState:(resetsettings==1)?NSOnState:NSOffState];
  NSInteger keepmaxi = [userdefaults integerForKey:@"keepmaxiontop"];
  [keepmaxiontopcheckbox setState:keepmaxi?NSOnState:NSOffState];
  NSInteger keepmini = [userdefaults integerForKey:@"keepminiontop"];
  [keepminiontopcheckbox setState:keepmini?NSOnState:NSOffState];
}

- (void)showDialog{
  [[self window] makeKeyAndOrderFront:self];
}



- (IBAction)changeShowASCIIatStartup:(id)sender{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:([sender state] == NSOnState)?1:0 forKey:@"showascii"];
}


- (IBAction)changeResetSettingsAtStartup:(id)sender{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:([sender state] == NSOnState)?1:0 forKey:@"resetsettings"];
}


- (IBAction)changeKeepWindowOnTop:(id)sender{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  NABool keepmaxi = ([keepmaxiontopcheckbox state] == NSOnState)?NA_TRUE:NA_FALSE;
  NABool keepmini = ([keepminiontopcheckbox state] == NSOnState)?NA_TRUE:NA_FALSE;
  
  [userdefaults setInteger:(NSInteger)keepmaxi forKey:@"keepmaxiontop"];
  [userdefaults setInteger:(NSInteger)keepmini forKey:@"keepminiontop"];
  [(BitFiddleApplication*)NSApp update];
}


@end
