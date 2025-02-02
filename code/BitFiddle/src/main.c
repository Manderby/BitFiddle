
#include "NABase/NABase.h"

#if NA_OS == NA_OS_WINDOWS
#if (NA_COMPILE_GUI == 1)



#include "NAUtility/NAMemory.h"
#include "NAApp/NAApp.h"
#include "BitFiddleNSApplication.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "ASCIIController.h"
#include "BitFiddleApplication.h"



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



  //NAString* appPath = naNewApplicationPath();

  //printf("%s\n", naGetStringUTF8Pointer(appPath));
  //while(1){}
  
  naSetApplicationName("Bit Fiddle");
  //naSetApplicationCompanyName("ASuppaCombbany");
  naSetApplicationVersionString("1.5");
  naSetApplicationBuildString("1.5");
  //naSetApplicationResourcePath(naGetStringUTF8Pointer(appPath));
  //NAString* iconPath = naNewStringWithFormat("%s%cicon.png", naGetStringUTF8Pointer(appPath), NA_PATH_DELIMITER_WIN);
  //naSetApplicationIconPath(naGetStringUTF8Pointer(iconPath));
  naSetApplicationIconPath("icon.png");

  //naDelete(iconPath);
  //naDelete(appPath);

  bitStartApplication();
}



void postStartup(void* arg){
  NA_UNUSED(arg);

  bitCreateUI();
}



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd){
  NA_UNUSED(hInstance);
  NA_UNUSED(hPrevInstance);
  NA_UNUSED(lpCmdLine);
  NA_UNUSED(nShowCmd);
  //naOpenConsoleWindow();

  naStartRuntime();
  naStartApplication(preStartup, postStartup, NA_NULL, NA_NULL);
  naStopRuntime();

  return 0;
}



#endif // (NA_COMPILE_GUI == 1)
#endif // NA_OS == NA_OS_WINDOWS



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
