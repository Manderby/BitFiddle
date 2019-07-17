
#include "NATranslator.h"

extern NAInt translatorGroup;

#define BIT_TRANSLATE(id) naTranslate(translatorGroup, id)

enum{
  BitFiddleApplicationDescription,
  BitFiddleNewVersionDescription,

  // Expressions in Main Window
  BitFiddleConversionUnsigned,
  BitFiddleConversionOnesComp,
  BitFiddleConversionTwosComp,
  BitFiddleConversionByteSwap,

  // Expressions in Preferences window
  BitFiddlePrefsWindowTitle,    
  BitFiddlePrefsAtStartup,    
  BitFiddlePrefsShowASCII,    
  BitFiddlePrefsResetSettings,
  BitFiddlePrefsKeepOnTop,    
  BitFiddlePrefsMaxiVersion,  
  BitFiddlePrefsMiniVersion,  
  BitFiddlePrefsDone,

  // Additional information for ascii characters
  BitFiddleAdditionalInfo00,
  BitFiddleAdditionalInfo01,
  BitFiddleAdditionalInfo02,
  BitFiddleAdditionalInfo03,
  BitFiddleAdditionalInfo04,
  BitFiddleAdditionalInfo05,
  BitFiddleAdditionalInfo06,
  BitFiddleAdditionalInfo07,
  BitFiddleAdditionalInfo08,
  BitFiddleAdditionalInfo09,
  BitFiddleAdditionalInfo0a,
  BitFiddleAdditionalInfo0b,
  BitFiddleAdditionalInfo0c,
  BitFiddleAdditionalInfo0d,
  BitFiddleAdditionalInfo0e,
  BitFiddleAdditionalInfo0f,
  
  BitFiddleAdditionalInfo10,
  BitFiddleAdditionalInfo11,
  BitFiddleAdditionalInfo12,
  BitFiddleAdditionalInfo13,
  BitFiddleAdditionalInfo14,
  BitFiddleAdditionalInfo15,
  BitFiddleAdditionalInfo16,
  BitFiddleAdditionalInfo17,
  BitFiddleAdditionalInfo18,
  BitFiddleAdditionalInfo19,
  BitFiddleAdditionalInfo1a,
  BitFiddleAdditionalInfo1b,
  BitFiddleAdditionalInfo1c,
  BitFiddleAdditionalInfo1d,
  BitFiddleAdditionalInfo1e,
  BitFiddleAdditionalInfo1f,
  
  BitFiddleAdditionalInfo20,
  BitFiddleAdditionalInfo21,
  BitFiddleAdditionalInfo22,
  BitFiddleAdditionalInfo23,
  BitFiddleAdditionalInfo24,
  BitFiddleAdditionalInfo25,
  BitFiddleAdditionalInfo26,
  BitFiddleAdditionalInfo27,
  BitFiddleAdditionalInfo28,
  BitFiddleAdditionalInfo29,
  BitFiddleAdditionalInfo2a,
  BitFiddleAdditionalInfo2b,
  BitFiddleAdditionalInfo2c,
  BitFiddleAdditionalInfo2d,
  BitFiddleAdditionalInfo2e,
  BitFiddleAdditionalInfo2f,
  
  BitFiddleAdditionalInfo30,
  BitFiddleAdditionalInfo31,
  BitFiddleAdditionalInfo32,
  BitFiddleAdditionalInfo33,
  BitFiddleAdditionalInfo34,
  BitFiddleAdditionalInfo35,
  BitFiddleAdditionalInfo36,
  BitFiddleAdditionalInfo37,
  BitFiddleAdditionalInfo38,
  BitFiddleAdditionalInfo39,
  BitFiddleAdditionalInfo3a,
  BitFiddleAdditionalInfo3b,
  BitFiddleAdditionalInfo3c,
  BitFiddleAdditionalInfo3d,
  BitFiddleAdditionalInfo3e,
  BitFiddleAdditionalInfo3f,
  
  BitFiddleAdditionalInfo40,
  BitFiddleAdditionalInfo41,
  BitFiddleAdditionalInfo42,
  BitFiddleAdditionalInfo43,
  BitFiddleAdditionalInfo44,
  BitFiddleAdditionalInfo45,
  BitFiddleAdditionalInfo46,
  BitFiddleAdditionalInfo47,
  BitFiddleAdditionalInfo48,
  BitFiddleAdditionalInfo49,
  BitFiddleAdditionalInfo4a,
  BitFiddleAdditionalInfo4b,
  BitFiddleAdditionalInfo4c,
  BitFiddleAdditionalInfo4d,
  BitFiddleAdditionalInfo4e,
  BitFiddleAdditionalInfo4f,
  
  BitFiddleAdditionalInfo50,
  BitFiddleAdditionalInfo51,
  BitFiddleAdditionalInfo52,
  BitFiddleAdditionalInfo53,
  BitFiddleAdditionalInfo54,
  BitFiddleAdditionalInfo55,
  BitFiddleAdditionalInfo56,
  BitFiddleAdditionalInfo57,
  BitFiddleAdditionalInfo58,
  BitFiddleAdditionalInfo59,
  BitFiddleAdditionalInfo5a,
  BitFiddleAdditionalInfo5b,
  BitFiddleAdditionalInfo5c,
  BitFiddleAdditionalInfo5d,
  BitFiddleAdditionalInfo5e,
  BitFiddleAdditionalInfo5f,
  
  BitFiddleAdditionalInfo60,
  BitFiddleAdditionalInfo61,
  BitFiddleAdditionalInfo62,
  BitFiddleAdditionalInfo63,
  BitFiddleAdditionalInfo64,
  BitFiddleAdditionalInfo65,
  BitFiddleAdditionalInfo66,
  BitFiddleAdditionalInfo67,
  BitFiddleAdditionalInfo68,
  BitFiddleAdditionalInfo69,
  BitFiddleAdditionalInfo6a,
  BitFiddleAdditionalInfo6b,
  BitFiddleAdditionalInfo6c,
  BitFiddleAdditionalInfo6d,
  BitFiddleAdditionalInfo6e,
  BitFiddleAdditionalInfo6f,
  
  BitFiddleAdditionalInfo70,
  BitFiddleAdditionalInfo71,
  BitFiddleAdditionalInfo72,
  BitFiddleAdditionalInfo73,
  BitFiddleAdditionalInfo74,
  BitFiddleAdditionalInfo75,
  BitFiddleAdditionalInfo76,
  BitFiddleAdditionalInfo77,
  BitFiddleAdditionalInfo78,
  BitFiddleAdditionalInfo79,
  BitFiddleAdditionalInfo7a,
  BitFiddleAdditionalInfo7b,
  BitFiddleAdditionalInfo7c,
  BitFiddleAdditionalInfo7d,
  BitFiddleAdditionalInfo7e,
  BitFiddleAdditionalInfo7f,
};
