

#import <Cocoa/Cocoa.h>
#include "NAMemory.h"
#include "NAUI.h"
#include "BitFiddleApplication.h"
#include "BitFiddleTranslations.h"
#include "ASCIIWindow.h"



NAInt translatorGroup;

const char* bitPrefASCIIUseEscape = "ASCIIUseEscape";
const char* bitPrefASCIIUseHex = "ASCIIUseHex";

const char* bitPrefShowASCIIOnStartup = "showASCIIOnStartup";
const char* bitPrefResetConversionOnStartup = "resetConversionOnStartup";
const char* bitPrefKeepMaxiOnTop = "keepMaxiOnTop";
const char* bitPrefKeepMiniOnTop = "keepMiniOnTop";


void updateApp(){
  [(BitFiddleApplication*)NSApp update];
}

void prestartup(void* arg){
  translatorGroup = naRegisterTranslatorGroup();
  #include "BitFiddleStrings_eng.h"
  #include "BitFiddleStrings_deu.h"
}

void poststartup(void* arg){
  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
}

int main(int argc, char *argv[]){
  naStartRuntime();

//  return NSApplicationMain(argc, (const char **)argv);

  [BitFiddleApplication sharedApplication];
  naStartApplication(prestartup, poststartup, NA_NULL);
  return 0;
}
