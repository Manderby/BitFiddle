
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"
#include "ConverterController.h"
#include "ASCIIController.h"
#include "PreferencesController.h"

typedef struct BitApp BitApp;
struct BitApp{
  NABool swapEndianness;
  ConversionType conversionType;
  
  BitConverterController* converterController;
  BitASCIIController* asciiController;
  BitPreferencesController* preferencesController;
};

BitApp* bitApp = NA_NULL;



void bitInitApp(void){
  bitApp = naAlloc(BitApp);

  bitApp->swapEndianness = naGetPreferencesBool(BitPrefs[SwapEndianness]);
  bitApp->conversionType = (ConversionType)naGetPreferencesEnum(BitPrefs[SelectedComplementEncoding]);

  NABool resetsettings = naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]);
  if(resetsettings){
    bitApp->swapEndianness = NA_FALSE;
    bitApp->conversionType = COMPUTE_UNSIGNED;
  }

  bitApp->converterController = bitCreateConverterController(NA_FALSE);
  bitApp->asciiController = bitCreateASCIIController();
  bitApp->preferencesController = bitCreatePreferencesController();
}



void bitClearApp(void){
  bitClearConverterController(bitApp->converterController);
  bitClearASCIIController(bitApp->asciiController);
  bitClearPreferencesController(bitApp->preferencesController);
  
  naFree(bitApp);
}



void bitShowConverterController(void){
  naShowConverterController(bitApp->converterController);
}
void bitShowASCIIController(void){
  naShowASCIIController(bitApp->asciiController);
}
void bitShowPreferencesController(void){
  naShowPreferencesController(bitApp->preferencesController);
}



void bitUpdateApp(void){
  NABool keepConverterOnTop = naGetPreferencesBool(BitPrefs[KeepConverterOnTop]);
  bitKeepConverterOnTop(bitApp->converterController, keepConverterOnTop);
}



ConversionType bitGetConversionType(void){
  return bitApp->conversionType;
}



void bitSetConversionType(ConversionType conversionType){
  bitApp->conversionType = conversionType;
  naSetPreferencesEnum(BitPrefs[SelectedComplementEncoding], conversionType);
  bitUpdateConverterController();
}



NABool bitGetEndiannessSwap(void){
  return bitApp->swapEndianness;
}



void bitSetEndiannessSwap(NABool swapEndianness){
  bitApp->swapEndianness = swapEndianness;  
  naSetPreferencesBool(BitPrefs[SwapEndianness], swapEndianness);
  bitUpdateConverterController();
}
