
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"
#include "ComplementWindow.h"
#include "ASCIIWindow.h"
#include "PreferencesWindow.h"

typedef struct BitApp BitApp;
struct BitApp{
  NABool swapEndianness;
  ConversionType conversionType;
  
  NAWindow* complementWindow;
  NAWindow* asciiWindow;
  NAWindow* preferencesWindow;
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

  bitApp->complementWindow = bitCreateComplementWindow(NA_FALSE);
  bitApp->asciiWindow = bitCreateASCIIWindow();
  bitApp->preferencesWindow = bitCreatePreferencesWindow();
  
  bitShowComplementWindow();
}



void bitShowComplementWindow(void){
  naShowWindow(bitApp->complementWindow);
}
void bitShowASCIIWindow(void){
  naShowWindow(bitApp->asciiWindow);
}
void bitShowPreferencesWindow(void){
  naShowWindow(bitApp->preferencesWindow);
}



void bitUpdateApp(void){

}



ConversionType bitGetConversionType(void){
  return bitApp->conversionType;
}


void bitSetConversionType(ConversionType conversionType){
  bitApp->conversionType = conversionType;
  naSetPreferencesEnum(BitPrefs[SelectedComplementEncoding], conversionType);
  bitUpdateApp();
  bitUpdateComplementWindow();
}



NABool bitGetEndiannessSwap(void){
  return bitApp->swapEndianness;
}



void bitSetEndiannessSwap(NABool swapEndianness){
  bitApp->swapEndianness = swapEndianness;  
  naSetPreferencesBool(BitPrefs[SwapEndianness], swapEndianness);
  bitUpdateApp();
  bitUpdateComplementWindow();
}
