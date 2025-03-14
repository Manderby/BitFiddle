
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
  
  BitConverterController* converterController;
  BitAsciiController* asciiController;
  BitPreferencesController* preferencesController;
  BitFiddleAboutController* aboutController;
};



BitApplication* bit_App = NA_NULL;



NAImageSet* bit_LoadImageAsset(
  const NAUTF8Char* dir,
  const NAUTF8Char* baseBame,
  const NAUTF8Char* suffix)
{
  NAString* imagePath = naNewApplicationResourcePath(
    dir,
    baseBame,
    suffix);
  NAImage* image = naCreateImageWithFilePath(
    naGetStringUTF8Pointer(imagePath));
  NAImageSet* imageSet = naCreateImageSet(
    image,
    NA_UI_RESOLUTION_2x,
    NA_BLEND_ERODE_LIGHT);
  naRelease(image);
  naDelete(imagePath);
  return imageSet;
}



void bitStartApplication(void) {
  bit_App = naAlloc(BitApplication);

  bitInitTranslations();
  bitInitPreferences();

  if(bitGetPrefsResetConversionOnStartup()) {
    bitSetPrefsSwapEndianness(NA_FALSE);
    bitSetPrefsComplementEncoding(COMPUTE_UNSIGNED);
  }

  bit_App->monoFont = naCreateFontWithPreset(
    NA_FONT_KIND_MONOSPACE,
    NA_FONT_SIZE_DEFAULT);

  bit_App->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON] =
    bit_LoadImageAsset(NA_NULL, "help", "png");
  bit_App->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON] =
    bit_LoadImageAsset(NA_NULL, "prefs", "png");
  bit_App->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON] =
    bit_LoadImageAsset(NA_NULL, "ascii", "png");
}



NAImageSet* bitGetImageAsset(BitImageAsset asset) {
  return bit_App->imageAssets[asset];
}

NAFont* bitGetMonospaceFont() {
  return bit_App->monoFont;
}



void bitCreateUI() {
  bit_App->converterController   = NA_NULL;
  bit_App->asciiController       = NA_NULL;
  bit_App->preferencesController = NA_NULL;
  bit_App->aboutController       = NA_NULL;
  
  bitRecreateConverterController();
  
  if(bitGetPrefsShowAsciiOnStartup()) {
    bitShowApplicationAsciiController();
  }
}



void bitStopApplication(void* data) {
  NA_UNUSED(data);
  
  if(bit_App->converterController) {
    bitDeallocConverterController(bit_App->converterController);
  }
  if(bit_App->asciiController) {
    bitDeallocAsciiController(bit_App->asciiController);
  }
  if(bit_App->preferencesController) {
    bitDeallocPreferencesController(bit_App->preferencesController);
  }
  if(bit_App->aboutController) {
    bitDeallocAboutController(bit_App->aboutController);
  }
  
  naRelease(bit_App->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON]);
  naRelease(bit_App->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON]);
  naRelease(bit_App->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON]);
  
  naRelease(bit_App->monoFont);
  
  naFree(bit_App);
}



void bitRecreateConverterController() {
  if(bit_App->converterController) {
    bitDeallocConverterController(bit_App->converterController);
  }
  bit_App->converterController = bitAllocConverterController();
  bitShowConverterController(bit_App->converterController);
  bitUpdateAppConverterControllerOnTop();
}

void bitUpdateAppConverterControllerOnTop() {
  bitKeepConverterOnTop(
    bit_App->converterController,
    bitGetPrefsKeepConverterOnTop());
}



void bitShowApplicationAsciiController() {
  if(!bit_App->asciiController) {
    bit_App->asciiController = bitAllocAsciiController();
  }
  bitShowAsciiController(bit_App->asciiController);
}

void bitShowApplicationPreferencesController() {
  if(!bit_App->preferencesController) {
    bit_App->preferencesController = bitAllocPreferencesController();
  }
  bitShowPreferencesController(bit_App->preferencesController);
}

void bitShowApplicationAboutController() {
  if(!bit_App->aboutController) {
    bit_App->aboutController = bitAllocAboutController();
  }
  bitShowAboutController(bit_App->aboutController);
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
