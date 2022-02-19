
#include "NABase.h"

#if NA_OS == NA_OS_WINDOWS
#if (NA_COMPILE_GUI == 1)



#include "NAMemory.h"
#include "NAApp.h"
#include "BitFiddleNSApplication.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "ASCIIController.h"
#include "BitFiddleApplication.h"
#include "ManderApp.h"
#include "ManderAppAbout.h"



void prestartup(void* arg){
  NA_UNUSED(arg);

  naSetApplicationName("Bit Fiddle");
  //naSetApplicationCompanyName("ASuppaCombbany");
  naSetApplicationVersionString("1.4");
  naSetApplicationBuildString("1.4");
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
    //naOpenConsoleWindow();
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



#endif // (NA_COMPILE_GUI == 1)
#endif // NA_OS == NA_OS_WINDOWS



  // Copyright(c) Tobias Stamm
  // 
  // Permission is hereby granted, free of charge, to any person obtaining a
  // copy of this softwareand associated documentation files(the "Software"),
  // to deal in the Software without restriction, including without limitation
  // the rights to use, copy, modify, merge, publish, distribute, sublicense,
  // and /or sell copies of the Software, and to permit persons to whom the
  // Software is furnished to do so, subject to the following conditions :
  // 
  // The above copyright noticeand this permission notice shall be included in
  // all copies or substantial portions of the Software.
  // 
  // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
  // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  // FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  // DEALINGS IN THE SOFTWARE.
