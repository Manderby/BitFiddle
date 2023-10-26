
#include "NABase/NABase.h"

#if NA_OS == NA_OS_WINDOWS
#if (NA_COMPILE_GUI == 1)



#include "NAUtility/NAMemory.h"
#include "NAApp.h"
#include "BitFiddleNSApplication.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "ASCIIController.h"
#include "BitFiddleApplication.h"
#include "ManderApp.h"
#include "ManderAppAbout.h"



void preStartup(void* arg){
  NA_UNUSED(arg);



  //naOpenConsoleWindow();

  //wchar_t buffer2[5120];
  //GetModuleFileName(NULL, buffer2, 5120);
  //printf("%ls\n", buffer2);

  //NAString* appName = naNewApplicationName();
  //printf("%s\n", naGetStringUTF8Pointer(appName));
  //naDelete(appName);

  //NAString* cwd = naNewStringWithCurWorkingDirectory();
  //printf("%s\n", naGetStringUTF8Pointer(cwd));
  //naDelete(cwd);



  NAString* appPath = naNewApplicationPath();

  //printf("%s\n", naGetStringUTF8Pointer(appPath));
  //while(1){}
  
  naSetApplicationName("Bit Fiddle");
  //naSetApplicationCompanyName("ASuppaCombbany");
  naSetApplicationVersionString("1.4");
  naSetApplicationBuildString("1.4");
  naSetApplicationResourcePath(naGetStringUTF8Pointer(appPath));
  NAString* iconPath = naNewStringWithFormat("%s%cicon.png", naGetStringUTF8Pointer(appPath), NA_PATH_DELIMITER_WIN);
  naSetApplicationIconPath(naGetStringUTF8Pointer(iconPath));

  naDelete(iconPath);
  naDelete(appPath);

  mandInitManderApp();
  bitStartApplication();
}



void postStartup(void* arg){
  NA_UNUSED(arg);

  mandCreateUI();
  mandSetAboutWindowStorageTag(BIT_WINDOW_TAG_ABOUT);

  bitCreateUI();
}



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd){
  NA_UNUSED(hInstance);
  NA_UNUSED(hPrevInstance);
  NA_UNUSED(lpCmdLine);
  NA_UNUSED(nShowCmd);
  //naOpenConsoleWindow();

  naStartRuntime();
  naStartApplication(preStartup, postStartup, NA_NULL);
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
