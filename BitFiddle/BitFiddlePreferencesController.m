//
//  BitFiddlePreferencesController.m
//  Bit Fiddle
//
//  Created by Manderim on 07.08.13.
//
//

#import "BitFiddlePreferencesController.h"


@implementation BitFiddlePreferencesController


- (void)awakeFromNib{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  NSInteger showascii = [userdefaults integerForKey:@"showascii"];
  [showasciiwindowatstartupcheckbox setState:(showascii==1)?NSOnState:NSOffState];
}

- (void)showDialog{
  [[self window] makeKeyAndOrderFront:self];
}


- (IBAction)pressDone:(id)sender{
  [[self window] close];
}

- (IBAction)changeShowASCIIatStartup:(id)sender{
  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:([sender state] == NSOnState)?1:-1 forKey:@"showascii"];
}


@end
