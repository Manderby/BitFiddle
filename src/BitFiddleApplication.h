
#include "NABase.h"
#include "BitFiddleCommon.h"

void bitInitApp(void);
void bitShowComplementWindow(void);
void bitShowASCIIWindow(void);
void bitShowPreferencesWindow(void);

ConversionType bitGetConversionType(void);
void bitSetConversionType(ConversionType conversionType);

NABool bitGetEndiannessSwap(void);
void bitSetEndiannessSwap(NABool swapEndianness);
