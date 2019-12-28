
#include "BitFiddleTranslations.h"

NAInt translatorGroup;

const NAUTF8Char* bitTranslate(NAInt id){
  return naTranslate(translatorGroup, id);
}

void initTranslations(){
  translatorGroup = naRegisterTranslatorGroup();
  #include "res/BitFiddleStrings_eng.h"
  #include "res/BitFiddleStrings_deu.h"
}
