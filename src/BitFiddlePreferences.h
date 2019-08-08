
#include "NAPreferences.h"

enum {
  UseASCIIEscape,
  UseASCIIHex,

  ShowASCIIOnStartup,
  ResetConversionOnStartup,
  KeepConverterOnTop,

  SwapEndianness,
  SelectedComplementEncoding,
  
  BitPrefCount
};

extern const char* BitPrefs[BitPrefCount];

void initPreferences(void);
