

#import <Cocoa/Cocoa.h>
#include "NAMemory.h"
#include "NAUI.h"
#include "BitFiddleApplication.h"
#include "BitFiddleTranslations.h"
#include "ASCIIWindow.h"



void updateApp(){
  [(BitFiddleApplication*)NSApp update];
}

void poststartup(void* arg){
  [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
}

int main(int argc, char *argv[]){
  naStartRuntime();
  [BitFiddleApplication sharedApplication];
  naStartApplication(NA_NULL, poststartup, NA_NULL);
  return 0;
}
