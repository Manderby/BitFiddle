
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

#include "NAUtility/NAMemory.h"

struct BitPreferencesController{
  NAWindow* window;
  
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
  bitUpdateApp();
}



void bit_PressDone(NAReaction reaction) {
  BitPreferencesController* con = reaction.controller;
  naCloseWindow(con->window);
}



BitPreferencesController* bitAllocPreferencesController() {
  BitPreferencesController* con = naAlloc(BitPreferencesController);

  con->window = naNewWindow(
    bitTranslate(BitFiddlePrefsWindowTitle),
    naMakeRectS(820, 15, 260, 236),
    0,
    BIT_WINDOW_TAG_PREFERENCES);
    
  NASpace* contentSpace = naGetWindowContentSpace(con->window);



  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), 330);
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
  naAddSpaceChild(
    contentSpace,
    con->onTopLabel,
    naMakePos(20., 118.));

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
    naMakePos(30., 96.));

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
    naMakePos(30., 74.));

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
    naMakePos(130., 74.));

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
    naMakePos(30., 52.));

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
    naMakePos(130., 52.));



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
    naMakePos(160., 20.));

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
