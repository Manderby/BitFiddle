
#include "BitFiddleApplication.h"



#if NA_OS == NA_OS_WINDOWS



#include <windows.h>

int WINAPI WinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR lpCmdLine,
  _In_ int nShowCmd)
{
  NA_UNUSED(hInstance);
  NA_UNUSED(hPrevInstance);
  NA_UNUSED(lpCmdLine);
  NA_UNUSED(nShowCmd);

  //naOpenConsoleWindow();

  naStartApplication(
    bitPreStartupApplication,
    bitPostStartupApplication,
    bitStopApplication,
    NA_NULL);

  return 0;
}



#elif NA_OS == NA_OS_MAC_OS_X



int main(int argc, char *argv[]) {
  NA_UNUSED(argc);
  NA_UNUSED(argv);

  naInstanciateNSApplication(BitFiddleNSApplication);
  
  naStartApplication(
    bitPreStartupApplication,
    bitPostStartupApplication,
    bitStopApplication,
    NA_NULL);
  
  return 0;
}



#endif // NA_OS



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
