
#ifndef BIT_APPLICATION_DEFINED
#define BIT_APPLICATION_DEFINED

#include "NABase.h"
#include "NAUI.h"

typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT = 1,
  COMPUTE_TWOS_COMPLEMENT = 2,
} ConversionType;

void bitStartApp(void);
void bitCreateUI(void);
void bitStopApp(void);

void bitShowConverterController(void);
void bitShowASCIIController(void);
void bitShowPreferencesController(void);
void bitUpdateApp(void);

ConversionType bitGetConversionType(void);
void bitSetConversionType(ConversionType conversionType);

NABool bitGetEndiannessSwap(void);
NABool bitSwitchAppEndianness(void* controller, NAUIElement* uielement, NAUICommand command, void* arg);
NABool bitSwitchConversionType(void* controller, NAUIElement* uielement, NAUICommand command, void* arg);

#endif // BIT_APPLICATION_DEFINED
