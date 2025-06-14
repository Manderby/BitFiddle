
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

#include "NAUtility/NAMemory.h"

struct BitPreferencesController{
  NAWindow* window;
  
  NALabel* languageLabel;
  NASelect* languageSelect;
  NAMenuItem* languageSystem;
  NAMenuItem* languageDeutsch;
  NAMenuItem* languageEnglish;
  NAMenuItem* languageEspanol;
  NAMenuItem* languageFrancais;
  NAMenuItem* languageKlingon;
  NAMenuItem* languageJapanese;
  NAMenuItem* languageChinese;
  NAMenuItem* languageReport;

  NALabel* startupLabel;
  NACheckBox* showAsciiOnStartupCheckBox;
  NACheckBox* resetConversionOnStartupCheckBox;
  
  NALabel* onTopLabel;
  NACheckBox* keepConverterOnTopCheckBox;
  NACheckBox* show16BitsCheckBox;
  NACheckBox* showNBitsCheckBox;
  NACheckBox* showBinCheckBox;
  NACheckBox* showAscCheckBox;

  NAButton* doneButton;
};



void bit_SwitchPreferencesSetting(NAReaction reaction) {
  BitPreferencesController* con = reaction.controller;
  NABool state = naGetCheckBoxState(reaction.uiElement);
  if(reaction.uiElement == con->showAsciiOnStartupCheckBox) {
    bitSetPrefsShowAsciiOnStartup(state);
  }else if(reaction.uiElement == con->resetConversionOnStartupCheckBox) {
    bitSetPrefsResetConversionOnStartup(state);
  }else if(reaction.uiElement == con->keepConverterOnTopCheckBox) {
    bitSetPrefsKeepConverterOnTop(state);
  }else if(reaction.uiElement == con->show16BitsCheckBox) {
    bitSetPrefsShow16Bits(state);
    bitRecreateConverterController();
  }else if(reaction.uiElement == con->showNBitsCheckBox) {
    bitSetPrefsShowNBits(state);
    bitRecreateConverterController();
  }else if(reaction.uiElement == con->showBinCheckBox) {
    bitSetPrefsShowBin(state);
    bitRecreateConverterController();
  }else if(reaction.uiElement == con->showAscCheckBox) {
    bitSetPrefsShowAsc(state);
    bitRecreateConverterController();
  }else{
    #if NA_DEBUG
      naError("Unknown uiElement sent message");
    #endif
  }

  bitUpdatePreferencesController(con);
  bitUpdateAppConverterControllerOnTop();
}



void bit_PressDone(NAReaction reaction) {
  BitPreferencesController* con = reaction.controller;
  naCloseWindow(con->window);
}



void bit_ChangePreferencesLanguage(NAReaction reaction){
  BitPreferencesController* con = reaction.controller;

  if(reaction.uiElement == con->languageSystem){
    bitSetPrefsPreferredLanguage((NALanguageCode3)0);
  }else if(reaction.uiElement == con->languageDeutsch){
    bitSetPrefsPreferredLanguage(NA_LANG_DEU);
  }else if(reaction.uiElement == con->languageEnglish){
    bitSetPrefsPreferredLanguage(NA_LANG_ENG);
  }else if(reaction.uiElement == con->languageEspanol){
    bitSetPrefsPreferredLanguage(NA_LANG_SPA);
  }else if(reaction.uiElement == con->languageFrancais){
    bitSetPrefsPreferredLanguage(NA_LANG_FRA);
  }else if(reaction.uiElement == con->languageKlingon){
    bitSetPrefsPreferredLanguage(NA_LANG_TLH);
  }else if(reaction.uiElement == con->languageJapanese){
    bitSetPrefsPreferredLanguage(NA_LANG_JPN);
  }else if(reaction.uiElement == con->languageChinese){
    bitSetPrefsPreferredLanguage(NA_LANG_ZHO);
  }

  naPresentAlertBox(
    NA_ALERT_BOX_INFO,
    bitTranslate(BitLanguageChangeAlertTitle),
    bitTranslate(BitLanguageChangeAlertText));

  bitUpdatePreferencesController(con);
}



