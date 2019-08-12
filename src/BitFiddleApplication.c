
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"
#include "ConverterController.h"
#include "ASCIIController.h"
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "ManderAppAbout.h"
#include "ManderApp.h"



typedef struct BitApp BitApp;
struct BitApp{
  NABool swapEndianness;
  ConversionType conversionType;
  
  BitConverterController* converterController;
  BitASCIIController* asciiController;
  BitPreferencesController* preferencesController;
};



BitApp* bitApp = NA_NULL;



void bitStartApp(void){
  bitApp = naAlloc(BitApp);

  initTranslations();
  initPreferences();

  bitApp->swapEndianness = naGetPreferencesBool(BitPrefs[SwapEndianness]);
  bitApp->conversionType = (ConversionType)naGetPreferencesEnum(BitPrefs[SelectedComplementEncoding]);

  NABool resetsettings = naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]);
  if(resetsettings){
    bitApp->swapEndianness = NA_FALSE;
    bitApp->conversionType = COMPUTE_UNSIGNED;
  }
}



void bitCreateUI(){
  bitApp->converterController   = bitCreateConverterController();
  bitApp->asciiController       = bitCreateASCIIController();
  bitApp->preferencesController = bitCreatePreferencesController();
  
  naAddUIKeyboardShortcut(bitApp, naGetApplication(), NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_E, bitSwitchAppEndianness);
  naAddUIKeyboardShortcut(bitApp, naGetApplication(), NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_0, bitSwitchConversionType);
  naAddUIKeyboardShortcut(bitApp, naGetApplication(), NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_1, bitSwitchConversionType);
  naAddUIKeyboardShortcut(bitApp, naGetApplication(), NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_2, bitSwitchConversionType);

  bitShowConverterController();
  NABool showASCIIOnStartup = naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]);
  if(showASCIIOnStartup){bitShowASCIIController();}

  mandSetAboutDescription(bitTranslate(BitFiddleApplicationDescription));
  mandAlertNewVersion(bitTranslate(BitFiddleNewVersionDescription));
}



void bitStopApp(){
  bitClearConverterController(bitApp->converterController);
  bitClearASCIIController(bitApp->asciiController);
  bitClearPreferencesController(bitApp->preferencesController);
  
  naFree(bitApp);
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



NABool bitSwitchAppEndianness(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitApp* con = controller;
  
  con->swapEndianness = !con->swapEndianness;  
  naSetPreferencesBool(BitPrefs[SwapEndianness], con->swapEndianness);
  bitUpdateConverterController(con->converterController);
  return NA_TRUE;
}

NABool bitSwitchConversionType(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controller);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  NAUIKeyCode* keyCode = arg;

  switch(*keyCode){
  case NA_KEYCODE_0: bitSetConversionType(COMPUTE_UNSIGNED); break;
  case NA_KEYCODE_1: bitSetConversionType(COMPUTE_ONES_COMPLEMENT); break;
  case NA_KEYCODE_2: bitSetConversionType(COMPUTE_TWOS_COMPLEMENT); break;
  default:
    naError("Undefined keyCode");
  }
  return NA_TRUE;
}
