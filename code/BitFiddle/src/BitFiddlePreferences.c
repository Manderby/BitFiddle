
#include "BitFiddlePreferences.h"



enum {
  PreferredLanguage,

  AsciiUseEscape,
  AsciiUseHex,

  ShowAsciiOnStartup,
  ResetConversionOnStartup,
  KeepConverterOnTop,
  
  Show16Bits,
  ShowNBits,
  ShowBin,
  ShowAsc,

  SwapEndianness,
  SelectedComplementEncoding,
  
  BitPrefCount
};

const char* BitPrefs[BitPrefCount] = {
  [PreferredLanguage]        = "preferredLanguage",

  [AsciiUseEscape]             = "useAsciiEscape",
  [AsciiUseHex]                = "useAsciiHex",

  [ShowAsciiOnStartup]         = "showAsciiOnStartup",
  [ResetConversionOnStartup]   = "resetConversionOnStartup",
  [KeepConverterOnTop]         = "keepConverterOnTop",

  [Show16Bits]                 = "show16Bits",
  [ShowNBits]                  = "showNBits",
  [ShowBin]                    = "showBin",
  [ShowAsc]                    = "showAsc",

  [SwapEndianness]             = "swapEndianness",
  [SelectedComplementEncoding] = "selectedComplementEncoding",
};



void bitInitPreferences(void) {
  naInitPreferencesi64(
    BitPrefs[PreferredLanguage],
    0,
    0,
    NA_MAX_i64);

  naInitPreferencesBool(BitPrefs[AsciiUseEscape], NA_FALSE);
  naInitPreferencesBool(BitPrefs[AsciiUseHex], NA_FALSE);

  naInitPreferencesBool(BitPrefs[ShowAsciiOnStartup], NA_FALSE);
  naInitPreferencesBool(BitPrefs[ResetConversionOnStartup], NA_TRUE);
  naInitPreferencesBool(BitPrefs[KeepConverterOnTop], NA_FALSE);

  naInitPreferencesBool(BitPrefs[Show16Bits], NA_TRUE);
  naInitPreferencesBool(BitPrefs[ShowNBits], NA_TRUE);
  naInitPreferencesBool(BitPrefs[ShowBin], NA_TRUE);
  naInitPreferencesBool(BitPrefs[ShowAsc], NA_TRUE);

  naInitPreferencesBool(BitPrefs[SwapEndianness], NA_FALSE);
  naInitPreferencesEnum(
    BitPrefs[SelectedComplementEncoding],
    COMPUTE_UNSIGNED,
    COMPUTE_BIT_CONVERSION_COUNT);
}



NALanguageCode3 bitGetPrefsPreferredLanguage(){
  return (NALanguageCode3)naGetPreferencesi64(BitPrefs[PreferredLanguage]);
}
void bitSetPrefsPreferredLanguage(NALanguageCode3 languageCode){
  naSetPreferencesi64(BitPrefs[PreferredLanguage], (int64)languageCode);
  naSetTranslatorLanguagePreference(languageCode);
}



NABool bitGetPrefsAsciiUseEscape() {
  return naGetPreferencesBool(BitPrefs[AsciiUseEscape]);
}
void bitSetPrefsAsciiUseEscape(NABool use) {
  naSetPreferencesBool(BitPrefs[AsciiUseEscape], use);
}

NABool bitGetPrefsAsciiUseHex() {
  return naGetPreferencesBool(BitPrefs[AsciiUseHex]);
}
void bitSetPrefsAsciiUseHex(NABool use) {
  naSetPreferencesBool(BitPrefs[AsciiUseHex], use);
}



NABool bitGetPrefsShowAsciiOnStartup() {
  return naGetPreferencesBool(BitPrefs[ShowAsciiOnStartup]);
}
void bitSetPrefsShowAsciiOnStartup(NABool show) {
  naSetPreferencesBool(BitPrefs[ShowAsciiOnStartup], show);
}

NABool bitGetPrefsResetConversionOnStartup() {
  return naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]);
}
void bitSetPrefsResetConversionOnStartup(NABool reset) {
  naSetPreferencesBool(BitPrefs[ResetConversionOnStartup], reset);
}

NABool bitGetPrefsKeepConverterOnTop() {
  return naGetPreferencesBool(BitPrefs[KeepConverterOnTop]);
}
void bitSetPrefsKeepConverterOnTop(NABool keepOnTop) {
  naSetPreferencesBool(BitPrefs[KeepConverterOnTop], keepOnTop);
}



NABool bitGetPrefsShow16Bits() {
  return naGetPreferencesBool(BitPrefs[Show16Bits]);
}
void bitSetPrefsShow16Bits(NABool show) {
  naSetPreferencesBool(BitPrefs[Show16Bits], show);
}

NABool bitGetPrefsShowNBits() {
  return naGetPreferencesBool(BitPrefs[ShowNBits]);
}
void bitSetPrefsShowNBits(NABool show) {
  naSetPreferencesBool(BitPrefs[ShowNBits], show);
}

NABool bitGetPrefsShowBin() {
  return naGetPreferencesBool(BitPrefs[ShowBin]);
}
void bitSetPrefsShowBin(NABool show) {
  naSetPreferencesBool(BitPrefs[ShowBin], show);
}

NABool bitGetPrefsShowAsc() {
  return naGetPreferencesBool(BitPrefs[ShowAsc]);
}
void bitSetPrefsShowAsc(NABool show) {
  naSetPreferencesBool(BitPrefs[ShowAsc], show);
}



NABool bitGetPrefsSwapEndianness() {
  return naGetPreferencesBool(BitPrefs[SwapEndianness]);
}
void bitSetPrefsSwapEndianness(NABool swap) {
  naSetPreferencesBool(BitPrefs[SwapEndianness], swap);
}

BitConversionType bitGetPrefsComplementEncoding() {
  return naGetPreferencesEnum(BitPrefs[SelectedComplementEncoding]);
}
void bitSetPrefsComplementEncoding(BitConversionType encoding) {
  naSetPreferencesEnum(BitPrefs[SelectedComplementEncoding], encoding);
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
