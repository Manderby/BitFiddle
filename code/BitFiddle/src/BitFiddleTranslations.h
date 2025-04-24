
#include "NAUtility/NATranslator.h"

enum{
  BitFiddleAbout,
  BitFiddleDone,
  BitFiddleVersionBuild,
  BitFiddleOnlineHelp,
  BitFiddleApplicationHelpURL,

  // Expressions in Main Window
  BitFiddleConversionUnsigned,
  BitFiddleConversionOnesComp,
  BitFiddleConversionTwosComp,
  BitFiddleConversionByteSwap,

  // Expressions in Preferences window
  BitFiddlePrefsWindowTitle,    
  BitFiddlePrefsAtStartup,    
  BitFiddlePrefsShowAscii,    
  BitFiddlePrefsResetSettings,
  BitFiddlePrefsConverterWindow,    
  BitFiddlePrefsKeepOnTop,
  BitFiddlePrefsShow16Bits,
  BitFiddlePrefsShowNBits,
  BitFiddlePrefsShowBin,
  BitFiddlePrefsShowAsc,
  BitFiddlePrefsDone,

  // Additional information for ascii characters
  BitFiddleAsciiAdditionalInfo00,
  BitFiddleAsciiAdditionalInfo01,
  BitFiddleAsciiAdditionalInfo02,
  BitFiddleAsciiAdditionalInfo03,
  BitFiddleAsciiAdditionalInfo04,
  BitFiddleAsciiAdditionalInfo05,
  BitFiddleAsciiAdditionalInfo06,
  BitFiddleAsciiAdditionalInfo07,
  BitFiddleAsciiAdditionalInfo08,
  BitFiddleAsciiAdditionalInfo09,
  BitFiddleAsciiAdditionalInfo0a,
  BitFiddleAsciiAdditionalInfo0b,
  BitFiddleAsciiAdditionalInfo0c,
  BitFiddleAsciiAdditionalInfo0d,
  BitFiddleAsciiAdditionalInfo0e,
  BitFiddleAsciiAdditionalInfo0f,
  
  BitFiddleAsciiAdditionalInfo10,
  BitFiddleAsciiAdditionalInfo11,
  BitFiddleAsciiAdditionalInfo12,
  BitFiddleAsciiAdditionalInfo13,
  BitFiddleAsciiAdditionalInfo14,
  BitFiddleAsciiAdditionalInfo15,
  BitFiddleAsciiAdditionalInfo16,
  BitFiddleAsciiAdditionalInfo17,
  BitFiddleAsciiAdditionalInfo18,
  BitFiddleAsciiAdditionalInfo19,
  BitFiddleAsciiAdditionalInfo1a,
  BitFiddleAsciiAdditionalInfo1b,
  BitFiddleAsciiAdditionalInfo1c,
  BitFiddleAsciiAdditionalInfo1d,
  BitFiddleAsciiAdditionalInfo1e,
  BitFiddleAsciiAdditionalInfo1f,
  
  BitFiddleAsciiAdditionalInfo20,
  BitFiddleAsciiAdditionalInfo21,
  BitFiddleAsciiAdditionalInfo22,
  BitFiddleAsciiAdditionalInfo23,
  BitFiddleAsciiAdditionalInfo24,
  BitFiddleAsciiAdditionalInfo25,
  BitFiddleAsciiAdditionalInfo26,
  BitFiddleAsciiAdditionalInfo27,
  BitFiddleAsciiAdditionalInfo28,
  BitFiddleAsciiAdditionalInfo29,
  BitFiddleAsciiAdditionalInfo2a,
  BitFiddleAsciiAdditionalInfo2b,
  BitFiddleAsciiAdditionalInfo2c,
  BitFiddleAsciiAdditionalInfo2d,
  BitFiddleAsciiAdditionalInfo2e,
  BitFiddleAsciiAdditionalInfo2f,
  
