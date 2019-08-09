
#include "NABase.h"
#include "BitFiddleCommon.h"

void bitInitApp(void);
void bitClearApp(void);

void bitShowConverterController(void);
void bitShowASCIIController(void);
void bitShowPreferencesController(void);
void bitUpdateApp(void);

ConversionType bitGetConversionType(void);
void bitSetConversionType(ConversionType conversionType);

NABool bitGetEndiannessSwap(void);
void bitSetEndiannessSwap(NABool swapEndianness);
