
#include "PreferencesWindow.h"
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

NAWindow* window;
NALabel* startupLabel;
NACheckbox* showASCIIOnStartupCheckbox;
NACheckbox* resetConversionOnStartupCheckbox;
NALabel* onTopLabel;
NACheckbox* keepConverterOnTopCheckbox;
NAButton* doneButton;





NABool switchPreferencesSetting(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  if(uielement == showASCIIOnStartupCheckbox){
    naSetPreferencesBool(BitPrefs[ShowASCIIOnStartup], naGetCheckboxState(showASCIIOnStartupCheckbox));
  }else if(uielement == resetConversionOnStartupCheckbox){
    naSetPreferencesBool(BitPrefs[ResetConversionOnStartup], naGetCheckboxState(resetConversionOnStartupCheckbox));
  }else if(uielement == keepConverterOnTopCheckbox){
    naSetPreferencesBool(BitPrefs[KeepConverterOnTop], naGetCheckboxState(keepConverterOnTopCheckbox));
  }else{
    #ifndef NDEBUG
      naError("Unknown uielement sent message");
    #endif
  }
  bitUpdateApp();
  return NA_TRUE;
}



NABool pressDone(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  naCloseWindow(window);
  return NA_TRUE;
}



NAWindow* bitCreatePreferencesWindow(){
  NARect windowrect = naMakeRectS(20, 20, 250, 214);
  window = naNewWindow(bitTranslate(BitFiddlePrefsWindowTitle), windowrect, NA_FALSE);
  NASpace* space = naGetWindowContentSpace(window);

  startupLabel = naNewLabel(bitTranslate(BitFiddlePrefsAtStartup), naMakeRectS(20, 172, 330, 22));
  naAddSpaceChild(space, startupLabel);

  showASCIIOnStartupCheckbox = naNewCheckbox(bitTranslate(BitFiddlePrefsShowASCII), naMakeRectS(30, 150, 220, 22));
  naAddUIReaction(NA_NULL, showASCIIOnStartupCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(showASCIIOnStartupCheckbox, naGetPreferencesBool(BitPrefs[ShowASCIIOnStartup]));
  naAddSpaceChild(space, showASCIIOnStartupCheckbox);

  resetConversionOnStartupCheckbox = naNewCheckbox(bitTranslate(BitFiddlePrefsResetSettings), naMakeRectS(30, 128, 220, 22));
  naAddUIReaction(NA_NULL, resetConversionOnStartupCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(resetConversionOnStartupCheckbox, naGetPreferencesBool(BitPrefs[ResetConversionOnStartup]));
  naAddSpaceChild(space, resetConversionOnStartupCheckbox);

  onTopLabel = naNewLabel(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(20, 96, 230, 22));
  naAddSpaceChild(space, onTopLabel);

  keepConverterOnTopCheckbox = naNewCheckbox(bitTranslate(BitFiddlePrefsKeepOnTop), naMakeRectS(30, 52, 220, 22));
  naAddUIReaction(NA_NULL, keepConverterOnTopCheckbox, NA_UI_COMMAND_PRESSED, switchPreferencesSetting);
  naSetCheckboxState(keepConverterOnTopCheckbox, naGetPreferencesBool(BitPrefs[KeepConverterOnTop]));
  naAddSpaceChild(space, keepConverterOnTopCheckbox);

  doneButton = naNewButton(bitTranslate(BitFiddlePrefsDone), naMakeRectS(150, 20, 80, 24));
  naAddUIReaction(NA_NULL, doneButton, NA_UI_COMMAND_PRESSED, pressDone);
  naAddSpaceChild(space, doneButton);

  return window;
}

