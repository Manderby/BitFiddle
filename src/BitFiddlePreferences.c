
#include "BitFiddlePreferences.h"
#include "BitFiddleCommon.h"

const char* BitPrefs[BitPrefCount] = {
  "useASCIIEscape",
  "useASCIIHex",

  "showASCIIOnStartup",
  "resetConversionOnStartup",
  "keepMaxiOnTop",
  "keepMiniOnTop",

  "swapEndianness",
  "selectedSelectedComplementEncoding",
  "useMini"
};

void initPreferences(void){
  naInitPreferencesBool(BitPrefs[UseASCIIEscape], NA_FALSE);
  naInitPreferencesBool(BitPrefs[UseASCIIHex], NA_FALSE);

  naInitPreferencesBool(BitPrefs[ShowASCIIOnStartup], NA_FALSE);
  naInitPreferencesBool(BitPrefs[ResetConversionOnStartup], NA_TRUE);
  naInitPreferencesBool(BitPrefs[KeepMaxiOnTop], NA_FALSE);
  naInitPreferencesBool(BitPrefs[KeepMiniOnTop], NA_FALSE);

  naInitPreferencesBool(BitPrefs[SwapEndianness], NA_FALSE);
  naInitPreferencesEnum(BitPrefs[SelectedComplementEncoding], COMPUTE_UNSIGNED);
  naInitPreferencesBool(BitPrefs[UseMini], NA_FALSE);
}
