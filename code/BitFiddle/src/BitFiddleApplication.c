
#include "BitFiddleApplication.h"
#include "BitFiddleAboutController.h"
#include "BitFiddlePreferences.h"
#include "ConverterController.h"
#include "ASCIIController.h"
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"



struct BitApplication{
  NABool swapEndianness;
  BitConversionType conversionType;
  NAUIImage* imageAssets[BIT_IMAGE_ASSET_COUNT];
  
  NABool showAsc;
  
  BitConverterController* converterController;
  BitASCIIController* asciiController;
  BitPreferencesController* preferencesController;
  BitFiddleAboutController* aboutController;
};



BitApplication* bitApp = NA_NULL;



NAUIImage* bit_LoadImageAsset(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NAString* imagePath = naNewApplicationResourcePath(dir, basename, suffix);
  NABabyImage* babyImage = naCreateBabyImageFromFilePath(naGetStringUTF8Pointer(imagePath));
  NAUIImage* uiimage = naCreateUIImage(babyImage, NA_UIIMAGE_RESOLUTION_SCREEN_2x, NA_BLEND_ERODE_LIGHT);
  naReleaseBabyImage(babyImage);
  naDelete(imagePath);
  return uiimage;
}



void bitStartApplication(void){
  bitApp = naAlloc(BitApplication);

  bitInitTranslations();
  bitInitPreferences();

  bitApp->swapEndianness = naGetPreferencesBool(BitPrefs[SwapEndianness]);
  bitApp->conversionType = (BitConversionType)naGetPreferencesEnum(BitPrefs[SelectedComplementEncoding]);

  NABool resetsettings = naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]);
  if(resetsettings){
    bitApp->swapEndianness = NA_FALSE;
    bitApp->conversionType = COMPUTE_UNSIGNED;
  }

  bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON] =  bit_LoadImageAsset(NA_NULL, "help", "png");
  bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON] = bit_LoadImageAsset(NA_NULL, "prefs", "png");
  bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON] = bit_LoadImageAsset(NA_NULL, "ascii", "png");
}



NAUIImage* bitGetImageAsset(BitImageAsset asset){
  return bitApp->imageAssets[asset];
}



void bitCreateUI(){
  bitApp->converterController   = bitCreateConverterController();
  bitApp->asciiController       = bitCreateASCIIController();
  bitApp->preferencesController = bitCreatePreferencesController();
  bitApp->aboutController       = bitCreateAboutController();
  
  #if NA_OS == NA_OS_MAC_OS_X
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_E), bitSwitchAppEndianness, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_0), bitSwitchBitConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_1), bitSwitchBitConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_2), bitSwitchBitConversionType, bitApp);
  #elif NA_OS == NA_OS_WINDOWS
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_E), bitSwitchAppEndianness, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_0), bitSwitchBitConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_1), bitSwitchBitConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeyStroke(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_2), bitSwitchBitConversionType, bitApp);
  #endif

  bitShowApplicationConverterController();
  NABool showASCIIOnStartup = naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]);
  if(showASCIIOnStartup){bitShowApplicationASCIIController();}

  bitUpdateApp();
}



void bitStopApplication(void* data){
  NA_UNUSED(data);
  
  bitClearConverterController(bitApp->converterController);
  bitClearASCIIController(bitApp->asciiController);
  bitClearPreferencesController(bitApp->preferencesController);
  bitClearAboutController(bitApp->aboutController);
  
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON]);
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON]);
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON]);
  
  naFree(bitApp);
}



BitApplication* bitGetApplication(void){
  return bitApp;
}



void bitShowApplicationConverterController(){
  bitShowConverterController(bitApp->converterController);
}
void bitShowApplicationASCIIController(){
  bitShowASCIIController(bitApp->asciiController);
}
void bitShowApplicationPreferencesController(){
  bitShowPreferencesController(bitApp->preferencesController);
}
void bitShowApplicationAboutController(){
  bitShowAboutController(bitApp->aboutController);
}



void bitRecreateConverterController(){
  bitClearConverterController(bitApp->converterController);
  bitApp->converterController = bitCreateConverterController();
  bitShowApplicationConverterController();
}



void bitUpdateApp(){
  NABool keepConverterOnTop = naGetPreferencesBool(BitPrefs[KeepConverterOnTop]);
  bitKeepConverterOnTop(bitApp->converterController, keepConverterOnTop);
}



BitConversionType bitGetBitConversionType(){
  return bitApp->conversionType;
}



void bitSetBitConversionType(BitConversionType conversionType){
  bitApp->conversionType = conversionType;
  naSetPreferencesEnum(BitPrefs[SelectedComplementEncoding], conversionType);
  bitUpdateConverterController(bitApp->converterController);
}



NABool bitGetEndiannessSwap(){
  return bitApp->swapEndianness;
}



NABool bitSwitchAppEndianness(NAReaction reaction){
  BitApplication* con = reaction.controller;
  
  con->swapEndianness = !con->swapEndianness;  
  naSetPreferencesBool(BitPrefs[SwapEndianness], con->swapEndianness);
  bitUpdateConverterController(con->converterController);
  return NA_TRUE;
}

NABool bitSwitchBitConversionType(NAReaction reaction){
  NA_UNUSED(reaction);
  NAKeyStroke keyStroke = naGetCurrentKeyStroke();
  switch(keyStroke.keyCode){
  case NA_KEYCODE_0: bitSetBitConversionType(COMPUTE_UNSIGNED); break;
  case NA_KEYCODE_1: bitSetBitConversionType(COMPUTE_ONES_COMPLEMENT); break;
  case NA_KEYCODE_2: bitSetBitConversionType(COMPUTE_TWOS_COMPLEMENT); break;
  default:
    #if NA_DEBUG
      naError("Undefined keyCode");
    #endif
    break;
  }
  return NA_TRUE;
}



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
