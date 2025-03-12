
#ifndef BIT_APPLICATION_DEFINED
#define BIT_APPLICATION_DEFINED

#include "NABase/NABase.h"
#include "NAApp/NAApp.h"

typedef struct BitApplication BitApplication;

typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT = 1,
  COMPUTE_TWOS_COMPLEMENT = 2,
  COMPUTE_BIT_CONVERSION_COUNT
} BitConversionType;

typedef enum{
  BIT_IMAGE_ASSET_HELP_BUTTON,
  BIT_IMAGE_ASSET_PREFS_BUTTON,
  BIT_IMAGE_ASSET_ASCII_BUTTON,
  BIT_IMAGE_ASSET_COUNT
} BitImageAsset;

#define BIT_WINDOW_TAG_CONVERTER 1
#define BIT_WINDOW_TAG_PREFERENCES 2
#define BIT_WINDOW_TAG_ASCII 3
#define BIT_WINDOW_TAG_ABOUT 4

void bitStartApplication(void);
void bitCreateUI(void);
void bitStopApplication(void* data);
BitApplication* bitGetApplication(void);

NAImageSet* bitGetImageAsset(BitImageAsset asset);

void bitShowApplicationConverterController(void);
void bitShowApplicationAsciiController(void);
void bitShowApplicationPreferencesController(void);
void bitShowApplicationAboutController(void);
void bitUpdateApp(void);

BitConversionType bitGetBitConversionType(void);
void bitSetBitConversionType(BitConversionType conversionType);

NABool bitGetEndiannessSwap(void);
void bitSwitchAppEndianness(NAReaction reaction);
void bitSwitchBitConversionType(NAReaction reaction);

void bitRecreateConverterController(void);

#endif // BIT_APPLICATION_DEFINED



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
