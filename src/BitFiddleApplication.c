
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"
#include "ConverterWindow.h"
#include "ASCIIWindow.h"
#include "PreferencesWindow.h"

typedef struct BitApp BitApp;
struct BitApp{
  NABool swapEndianness;
  ConversionType conversionType;
  
  NAWindow* converterWindow;
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

  bitApp->converterWindow = bitCreateConverterWindow(NA_FALSE);
  bitApp->asciiWindow = bitCreateASCIIWindow();
  bitApp->preferencesWindow = bitCreatePreferencesWindow();
  
  bitShowConverterWindow();
}



void bitShowConverterWindow(void){
  naShowWindow(bitApp->converterWindow);
}
void bitShowASCIIWindow(void){
  naShowWindow(bitApp->asciiWindow);
}
void bitShowPreferencesWindow(void){
  naShowWindow(bitApp->preferencesWindow);
}



void bitUpdateApp(void){
  NABool keepConverterOnTop = naGetPreferencesBool(BitPrefs[KeepConverterOnTop]);
  naKeepWindowOnTop(bitApp->converterWindow, keepConverterOnTop);
}



ConversionType bitGetConversionType(void){
  return bitApp->conversionType;
}


void bitSetConversionType(ConversionType conversionType){
  bitApp->conversionType = conversionType;
  naSetPreferencesEnum(BitPrefs[SelectedComplementEncoding], conversionType);
  bitUpdateApp();
  bitUpdateConverterWindow();
}



NABool bitGetEndiannessSwap(void){
  return bitApp->swapEndianness;
}



void bitSetEndiannessSwap(NABool swapEndianness){
  bitApp->swapEndianness = swapEndianness;  
  naSetPreferencesBool(BitPrefs[SwapEndianness], swapEndianness);
  bitUpdateApp();
  bitUpdateConverterWindow();
}