void bit_ReportBadTranslation(NAReaction reaction){
  BitPreferencesController* con = reaction.controller;

  naPresentAlertBox(
    NA_ALERT_BOX_INFO,
    bitTranslate(BitLanguageBadTranslationTitle),
    bitTranslate(BitLanguageBadTranslationText));

  bitUpdatePreferencesController(con);
}



BitPreferencesController* bitAllocPreferencesController() {
  BitPreferencesController* con = naAlloc(BitPreferencesController);

  NAFont* titleFont = bitGetTitleFont();

  con->window = naNewWindow(
    bitTranslate(BitFiddlePrefsWindowTitle),
    naMakeRectS(820, 15, 440, 278),
    0);
    
  naSetWindowStorageTag(con->window, BIT_WINDOW_TAG_PREFERENCES);

  NASpace* contentSpace = naGetWindowContentSpace(con->window);



  con->languageLabel = naNewLabel(bitTranslate(BitLanguage), 250);
  con->languageSelect = naNewSelect(150);
  con->languageSystem = naNewMenuItem(bitTranslate(BitLanguageSameAsSystem), NA_NULL);
  con->languageDeutsch = naNewMenuItem(bitTranslate(BitLanguageDeutsch), NA_NULL);
  con->languageEnglish = naNewMenuItem(bitTranslate(BitLanguageEnglish), NA_NULL);
  con->languageEspanol = naNewMenuItem(bitTranslate(BitLanguageEspanol), NA_NULL);
  con->languageFrancais = naNewMenuItem(bitTranslate(BitLanguageFrancais), NA_NULL);
  con->languageKlingon = naNewMenuItem(bitTranslate(BitLanguageKlingon), NA_NULL);
  con->languageJapanese = naNewMenuItem(bitTranslate(BitLanguageJapanese), NA_NULL);
  con->languageChinese = naNewMenuItem(bitTranslate(BitLanguageChinese), NA_NULL);
  con->languageReport = naNewMenuItem(bitTranslate(BitLanguageReport), NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageSystem, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, naNewMenuSeparator(), NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageDeutsch, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageEnglish, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageEspanol, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageFrancais, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageKlingon, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageJapanese, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageChinese, NA_NULL);
  naAddSelectMenuItem(con->languageSelect, naNewMenuSeparator(), NA_NULL);
  naAddSelectMenuItem(con->languageSelect, con->languageReport, NA_NULL);

  naAddSpaceChild(contentSpace, con->languageLabel, naMakePos(20, 236));
  naAddSpaceChild(contentSpace, con->languageSelect, naMakePos(270, 236));

  naAddUIReaction(con->languageSystem, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageDeutsch, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageEnglish, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageEspanol, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageFrancais, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageKlingon, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageJapanese, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);
  naAddUIReaction(con->languageChinese, NA_UI_COMMAND_PRESSED, bit_ChangePreferencesLanguage, con);

  naAddUIReaction(con->languageReport, NA_UI_COMMAND_PRESSED, bit_ReportBadTranslation, con);



  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), 330);
  naSetLabelFont(con->startupLabel, titleFont);
  naAddSpaceChild(
    contentSpace,
    con->startupLabel,
    naMakePos(20., 194.));

  con->showAsciiOnStartupCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsShowAscii),
    220);
  naAddUIReaction(
    con->showAsciiOnStartupCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->showAsciiOnStartupCheckBox,
    naMakePos(30., 172.));

  con->resetConversionOnStartupCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsResetSettings),
    220);
  naAddUIReaction(
    con->resetConversionOnStartupCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->resetConversionOnStartupCheckBox,
    naMakePos(30., 150.));



  con->onTopLabel = naNewLabel(
    bitTranslate(BitFiddlePrefsConverterWindow),
    230);
  naSetLabelFont(con->onTopLabel, titleFont);
  naAddSpaceChild(
    contentSpace,
    con->onTopLabel,
    naMakePos(20., 108.));

  con->keepConverterOnTopCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsKeepOnTop),
    220);
  naAddUIReaction(
    con->keepConverterOnTopCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->keepConverterOnTopCheckBox,
    naMakePos(30., 86.));

  con->show16BitsCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsShow16Bits),
    80);
  naAddUIReaction(
    con->show16BitsCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->show16BitsCheckBox,
    naMakePos(30., 64.));

  con->showNBitsCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsShowNBits),
    80);
  naAddUIReaction(
    con->showNBitsCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->showNBitsCheckBox,
    naMakePos(130., 64.));

  con->showBinCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsShowBin),
    80);
  naAddUIReaction(
    con->showBinCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->showBinCheckBox,
    naMakePos(230., 64.));

  con->showAscCheckBox = naNewCheckBox(
    bitTranslate(BitFiddlePrefsShowAsc),
    80);
  naAddUIReaction(
    con->showAscCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchPreferencesSetting,
    con);
  naAddSpaceChild(
    contentSpace,
    con->showAscCheckBox,
    naMakePos(330., 64.));



  con->doneButton = naNewTextPushButton(
    bitTranslate(BitFiddlePrefsDone),
    80);
  naAddUIReaction(
    con->doneButton,
    NA_UI_COMMAND_PRESSED,
    bit_PressDone,
    con);
  naAddSpaceChild(
    contentSpace,
    con->doneButton,
    naMakePos(440/2-80/2, 20.));

  naSetButtonSubmit(con->doneButton, bit_PressDone, con);
  naSetButtonAbort(con->doneButton, bit_PressDone, con);

  return con;
}



