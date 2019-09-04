
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
  }else if(uielement == prefs->showAscCheckBox){
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



//NABool keyDone(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
//  NA_UNUSED(uielement);
//  NA_UNUSED(command);
//  NA_UNUSED(arg);
//  BitPreferencesController* prefs = controller;
//  naSimulateButtonPress(prefs->doneButton);
//  return NA_TRUE;
//}



BitPreferencesController* bitCreatePreferencesController(){
  BitPreferencesController* con = naAlloc(BitPreferencesController);

  NARect windowrect = naMakeRectS(20, 20, 250, 214);
  con->window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE);
  NASpace* space = naGetWindowContentSpace(con->window);

  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), naMakeRectS(20, 172, 330, 22));
  naAddSpaceChild(space, con->startupLabel);

  con->showASCIIOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsShowASCII), naMakeRectS(30, 150, 220, 22));
  naAddUIReaction(con, con->showASCIIOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->showASCIIOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, con->showASCIIOnStartupCheckBox);

  con->resetConversionOnStartupCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsResetSettings), naMakeRectS(30, 128, 220, 22));
  naAddUIReaction(con, con->resetConversionOnStartupCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->resetConversionOnStartupCheckBox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, con->resetConversionOnStartupCheckBox);

  con->onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(20, 96, 230, 22));
  naAddSpaceChild(space, con->onTopLabel);

  con->keepConverterOnTopCheckBox = naNewCheckBox(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(30, 52, 220, 22));
  naAddUIReaction(con, con->keepConverterOnTopCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckBoxState(con->keepConverterOnTopCheckBox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->keepConverterOnTopCheckBox);

  con->showAscCheckBox = naNewCheckBox("Show ASC", naMakeRectS(30, 74, 220, 22));
  naAddUIReaction(con, con->showAscCheckBox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
//  naSetCheckBoxState(con->showAscCheckBox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->showAscCheckBox);

  con->doneButton = naNewPushButton(bitTranslate(BitFiddlePrefsDone), naMakeRectS(150, 20, 80, 24));
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
