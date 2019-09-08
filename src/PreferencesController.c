
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



NABool switchPreferencesSetting(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitPreferencesController* prefs = controller;
  NABool state = naGetCheckBoxState(uielement);
  if(uielement == prefs->showASCIIOnStartupCheckBox){
    naSetPreferencesBool(BitPrefs[ShowASCIIOnStartup], state);
  }else if(uielement == prefs->resetConversionOnStartupCheckBox){
    naSetPreferencesBool(BitPrefs[ResetConversionOnStartup], state);
  }else if(uielement == prefs->keepConverterOnTopCheckBox){
    naSetPreferencesBool(BitPrefs[KeepConverterOnTop], state);
    bitUpdateApp();
  }else if(uielement == prefs->show16BitsCheckBox){
    naSetPreferencesBool(BitPrefs[Show16Bits], state);
    bitRecreateConverterController();
  }else if(uielement == prefs->showNBitsCheckBox){
    naSetPreferencesBool(BitPrefs[ShowNBits], state);
    bitRecreateConverterController();
  }else if(uielement == prefs->showBinCheckBox){
    naSetPreferencesBool(BitPrefs[ShowBin], state);
    bitRecreateConverterController();
  }else if(uielement == prefs->showAscCheckBox){
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



NABool pressDone(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitPreferencesController* prefs = controller;
  naCloseWindow(prefs->window);
  return NA_TRUE;
}



BitPreferencesController* bitCreatePreferencesController(){
  BitPreferencesController* con = naAlloc(BitPreferencesController);

  NARect windowrect = naMakeRectS(20, 20, 260, 236);
  con->window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE);
  NASpace* space = naGetWindowContentSpace(con->window);

  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), naMakeRectS(20, 194, 330, 22));
  naAddSpaceChild(space, con->startupLabel);

  con->showASCIIOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowASCII), naMakeRectS(30, 172, 220, 22));
  naAddUIReaction(con, con->showASCIIOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->showASCIIOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, con->showASCIIOnStartupCheckBox);

  con->resetConversionOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsResetSettings), naMakeRectS(30, 150, 220, 22));
  naAddUIReaction(con, con->resetConversionOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->resetConversionOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, con->resetConversionOnStartupCheckBox);

  con->onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsConverterWindow), naMakeRectS(20, 118, 230, 22));
  naAddSpaceChild(space, con->onTopLabel);

  con->keepConverterOnTopCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(30, 96, 220, 22));
  naAddUIReaction(con, con->keepConverterOnTopCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->keepConverterOnTopCheckBox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->keepConverterOnTopCheckBox);

  con->show16BitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShow16Bits), naMakeRectS(30, 74, 80, 22));
  naAddUIReaction(con, con->show16BitsCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->show16BitsCheckBox, naGetPreferencesBool(BitPrefs[Show16Bits]));
  naAddSpaceChild(space, con->show16BitsCheckBox);

  con->showNBitsCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowNBits), naMakeRectS(130, 74, 80, 22));
  naAddUIReaction(con, con->showNBitsCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->showNBitsCheckBox, naGetPreferencesBool(BitPrefs[ShowNBits]));
  naAddSpaceChild(space, con->showNBitsCheckBox);

  con->showBinCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowBin), naMakeRectS(30, 52, 80, 22));
  naAddUIReaction(con, con->showBinCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->showBinCheckBox, naGetPreferencesBool(BitPrefs[ShowBin]));
  naAddSpaceChild(space, con->showBinCheckBox);

  con->showAscCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowAsc), naMakeRectS(130, 52, 80, 22));
  naAddUIReaction(con, con->showAscCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->showAscCheckBox, naGetPreferencesBool(BitPrefs[ShowAsc]));
  naAddSpaceChild(space, con->showAscCheckBox);

  con->doneButton = naNewPushButton(bitTranslate(BitFiddlePrefsDone), naMakeRectS(160, 20, 80, 24));
  naAddUIReaction(con, con->doneButton, NA_UI_COMMAND_PRESSED, pressDone);
  naAddSpaceChild(space, con->doneButton);

  naSetButtonSubmit(con->doneButton, con, pressDone);
  naSetButtonAbort(con->doneButton, con, pressDone);

  return con;
}



void bitClearPreferencesController(BitPreferencesController* prefs){
  naFree(prefs);
}



void naShowPreferencesController(BitPreferencesController* prefs){
  naShowWindow(prefs->window);
}
