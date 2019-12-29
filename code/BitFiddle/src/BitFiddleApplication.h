
#ifndef BIT_APPLICATION_DEFINED
#define BIT_APPLICATION_DEFINED

#include "BitFiddle.h"
#include BIT_NALIB_PATH(NABase.h)
#include BIT_NALIB_PATH(NAUI.h)
#include BIT_NALIB_PATH(NAEnvironment/NAUIImage.h)

typedef struct BitApplication BitApplication;

typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT = 1,
  COMPUTE_TWOS_COMPLEMENT = 2,
} ConversionType;

typedef enum{
  BIT_IMAGE_ASSET_HELP_BUTTON,
  BIT_IMAGE_ASSET_PREFS_BUTTON,
  BIT_IMAGE_ASSET_ASCII_BUTTON,
  BIT_IMAGE_ASSET_COUNT
} BitImageAsset;

#define BIT_WINDOW_TAG_CONVERTER 1
#define BIT_WINDOW_TAG_PREFERENCES 2
#define BIT_WINDOW_TAG_ASCII 3
#define BIT_WINDOW_TAG_ABOUT 4

void bitStartApplication(void);
void bitCreateUI(void);
void bitStopApplication(void);
BitApplication* bitGetApplication(void);

NAUIImage* bitGetImageAsset(BitImageAsset asset);

void bitShowConverterController(void);
void bitShowASCIIController(void);
void bitShowPreferencesController(void);
void bitUpdateApp(void);

ConversionType bitGetConversionType(void);
void bitSetConversionType(ConversionType conversionType);

NABool bitGetEndiannessSwap(void);
NABool bitSwitchAppEndianness(NAReaction reaction);
NABool bitSwitchConversionType(NAReaction reaction);

void bitRecreateConverterController(void);

#endif // BIT_APPLICATION_DEFINED
