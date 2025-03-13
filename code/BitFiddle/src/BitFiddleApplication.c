
#include "BitFiddleApplication.h"
#include "BitFiddleAboutController.h"
#include "BitFiddlePreferences.h"
#include "ConverterController.h"
#include "AsciiController.h"
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"

#include "NAApp/NAImageSet.h"
#include "NAUtility/NAString.h"
#include "NAVisual/NAImage.h"



struct BitApplication{
  NAFont* monoFont;

  NAImageSet* imageAssets[BIT_IMAGE_ASSET_COUNT];
  
  NABool showAsc;
  
  BitConverterController* converterController;
  BitAsciiController* asciiController;
  BitPreferencesController* preferencesController;
  BitFiddleAboutController* aboutController;
};



BitApplication* bitApp = NA_NULL;



NAImageSet* bit_LoadImageAsset(const NAUTF8Char* dir, const NAUTF8Char* baseBame, const NAUTF8Char* suffix) {
  NAString* imagePath = naNewApplicationResourcePath(dir, baseBame, suffix);
  NAImage* image = naCreateImageWithFilePath(naGetStringUTF8Pointer(imagePath));
  NAImageSet* imageSet = naCreateImageSet(image, NA_UI_RESOLUTION_2x, NA_BLEND_ERODE_LIGHT);
  naRelease(image);
  naDelete(imagePath);
  return imageSet;
}



void bitStartApplication(void) {
  bitApp = naAlloc(BitApplication);

  bitInitTranslations();
  bitInitPreferences();

  if(bitGetPrefsResetConversionOnStartup()) {
    bitSetPrefsSwapEndianness(NA_FALSE);
    bitSetPrefsComplementEncoding(COMPUTE_UNSIGNED);
  }

  bitApp->monoFont = naCreateFontWithPreset(
    NA_FONT_KIND_MONOSPACE,
    NA_FONT_SIZE_DEFAULT);

  bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON] =  bit_LoadImageAsset(NA_NULL, "help", "png");
  bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON] = bit_LoadImageAsset(NA_NULL, "prefs", "png");
  bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON] = bit_LoadImageAsset(NA_NULL, "ascii", "png");
}



NAImageSet* bitGetImageAsset(BitImageAsset asset) {
  return bitApp->imageAssets[asset];
}



void bitCreateUI() {
  bitApp->converterController   = bitAllocConverterController();
  bitApp->asciiController       = NA_NULL;
  bitApp->preferencesController = NA_NULL;
  bitApp->aboutController       = NA_NULL;
  
  bitShowApplicationConverterController();
  
  if(bitGetPrefsShowAsciiOnStartup()) {
    bitShowApplicationAsciiController();
  }

  bitUpdateApp();
}



void bitStopApplication(void* data) {
  NA_UNUSED(data);
  
  bitDeallocConverterController(bitApp->converterController);
  if(bitApp->asciiController) {
    bitDeallocAsciiController(bitApp->asciiController);
  }
  bitDeallocPreferencesController(bitApp->preferencesController);
  bitDeallocAboutController(bitApp->aboutController);
  
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON]);
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON]);
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON]);
  
  naRelease(bitApp->monoFont);
  
  naFree(bitApp);
}



BitApplication* bitGetApplication(void) {
  return bitApp;
}



void bitShowApplicationConverterController() {
  bitShowConverterController(bitApp->converterController);
}
void bitShowApplicationAsciiController() {
  if(!bitApp->asciiController) {
    bitApp->asciiController = bitAllocAsciiController();
  }
  bitShowAsciiController(bitApp->asciiController);
}
void bitShowApplicationPreferencesController() {
  if(!bitApp->preferencesController) {
    bitApp->preferencesController = bitAllocPreferencesController();
  }
  bitShowPreferencesController(bitApp->preferencesController);
}
void bitShowApplicationAboutController() {
  if(!bitApp->aboutController) {
    bitApp->aboutController = bitAllocAboutController();
  }
  bitShowAboutController(bitApp->aboutController);
}



void bitRecreateConverterController() {
  bitDeallocConverterController(bitApp->converterController);
  bitApp->converterController = bitAllocConverterController();
  bitShowApplicationConverterController();
}



NAFont* bitGetMonospaceFont() {
  return bitApp->monoFont;
}



void bitUpdateApp() {
  bitKeepConverterOnTop(
    bitApp->converterController,
    bitGetPrefsKeepConverterOnTop());
}








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
