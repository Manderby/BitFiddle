

#import <Cocoa/Cocoa.h>
#include "NAMemory.h"
#include "NATranslator.h"



int main(int argc, char *argv[]){
  naStartRuntime();
  naStartTranslator();
  int retval = NSApplicationMain(argc, (const char **)argv);

  return retval;
}
