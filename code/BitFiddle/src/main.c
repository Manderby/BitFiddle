
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
#include BIT_MANDERAPP_PATH(ManderAppAbout.h)



void prestartup(void* arg){
  NA_UNUSED(arg);

  naSetApplicationName("Bit Fiddle");
  //naSetApplicationCompanyName("ASuppaCombbany");
  naSetApplicationVersionString("1.5");
  naSetApplicationBuildString("Windows Beta 1");
  naSetApplicationIconPath("icon.png");

  mandInitManderApp();
  bitStartApplication();
}



void poststartup(void* arg){
  NA_UNUSED(arg);

  mandCreateUI();
  mandSetAboutWindowStorageTag(BIT_WINDOW_TAG_ABOUT);

  bitCreateUI();
}



#if NA_OS == NA_OS_WINDOWS
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    NA_UNUSED(hInstance);
    NA_UNUSED(hPrevInstance);
    NA_UNUSED(lpCmdLine);
    NA_UNUSED(nShowCmd);
    naOpenConsoleWindow();
#else
  int main(int argc, char *argv[]){
  NA_UNUSED(argc);
  NA_UNUSED(argv);
#endif

  naStartRuntime();
    //NAString* cwd = naNewStringWithCurWorkingDirectory();
    //printf("%s\n", naGetStringUTF8Pointer(cwd));
    //naDelete(cwd);

    naStartApplication(prestartup, poststartup, NA_NULL);
  naStopRuntime();
  return 0;
}



#endif // (NA_CONFIG_COMPILE_GUI == 1)
#endif // NA_OS == NA_OS_WINDOWS
