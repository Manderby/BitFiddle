
#include "NABase.h"
#include "BitFiddleCommon.h"

void bitInitApp(void);
void bitShowConverterWindow(void);
void bitShowASCIIWindow(void);
void bitShowPreferencesWindow(void);
void bitUpdateApp(void);

ConversionType bitGetConversionType(void);
void bitSetConversionType(ConversionType conversionType);

NABool bitGetEndiannessSwap(void);
void bitSetEndiannessSwap(NABool swapEndianness);
