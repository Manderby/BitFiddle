
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NABase.h)

#if NA_OS == NA_OS_MAC_OS_X
#if defined __OBJC__
#if (NA_CONFIG_COMPILE_GUI == 1)



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
  bitStartApplication();
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



#endif // (NA_CONFIG_COMPILE_GUI == 1)
#endif // defined __OBJC__
#endif // NA_OS == NA_OS_MAC_OS_X
