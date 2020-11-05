
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"
#include "ConverterController.h"
#include "ASCIIController.h"
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "ManderAppAbout.h"
#include "ManderApp.h"



struct BitApplication{
  NABool swapEndianness;
  ConversionType conversionType;
  NAUIImage* imageAssets[BIT_IMAGE_ASSET_COUNT];
  
  NABool showAsc;
  
  BitConverterController* converterController;
  BitASCIIController* asciiController;
  BitPreferencesController* preferencesController;
};



BitApplication* bitApp = NA_NULL;



NAUIImage* loadImageAsset(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NAString* imagePath = naNewApplicationResourcePath(dir, basename, suffix);
  NABabyImage* babyImage = naCreateBabyImageFromFilePath(naGetStringUTF8Pointer(imagePath));
  NAUIImage* uiimage = naNewUIImage(babyImage, NA_NULL, NA_UIIMAGE_RESOLUTION_2x, NA_BLEND_BLACK_GREEN);
  naReleaseBabyImage(babyImage);
  naDelete(imagePath);
  return uiimage;
}



void bitStartApplication(void){
  bitApp = naAlloc(BitApplication);

  initTranslations();
  initPreferences();

  bitApp->swapEndianness = naGetPreferencesBool(BitPrefs[SwapEndianness]);
  bitApp->conversionType = (ConversionType)naGetPreferencesEnum(BitPrefs[SelectedComplementEncoding]);

  NABool resetsettings = naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]);
  if(resetsettings){
    bitApp->swapEndianness = NA_FALSE;
    bitApp->conversionType = COMPUTE_UNSIGNED;
  }

  bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON] =  loadImageAsset(NA_NULL, "help", "png");
  bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON] = loadImageAsset(NA_NULL, "prefs", "png");
  bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON] = loadImageAsset(NA_NULL, "ascii", "png");
}



NAUIImage* bitGetImageAsset(BitImageAsset asset){
  return bitApp->imageAssets[asset];
}



void bitCreateUI(){
  bitApp->converterController   = bitCreateConverterController();
  bitApp->asciiController       = bitCreateASCIIController();
  bitApp->preferencesController = bitCreatePreferencesController();
  
  #if NA_OS == NA_OS_MAC_OS_X
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_E), bitSwitchAppEndianness, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_0), bitSwitchConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_1), bitSwitchConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_2), bitSwitchConversionType, bitApp);
  #elif NA_OS == NA_OS_WINDOWS
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_E), bitSwitchAppEndianness, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_0), bitSwitchConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_1), bitSwitchConversionType, bitApp);
    naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_CONTROL, NA_KEYCODE_2), bitSwitchConversionType, bitApp);
  #endif

  bitShowConverterController();
  NABool showASCIIOnStartup = naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]);
  if(showASCIIOnStartup){bitShowASCIIController();}

  mandSetAboutDescriptionAndHelpURL(bitTranslate(BitFiddleApplicationDescription), bitTranslate(BitFiddleApplicationHelpURL));
  mandAlertNewVersion(bitTranslate(BitFiddleNewVersionDescription));

  bitUpdateApp();
}



void bitStopApplication(){
  bitClearConverterController(bitApp->converterController);
  bitClearASCIIController(bitApp->asciiController);
  bitClearPreferencesController(bitApp->preferencesController);
  
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON]);
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON]);
  naRelease(bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON]);
  
  naFree(bitApp);
  naStopApplication();
}



BitApplication* bitGetApplication(void){
  return bitApp;
}



void bitShowConverterController(){
  naShowConverterController(bitApp->converterController);
}
void bitShowASCIIController(){
  naShowASCIIController(bitApp->asciiController);
}
void bitShowPreferencesController(){
  naShowPreferencesController(bitApp->preferencesController);
}



void bitRecreateConverterController(){
  bitClearConverterController(bitApp->converterController);
  bitApp->converterController = bitCreateConverterController();
  bitShowConverterController();
}



void bitUpdateApp(){
  NABool keepConverterOnTop = naGetPreferencesBool(BitPrefs[KeepConverterOnTop]);
  bitKeepConverterOnTop(bitApp->converterController, keepConverterOnTop);
}



ConversionType bitGetConversionType(){
  return bitApp->conversionType;
}



void bitSetConversionType(ConversionType conversionType){
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

NABool bitSwitchConversionType(NAReaction reaction){
  NA_UNUSED(reaction);
  NAKeyboardStatus keyboardStatus = naGetKeyboardStatus();
  switch(keyboardStatus.keyCode){
  case NA_KEYCODE_0: bitSetConversionType(COMPUTE_UNSIGNED); break;
  case NA_KEYCODE_1: bitSetConversionType(COMPUTE_ONES_COMPLEMENT); break;
  case NA_KEYCODE_2: bitSetConversionType(COMPUTE_TWOS_COMPLEMENT); break;
  default:
    #ifndef NDEBUG
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
