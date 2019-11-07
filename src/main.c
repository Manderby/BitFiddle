
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NABase.h)

#if NA_OS == NA_OS_WINDOWS
#if (NA_CONFIG_COMPILE_GUI == 1)



#include BIT_NALIB_PATH(NAMemory.h)
#include BIT_NALIB_PATH(NAUI.h)
#include "BitFiddleNSApplication.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "ASCIIController.h"
#include "BitFiddleApplication.h"
#include BIT_MANDERAPP_PATH(ManderApp.h)



void prestartup(void* arg){
  NA_UNUSED(arg);
  mandInitManderApp();
  bitStartApplication();
}



void poststartup(void* arg){
  NA_UNUSED(arg);
  mandCreateUI();
  bitCreateUI();
}



int main(int argc, char *argv[]){
  NA_UNUSED(argc);
  NA_UNUSED(argv);

  naStartRuntime();

  NAString* cwd = naNewStringWithCurWorkingDirectory();
  printf("%s\n", naGetStringUTF8Pointer(cwd));
  naDelete(cwd);

  naStartApplication(prestartup, poststartup, NA_NULL);
  return 0;
}



#endif // (NA_CONFIG_COMPILE_GUI == 1)
#endif // NA_OS == NA_OS_WINDOWS
