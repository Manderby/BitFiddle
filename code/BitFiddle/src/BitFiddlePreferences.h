
#include "NAApp/NAPreferences.h"
#include "BitFiddleApplication.h"



void bitInitPreferences(void);



NABool bitGetPrefsAsciiUseEscape(void);
void   bitSetPrefsAsciiUseEscape(NABool use);

NABool bitGetPrefsAsciiUseHex(void);
void   bitSetPrefsAsciiUseHex(NABool use);



NABool bitGetPrefsShowAsciiOnStartup(void);
void   bitSetPrefsShowAsciiOnStartup(NABool show);

NABool bitGetPrefsResetConversionOnStartup(void);
void   bitSetPrefsResetConversionOnStartup(NABool reset);

NABool bitGetPrefsKeepConverterOnTop(void);
void   bitSetPrefsKeepConverterOnTop(NABool keepOnTop);



NABool bitGetPrefsShow16Bits(void);
void   bitSetPrefsShow16Bits(NABool show);

NABool bitGetPrefsShowNBits(void);
void   bitSetPrefsShowNBits(NABool show);

NABool bitGetPrefsShowBin(void);
void   bitSetPrefsShowBin(NABool show);

NABool bitGetPrefsShowAsc(void);
void   bitSetPrefsShowAsc(NABool show);



NABool bitGetPrefsSwapEndianness(void);
void   bitSetPrefsSwapEndianness(NABool swap);

BitConversionType bitGetPrefsComplementEncoding(void);
void bitSetPrefsComplementEncoding(BitConversionType encoding);




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
