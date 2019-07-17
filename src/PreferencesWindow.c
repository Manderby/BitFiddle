
#include "PreferencesWindow.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"

NAWindow* window;
NALabel* startupLabel;
NACheckbox* showASCIIOnStartupCheckbox;
NACheckbox* resetConversionOnStartupCheckbox;
NALabel* onTopLabel;
NACheckbox* keepMaxiOnTopCheckbox;
NACheckbox* keepMiniOnTopCheckbox;
NAButton* doneButton;


void updateApp();



NABool switchPreferencesSetting(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  if(uielement == showASCIIOnStartupCheckbox){
    naSetPreferencesBool(bitPrefShowASCIIOnStartup, naGetCheckboxState(showASCIIOnStartupCheckbox));
  }else if(uielement == resetConversionOnStartupCheckbox){
    naSetPreferencesBool(bitPrefResetConversionOnStartup, naGetCheckboxState(resetConversionOnStartupCheckbox));
  }else if(uielement == keepMaxiOnTopCheckbox){
    naSetPreferencesBool(bitPrefKeepMaxiOnTop, naGetCheckboxState(keepMaxiOnTopCheckbox));
  }else if(uielement == keepMiniOnTopCheckbox){
    naSetPreferencesBool(bitPrefKeepMiniOnTop, naGetCheckboxState(keepMiniOnTopCheckbox));
  }else{
    #ifndef NDEBUG
      naError("Unknown uielement sent message");
    #endif
  }
  updateApp();
  return NA_TRUE;
}



NABool pressDone(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  naCloseWindow(window);
  return NA_TRUE;
}



NAWindow* createPreferencesWindow(){
  NARect windowrect = naMakeRectS(20, 20, 250, 214);
  window = naNewWindow(naTranslate(translatorGroup, BitFiddlePrefsWindowTitle), windowrect, NA_FALSE);
  NASpace* space = naGetWindowContentSpace(window);

  startupLabel = naNewLabel(naTranslate(translatorGroup, BitFiddlePrefsAtStartup), naMakeRectS(20, 172, 330, 22));
  naAddSpaceChild(space, startupLabel);

  showASCIIOnStartupCheckbox = naNewCheckbox(naTranslate(translatorGroup, BitFiddlePrefsShowASCII), naMakeRectS(30, 150, 220, 22));
  naAddUIReaction(NA_NULL, showASCIIOnStartupCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(showASCIIOnStartupCheckbox, naGetPreferencesBool(bitPrefShowASCIIOnStartup));
  naAddSpaceChild(space, showASCIIOnStartupCheckbox);

  resetConversionOnStartupCheckbox = naNewCheckbox(naTranslate(translatorGroup, BitFiddlePrefsResetSettings), naMakeRectS(30, 128, 220, 22));
  naAddUIReaction(NA_NULL, resetConversionOnStartupCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(resetConversionOnStartupCheckbox, naGetPreferencesBool(bitPrefResetConversionOnStartup));
  naAddSpaceChild(space, resetConversionOnStartupCheckbox);

  onTopLabel = naNewLabel(naTranslate(translatorGroup, BitFiddlePrefsKeepOnTop), naMakeRectS(20, 96, 230, 22));
  naAddSpaceChild(space, onTopLabel);

  keepMaxiOnTopCheckbox = naNewCheckbox(naTranslate(translatorGroup, BitFiddlePrefsMaxiVersion), naMakeRectS(30, 74, 220, 22));
  naAddUIReaction(NA_NULL, keepMaxiOnTopCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(keepMaxiOnTopCheckbox, naGetPreferencesBool(bitPrefKeepMaxiOnTop));
  naAddSpaceChild(space, keepMaxiOnTopCheckbox);

  keepMiniOnTopCheckbox = naNewCheckbox(naTranslate(translatorGroup, BitFiddlePrefsMiniVersion), naMakeRectS(30, 52, 220, 22));
  naAddUIReaction(NA_NULL, keepMiniOnTopCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(keepMiniOnTopCheckbox, naGetPreferencesBool(bitPrefKeepMiniOnTop));
  naAddSpaceChild(space, keepMiniOnTopCheckbox);

  doneButton = naNewButton(naTranslate(translatorGroup, BitFiddlePrefsDone), naMakeRectS(150, 20, 80, 24));
  naAddUIReaction(NA_NULL, doneButton, NA_UI_COMMAND_PRESSED, pressDone);
  naAddSpaceChild(space, doneButton);

  return window;
}

