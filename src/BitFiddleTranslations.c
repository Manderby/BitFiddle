
#include "BitFiddleTranslations.h"

NAInt translatorGroup;

const NAUTF8Char* bitTranslate(NAInt id){
  return naTranslate(translatorGroup, id);
}

void initTranslations(){
  translatorGroup = naRegisterTranslatorGroup();
  #include "BitFiddleStrings_eng.h"
  #include "BitFiddleStrings_deu.h"
}
