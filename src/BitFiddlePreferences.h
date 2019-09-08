
#include "NAPreferences.h"

enum {
  UseASCIIEscape,
  UseASCIIHex,

  ShowASCIIOnStartup,
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

extern const char* BitPrefs[BitPrefCount];

void initPreferences(void);
