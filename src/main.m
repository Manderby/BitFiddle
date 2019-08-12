

#import <Cocoa/Cocoa.h>
#include "NAMemory.h"
#include "NAUI.h"
#include "BitFiddleNSApplication.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "ASCIIController.h"
#include "BitFiddleApplication.h"
#include "ManderApp.h"



void prestartup(void* arg){
  NA_UNUSED(arg);
  mandInitManderApp();
  bitStartApp();
}



void poststartup(void* arg){
  NA_UNUSED(arg);
  mandCreateUI();
  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
  bitCreateUI();
}



int main(int argc, char *argv[]){
  NA_UNUSED(argc);
  NA_UNUSED(argv);
  naStartRuntime();
  [BitFiddleNSApplication sharedApplication];
  naStartApplication(prestartup, poststartup, NA_NULL);
  return 0;
}
