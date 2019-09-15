
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
  NABabyImage* babyImage = naAllocBabyImageFromFilePath(naGetStringUTF8Pointer(imagePath));
  NAUIImage* uiimage = naAllocUIImage(babyImage, NA_NULL, NA_UIIMAGE_RESOLUTION_2x, NA_BLEND_BLACK_GREEN);
  naDeallocBabyImage(babyImage);
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
  
  naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_E), bitSwitchAppEndianness, bitApp);
  naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_0), bitSwitchConversionType, bitApp);
  naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_1), bitSwitchConversionType, bitApp);
  naAddUIKeyboardShortcut(naGetApplication(), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_2), bitSwitchConversionType, bitApp);

  bitShowConverterController();
  NABool showASCIIOnStartup = naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]);
  if(showASCIIOnStartup){bitShowASCIIController();}

  mandSetAboutDescriptionAndHelpURL(bitTranslate(BitFiddleApplicationDescription), bitTranslate(BitFiddleApplicationHelpURL));
  mandAlertNewVersion(bitTranslate(BitFiddleNewVersionDescription));
}



void bitStopApplication(){
  bitClearConverterController(bitApp->converterController);
  bitClearASCIIController(bitApp->asciiController);
  bitClearPreferencesController(bitApp->preferencesController);
  
  naDeallocUIImage(bitApp->imageAssets[BIT_IMAGE_ASSET_HELP_BUTTON]);
  naDeallocUIImage(bitApp->imageAssets[BIT_IMAGE_ASSET_PREFS_BUTTON]);
  naDeallocUIImage(bitApp->imageAssets[BIT_IMAGE_ASSET_ASCII_BUTTON]);
  
  naFree(bitApp);
  naStopApplication();
  naStopRuntime();
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
