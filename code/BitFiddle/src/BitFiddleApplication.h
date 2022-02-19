
#ifndef BIT_APPLICATION_DEFINED
#define BIT_APPLICATION_DEFINED

#include <NABase.h>
#include "NAApp.h"

typedef struct BitApplication BitApplication;

typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT = 1,
  COMPUTE_TWOS_COMPLEMENT = 2,
} ConversionType;

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
void bitStopApplication(void);
BitApplication* bitGetApplication(void);

NAUIImage* bitGetImageAsset(BitImageAsset asset);

void bitShowConverterController(void);
void bitShowASCIIController(void);
void bitShowPreferencesController(void);
void bitUpdateApp(void);

ConversionType bitGetConversionType(void);
void bitSetConversionType(ConversionType conversionType);

NABool bitGetEndiannessSwap(void);
NABool bitSwitchAppEndianness(NAReaction reaction);
NABool bitSwitchConversionType(NAReaction reaction);

void bitRecreateConverterController(void);

#endif // BIT_APPLICATION_DEFINED



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
