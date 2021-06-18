
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

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



NABool switchPreferencesSetting(NAReaction reaction){
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
    #ifndef NDEBUG
      naError("Unknown uiElement sent message");
    #endif
  }
  bitUpdateApp();
  return NA_TRUE;
}



NABool pressDone(NAReaction reaction){
  BitPreferencesController* prefs = reaction.controller;
  naCloseWindow(prefs->window);
  return NA_TRUE;
}



BitPreferencesController* bitCreatePreferencesController(){
  BitPreferencesController* con = naAlloc(BitPreferencesController);

  NARect windowrect = naMakeRectS(820, 15, 260, 236);
  con->window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE, BIT_WINDOW_TAG_PREFERENCES);
  NASpace* space = naGetWindowContentSpace(con->window);

  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), naMakeSize(330, 22));
  naAddSpaceChild(space, con->startupLabel, naMakePos(20., 194.));

  con->showASCIIOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowASCII), naMakeSize(220, 22));
  naAddUIReaction(con->showASCIIOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showASCIIOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, con->showASCIIOnStartupCheckBox, naMakePos(30., 172.));

  con->resetConversionOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsResetSettings), naMakeSize(220, 22));
  naAddUIReaction(con->resetConversionOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->resetConversionOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, con->resetConversionOnStartupCheckBox, naMakePos(30., 150.));

  con->onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsConverterWindow), naMakeSize(230, 22));
  naAddSpaceChild(space, con->onTopLabel, naMakePos(20., 118.));

  con->keepConverterOnTopCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeSize(220, 22));
  naAddUIReaction(con->keepConverterOnTopCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->keepConverterOnTopCheckBox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->keepConverterOnTopCheckBox, naMakePos(30., 96.));

  con->show16BitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShow16Bits), naMakeSize(80, 22));
  naAddUIReaction(con->show16BitsCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->show16BitsCheckBox, naGetPreferencesBool(BitPrefs[Show16Bits]));
  naAddSpaceChild(space, con->show16BitsCheckBox, naMakePos(30., 74.));

  con->showNBitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowNBits), naMakeSize(80, 22));
  naAddUIReaction(con->showNBitsCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showNBitsCheckBox, naGetPreferencesBool(BitPrefs[ShowNBits]));
  naAddSpaceChild(space, con->showNBitsCheckBox, naMakePos(130., 74.));

  con->showBinCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowBin), naMakeSize(80, 22));
  naAddUIReaction(con->showBinCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showBinCheckBox, naGetPreferencesBool(BitPrefs[ShowBin]));
  naAddSpaceChild(space, con->showBinCheckBox, naMakePos(30., 52.));

  con->showAscCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowAsc), naMakeSize(80, 22));
  naAddUIReaction(con->showAscCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showAscCheckBox, naGetPreferencesBool(BitPrefs[ShowAsc]));
  naAddSpaceChild(space, con->showAscCheckBox, naMakePos(130., 52.));

  con->doneButton = naNewTextButton(bitTranslate(BitFiddlePrefsDone), naMakeSize(80, 24), 0);
  naAddUIReaction(con->doneButton, NA_UI_COMMAND_PRESSED, pressDone, con);
  naAddSpaceChild(space, con->doneButton, naMakePos(160., 20.));

  naSetButtonSubmit(con->doneButton, pressDone, con);
  naSetButtonAbort(con->doneButton, pressDone, con);

  return con;
}



void bitClearPreferencesController(BitPreferencesController* prefs){
  naFree(prefs);
}



void naShowPreferencesController(BitPreferencesController* prefs){
  naShowWindow(prefs->window);
}



// Copyright(c) Tobias Stamm
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this softwareand associated documentation files(the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and /or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
