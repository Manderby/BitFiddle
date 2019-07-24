
#include "NAPreferences.h"

enum {
  UseASCIIEscape,
  UseASCIIHex,

  ShowASCIIOnStartup,
  ResetConversionOnStartup,
  KeepMaxiOnTop,
  KeepMiniOnTop,

  SwapEndianness,
  SelectedComplementEncoding,
  UseMini,
  
  BitPrefCount
};

extern const char* BitPrefs[BitPrefCount];

void initPreferences(void);
