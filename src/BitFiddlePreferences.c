
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

const char* BitPrefs[BitPrefCount] = {
  "useASCIIEscape",
  "useASCIIHex",

  "showASCIIOnStartup",
  "resetConversionOnStartup",
  "keepConverterOnTop",

  "swapEndianness",
  "selectedSelectedComplementEncoding",
};

void initPreferences(void){
  naInitPreferencesBool(BitPrefs[UseASCIIEscape], NA_FALSE);
  naInitPreferencesBool(BitPrefs[UseASCIIHex], NA_FALSE);

  naInitPreferencesBool(BitPrefs[ShowASCIIOnStartup], NA_FALSE);
  naInitPreferencesBool(BitPrefs[ResetConversionOnStartup], NA_TRUE);
  naInitPreferencesBool(BitPrefs[KeepConverterOnTop], NA_FALSE);

  naInitPreferencesBool(BitPrefs[SwapEndianness], NA_FALSE);
  naInitPreferencesEnum(BitPrefs[SelectedComplementEncoding], COMPUTE_UNSIGNED);
}
