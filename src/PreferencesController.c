
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
  NABool state = naGetCheckBoxState(reaction.uielement);
  if(reaction.uielement == prefs->showASCIIOnStartupCheckBox){
    naSetPreferencesBool(BitPrefs[ShowASCIIOnStartup], state);
  }else if(reaction.uielement == prefs->resetConversionOnStartupCheckBox){
    naSetPreferencesBool(BitPrefs[ResetConversionOnStartup], state);
  }else if(reaction.uielement == prefs->keepConverterOnTopCheckBox){
    naSetPreferencesBool(BitPrefs[KeepConverterOnTop], state);
    bitUpdateApp();
  }else if(reaction.uielement == prefs->show16BitsCheckBox){
    naSetPreferencesBool(BitPrefs[Show16Bits], state);
    bitRecreateConverterController();
  }else if(reaction.uielement == prefs->showNBitsCheckBox){
    naSetPreferencesBool(BitPrefs[ShowNBits], state);
    bitRecreateConverterController();
  }else if(reaction.uielement == prefs->showBinCheckBox){
    naSetPreferencesBool(BitPrefs[ShowBin], state);
    bitRecreateConverterController();
  }else if(reaction.uielement == prefs->showAscCheckBox){
    naSetPreferencesBool(BitPrefs[ShowAsc], state);
    bitRecreateConverterController();
  }else{
    #ifndef NDEBUG
      naError("Unknown uielement sent message");
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

  NARect windowrect = naMakeRectS(20, 20, 260, 236);
  con->window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE, BIT_WINDOW_TAG_PREFERENCES);
  NASpace* space = naGetWindowContentSpace(con->window);

  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), naMakeRectS(20, 194, 330, 22));
  naAddSpaceChild(space, con->startupLabel);

  con->showASCIIOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowASCII), naMakeRectS(30, 172, 220, 22));
  naAddUIReaction(con->showASCIIOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showASCIIOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, con->showASCIIOnStartupCheckBox);

  con->resetConversionOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsResetSettings), naMakeRectS(30, 150, 220, 22));
  naAddUIReaction(con->resetConversionOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->resetConversionOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, con->resetConversionOnStartupCheckBox);

  con->onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsConverterWindow), naMakeRectS(20, 118, 230, 22));
  naAddSpaceChild(space, con->onTopLabel);

  con->keepConverterOnTopCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(30, 96, 220, 22));
  naAddUIReaction(con->keepConverterOnTopCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->keepConverterOnTopCheckBox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->keepConverterOnTopCheckBox);

  con->show16BitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShow16Bits), naMakeRectS(30, 74, 80, 22));
  naAddUIReaction(con->show16BitsCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->show16BitsCheckBox, naGetPreferencesBool(BitPrefs[Show16Bits]));
  naAddSpaceChild(space, con->show16BitsCheckBox);

  con->showNBitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowNBits), naMakeRectS(130, 74, 80, 22));
  naAddUIReaction(con->showNBitsCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showNBitsCheckBox, naGetPreferencesBool(BitPrefs[ShowNBits]));
  naAddSpaceChild(space, con->showNBitsCheckBox);

  con->showBinCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowBin), naMakeRectS(30, 52, 80, 22));
  naAddUIReaction(con->showBinCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showBinCheckBox, naGetPreferencesBool(BitPrefs[ShowBin]));
  naAddSpaceChild(space, con->showBinCheckBox);

  con->showAscCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowAsc), naMakeRectS(130, 52, 80, 22));
  naAddUIReaction(con->showAscCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting, con);
  naSetCheckBoxState(con->showAscCheckBox, naGetPreferencesBool(BitPrefs[ShowAsc]));
  naAddSpaceChild(space, con->showAscCheckBox);

  con->doneButton = naNewPushButton(bitTranslate(BitFiddlePrefsDone), naMakeRectS(160, 20, 80, 24));
  naAddUIReaction(con->doneButton, NA_UI_COMMAND_PRESSED, pressDone, con);
  naAddSpaceChild(space, con->doneButton);

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