  BitFiddleAsciiAdditionalInfo30,
  BitFiddleAsciiAdditionalInfo31,
  BitFiddleAsciiAdditionalInfo32,
  BitFiddleAsciiAdditionalInfo33,
  BitFiddleAsciiAdditionalInfo34,
  BitFiddleAsciiAdditionalInfo35,
  BitFiddleAsciiAdditionalInfo36,
  BitFiddleAsciiAdditionalInfo37,
  BitFiddleAsciiAdditionalInfo38,
  BitFiddleAsciiAdditionalInfo39,
  BitFiddleAsciiAdditionalInfo3a,
  BitFiddleAsciiAdditionalInfo3b,
  BitFiddleAsciiAdditionalInfo3c,
  BitFiddleAsciiAdditionalInfo3d,
  BitFiddleAsciiAdditionalInfo3e,
  BitFiddleAsciiAdditionalInfo3f,
  
  BitFiddleAsciiAdditionalInfo40,
  BitFiddleAsciiAdditionalInfo41,
  BitFiddleAsciiAdditionalInfo42,
  BitFiddleAsciiAdditionalInfo43,
  BitFiddleAsciiAdditionalInfo44,
  BitFiddleAsciiAdditionalInfo45,
  BitFiddleAsciiAdditionalInfo46,
  BitFiddleAsciiAdditionalInfo47,
  BitFiddleAsciiAdditionalInfo48,
  BitFiddleAsciiAdditionalInfo49,
  BitFiddleAsciiAdditionalInfo4a,
  BitFiddleAsciiAdditionalInfo4b,
  BitFiddleAsciiAdditionalInfo4c,
  BitFiddleAsciiAdditionalInfo4d,
  BitFiddleAsciiAdditionalInfo4e,
  BitFiddleAsciiAdditionalInfo4f,
  
  BitFiddleAsciiAdditionalInfo50,
  BitFiddleAsciiAdditionalInfo51,
  BitFiddleAsciiAdditionalInfo52,
  BitFiddleAsciiAdditionalInfo53,
  BitFiddleAsciiAdditionalInfo54,
  BitFiddleAsciiAdditionalInfo55,
  BitFiddleAsciiAdditionalInfo56,
  BitFiddleAsciiAdditionalInfo57,
  BitFiddleAsciiAdditionalInfo58,
  BitFiddleAsciiAdditionalInfo59,
  BitFiddleAsciiAdditionalInfo5a,
  BitFiddleAsciiAdditionalInfo5b,
  BitFiddleAsciiAdditionalInfo5c,
  BitFiddleAsciiAdditionalInfo5d,
  BitFiddleAsciiAdditionalInfo5e,
  BitFiddleAsciiAdditionalInfo5f,
  
  BitFiddleAsciiAdditionalInfo60,
  BitFiddleAsciiAdditionalInfo61,
  BitFiddleAsciiAdditionalInfo62,
  BitFiddleAsciiAdditionalInfo63,
  BitFiddleAsciiAdditionalInfo64,
  BitFiddleAsciiAdditionalInfo65,
  BitFiddleAsciiAdditionalInfo66,
  BitFiddleAsciiAdditionalInfo67,
  BitFiddleAsciiAdditionalInfo68,
  BitFiddleAsciiAdditionalInfo69,
  BitFiddleAsciiAdditionalInfo6a,
  BitFiddleAsciiAdditionalInfo6b,
  BitFiddleAsciiAdditionalInfo6c,
  BitFiddleAsciiAdditionalInfo6d,
  BitFiddleAsciiAdditionalInfo6e,
  BitFiddleAsciiAdditionalInfo6f,
  
  BitFiddleAsciiAdditionalInfo70,
  BitFiddleAsciiAdditionalInfo71,
  BitFiddleAsciiAdditionalInfo72,
  BitFiddleAsciiAdditionalInfo73,
  BitFiddleAsciiAdditionalInfo74,
  BitFiddleAsciiAdditionalInfo75,
  BitFiddleAsciiAdditionalInfo76,
  BitFiddleAsciiAdditionalInfo77,
  BitFiddleAsciiAdditionalInfo78,
  BitFiddleAsciiAdditionalInfo79,
  BitFiddleAsciiAdditionalInfo7a,
  BitFiddleAsciiAdditionalInfo7b,
  BitFiddleAsciiAdditionalInfo7c,
  BitFiddleAsciiAdditionalInfo7d,
  BitFiddleAsciiAdditionalInfo7e,
  BitFiddleAsciiAdditionalInfo7f,
};

const NAUTF8Char* bitTranslate(uint32 id);
void bitInitTranslations(void);



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
