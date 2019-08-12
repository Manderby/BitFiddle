
#include "PreferencesController.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

struct BitPreferencesController{
  NAWindow* window;
  NALabel* startupLabel;
  NACheckbox* showASCIIOnStartupCheckbox;
  NACheckbox* resetConversionOnStartupCheckbox;
  NALabel* onTopLabel;
  NACheckbox* keepConverterOnTopCheckbox;
  NAButton* doneButton;
};



NABool switchPreferencesSetting(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitPreferencesController* prefs = controller;
  NABool state = naGetCheckboxState(uielement);
  if(uielement == prefs->showASCIIOnStartupCheckbox){
    naSetPreferencesBool(BitPrefs[ShowASCIIOnStartup], state);
  }else if(uielement == prefs->resetConversionOnStartupCheckbox){
    naSetPreferencesBool(BitPrefs[ResetConversionOnStartup], state);
  }else if(uielement == prefs->keepConverterOnTopCheckbox){
    naSetPreferencesBool(BitPrefs[KeepConverterOnTop], state);
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

  NARect windowrect = naMakeRectS(20, 20, 250, 214);
  con->window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE);
  NASpace* space = naGetWindowContentSpace(con->window);

  con->startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), naMakeRectS(20, 172, 330, 22));
  naAddSpaceChild(space, con->startupLabel);

  con->showASCIIOnStartupCheckbox = naNewCheckbox(bitTranslate(BitFiddlePrefsShowASCII), naMakeRectS(30, 150, 220, 22));
  naAddUIReaction(con, con->showASCIIOnStartupCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(con->showASCIIOnStartupCheckbox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, con->showASCIIOnStartupCheckbox);

  con->resetConversionOnStartupCheckbox = naNewCheckbox(bitTranslate(BitFiddlePrefsResetSettings), naMakeRectS(30, 128, 220, 22));
  naAddUIReaction(con, con->resetConversionOnStartupCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(con->resetConversionOnStartupCheckbox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, con->resetConversionOnStartupCheckbox);

  con->onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(20, 96, 230, 22));
  naAddSpaceChild(space, con->onTopLabel);

  con->keepConverterOnTopCheckbox = naNewCheckbox(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(30, 52, 220, 22));
  naAddUIReaction(con, con->keepConverterOnTopCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(con->keepConverterOnTopCheckbox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, con->keepConverterOnTopCheckbox);

  con->doneButton = naNewButton(bitTranslate(BitFiddlePrefsDone), naMakeRectS(150, 20, 80, 24));
  naAddUIReaction(con, con->doneButton, NA_UI_COMMAND_PRESSED, pressDone);
  naAddSpaceChild(space, con->doneButton);

  return con;
}



void bitClearPreferencesController(BitPreferencesController* prefs){
  naFree(prefs);
}



void naShowPreferencesController(BitPreferencesController* prefs){
  naShowWindow(prefs->window);
}
