

#import <Cocoa/Cocoa.h>
#include "NARuntime.h"



int main(int argc, char *argv[]){
  naStartRuntime();
  int retval = NSApplicationMain(argc, (const char **)argv);

  return retval;
}
