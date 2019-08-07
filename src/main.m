

#import <Cocoa/Cocoa.h>
#include "NAMemory.h"
#include "NAUI.h"
#include "BitFiddleNSApplication.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "ASCIIWindow.h"
#include "ManderApp.h"
#include "ManderAppAbout.h"
#include "BitFiddleApplication.h"



void updateApp(){
  [(BitFiddleNSApplication*)NSApp update];
}

NABool pressKey(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  NAInt itemIndex = getUISpaceIndex(uielement);  
//  naSetSpaceAlternateBackground(spaces[itemIndex], NA_FALSE);
  return NA_TRUE;
}

void prestartup(void* arg){
  mandPrestartupManderApp();
//  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
//  NSMenu* appMenu = [[NSMenu alloc] initWithTitle:@"Schnucki"];
//  NSMenuItem* aboutItem = [[NSMenuItem alloc] initWithTitle:@"about" action:@selector(void) keyEquivalent:@"A"];
//  [appMenu addItem:aboutItem];
//  NSMenuItem* quitItem = [[NSMenuItem alloc] initWithTitle:@"quit" action:@selector(void) keyEquivalent:@"Q"];
//  [appMenu addItem:quitItem];
  
//  NSMenuItem* appItem = [[NSApp mainMenu] itemAtIndex:0];
//  [appItem setMenu:appMenu];
//  [(BitFiddleNSApplication*)NSApp setMainMenu:appMenu];
//  NSMenu* mainMenu = [NSApp mainMenu];
  initTranslations();
  initPreferences();
  
  bitInitApp();
}

void poststartup(void* arg){
  mandPoststartupManderApp();
////  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
//  NSMenu* appMenu = [[NSMenu alloc] initWithTitle:@"Schnucki"];
//  NSMenuItem* quitItem = [[NSMenuItem alloc] initWithTitle:@"quit" action:@selector(void) keyEquivalent:@"W"];
//  [appMenu addItem:quitItem];
//  
//  NSMenuItem* appItem = [[NSApp mainMenu] itemAtIndex:0];
//  [appItem setMenu:appMenu];
//  [NSApp setMainMenu:appMenu];
//  NSMenu* mainMenu = [NSApp mainMenu];
  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
  naAddUIReaction(NA_NULL, naGetApplication(), NA_UI_COMMAND_KEYUP, pressKey);

  mandSetAboutWindowDescription(bitTranslate(BitFiddleApplicationDescription));
  mandAlertNewVersion(bitTranslate(BitFiddleNewVersionDescription));
}

int main(int argc, char *argv[]){
  naStartRuntime();
  [BitFiddleNSApplication sharedApplication];
  naStartApplication(prestartup, poststartup, NA_NULL);
  return 0;
}