void bitDeallocPreferencesController(BitPreferencesController* con) {
  naFree(con);
}



void bitShowPreferencesController(BitPreferencesController* con) {
  bitUpdatePreferencesController(con);
  
  naShowWindow(con->window);
}



void bitUpdatePreferencesController(BitPreferencesController* con) {
  NALanguageCode3 languageCode = bitGetPrefsPreferredLanguage();

  switch(languageCode){
  case NA_LANG_DEU: naSetSelectItemSelected(con->languageSelect, con->languageDeutsch); break;
  case NA_LANG_ENG: naSetSelectItemSelected(con->languageSelect, con->languageEnglish); break;
  case NA_LANG_SPA: naSetSelectItemSelected(con->languageSelect, con->languageEspanol); break;
  case NA_LANG_FRA: naSetSelectItemSelected(con->languageSelect, con->languageFrancais); break;
  case NA_LANG_TLH: naSetSelectItemSelected(con->languageSelect, con->languageKlingon); break;
  case NA_LANG_JPN: naSetSelectItemSelected(con->languageSelect, con->languageJapanese); break;
  case NA_LANG_ZHO: naSetSelectItemSelected(con->languageSelect, con->languageChinese); break;
  default: naSetSelectItemSelected(con->languageSelect, con->languageSystem); break;
  }

  naSetCheckBoxState(
    con->showAsciiOnStartupCheckBox,
    bitGetPrefsShowAsciiOnStartup());
  naSetCheckBoxState(
    con->resetConversionOnStartupCheckBox,
    bitGetPrefsResetConversionOnStartup());
  naSetCheckBoxState(
    con->keepConverterOnTopCheckBox,
    bitGetPrefsKeepConverterOnTop());
  naSetCheckBoxState(
    con->show16BitsCheckBox,
    bitGetPrefsShow16Bits());
  naSetCheckBoxState(
    con->showNBitsCheckBox,
    bitGetPrefsShowNBits());
  naSetCheckBoxState(
    con->showBinCheckBox,
    bitGetPrefsShowBin());
  naSetCheckBoxState(
    con->showAscCheckBox,
    bitGetPrefsShowAsc());
}



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
