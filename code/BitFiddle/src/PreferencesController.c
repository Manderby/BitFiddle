
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

#include "NAUtility/NAMemory.h"

struct BitPreferencesController{
  NAWindow* window;
  
  NALabel* startupLabel;
  NACheckBox* showASCIIOnStartupCheckBox;
  NACheckBox* resetConversionOnStartupCheckBox;
  
  NALabel* onTopLabel;
  NACheckBox* keepConverterOnTopCheckBox;
  NACheckBox* show16BitsCheckBox;
  NACheckBox* showNBitsCheckBox;
  NACheckBox* showBinCheckBox;
  NACheckBox* showAscCheckBox;

  NAButton* doneButton;
};



void bit_SwitchPreferencesSetting(NAReaction reaction){
  BitPreferencesController* prefs = reaction.controller;
  NABool state = naGetCheckBoxState(reaction.uiElement);
  if(reaction.uiElement == prefs->showASCIIOnStartupCheckBox){
    naSetPreferencesBool(BitPrefs[ShowASCIIOnStartup], state);
  }else if(reaction.uiElement == prefs->resetConversionOnStartupCheckBox){
    naSetPreferencesBool(BitPrefs[ResetConversionOnStartup], state);
  }else if(reaction.uiElement == prefs->keepConverterOnTopCheckBox){
    naSetPreferencesBool(BitPrefs[KeepConverterOnTop], state);
  }else if(reaction.uiElement == prefs->show16BitsCheckBox){
    naSetPreferencesBool(BitPrefs[Show16Bits], state);
    bitRecreateConverterController();
  }else if(reaction.uiElement == prefs->showNBitsCheckBox){
    naSetPreferencesBool(BitPrefs[ShowNBits], state);
    bitRecreateConverterController();
  }else if(reaction.uiElement == prefs->showBinCheckBox){
    naSetPreferencesBool(BitPrefs[ShowBin], state);
    bitRecreateConverterController();
  }else if(reaction.uiElement == prefs->showAscCheckBox){
    naSetPreferencesBool(BitPrefs[ShowAsc], state);
    bitRecreateConverterController();
  }else{
    #if NA_DEBUG
      naError("Unknown uiElement sent message");
    #endif
  }
  bitUpdateApp();
}



void bit_PressDone(NAReaction reaction){
  BitPreferencesController* prefs = reaction.controller;
  naCloseWindow(prefs->window);
}



BitPreferencesController* bitAllocPreferencesController(){
  BitPreferencesController* con = naAlloc(BitPreferencesController);

  NARect windowrect = naMakeRectS(820, 15, 260, 236);
  con->window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE, BIT_WINDOW_TAG_PREFERENCES);
  NASpace* space = naGetWindowContentSpace(con->window);

  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), 330);
  naAddSpaceChild(space, con->startupLabel, naMakePos(20., 194.));

  con->showASCIIOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowASCII), 220);
  naAddUIReaction(con->showASCIIOnStartupCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->showASCIIOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, con->showASCIIOnStartupCheckBox, naMakePos(30., 172.));

  con->resetConversionOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsResetSettings), 220);
  naAddUIReaction(con->resetConversionOnStartupCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->resetConversionOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, con->resetConversionOnStartupCheckBox, naMakePos(30., 150.));

  con->onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsConverterWindow), 230);
  naAddSpaceChild(space, con->onTopLabel, naMakePos(20., 118.));

  con->keepConverterOnTopCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsKeepOnTop), 220);
  naAddUIReaction(con->keepConverterOnTopCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->keepConverterOnTopCheckBox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->keepConverterOnTopCheckBox, naMakePos(30., 96.));

  con->show16BitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShow16Bits), 80);
  naAddUIReaction(con->show16BitsCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->show16BitsCheckBox, naGetPreferencesBool(BitPrefs[Show16Bits]));
  naAddSpaceChild(space, con->show16BitsCheckBox, naMakePos(30., 74.));

  con->showNBitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowNBits), 80);
  naAddUIReaction(con->showNBitsCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->showNBitsCheckBox, naGetPreferencesBool(BitPrefs[ShowNBits]));
  naAddSpaceChild(space, con->showNBitsCheckBox, naMakePos(130., 74.));

  con->showBinCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowBin), 80);
  naAddUIReaction(con->showBinCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->showBinCheckBox, naGetPreferencesBool(BitPrefs[ShowBin]));
  naAddSpaceChild(space, con->showBinCheckBox, naMakePos(30., 52.));

  con->showAscCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowAsc), 80);
  naAddUIReaction(con->showAscCheckBox, NA_UI_COMMAND_PRESSED, bit_SwitchPreferencesSetting, con);
  naSetCheckBoxState(con->showAscCheckBox, naGetPreferencesBool(BitPrefs[ShowAsc]));
  naAddSpaceChild(space, con->showAscCheckBox, naMakePos(130., 52.));

  con->doneButton = naNewTextPushButton(bitTranslate(BitFiddlePrefsDone), 80);
  naAddUIReaction(con->doneButton, NA_UI_COMMAND_PRESSED, bit_PressDone, con);
  naAddSpaceChild(space, con->doneButton, naMakePos(160., 20.));

  naSetButtonSubmit(con->doneButton, bit_PressDone, con);
  naSetButtonAbort(con->doneButton, bit_PressDone, con);

  return con;
}



void bitDeallocPreferencesController(BitPreferencesController* prefs){
  naFree(prefs);
}



void bitShowPreferencesController(BitPreferencesController* prefs){
  naShowWindow(prefs->window);
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
