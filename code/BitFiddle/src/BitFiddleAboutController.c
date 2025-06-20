
#include "BitFiddleAboutController.h"
#include "BitFiddleApplication.h"
#include "BitFiddleTranslations.h"
#include "NAUtility/NAMemory.h"
#include "NAUtility/NAString.h"
#include "NAApp/NAImageSet.h"
#include "NAVisual/NAImage.h"



struct BitFiddleAboutController{
  NAWindow* window;
  NAImageSpace* iconSpace;
  NALabel* appNameLabel;
  NALabel* appVersionLabel;
  NAImageSpace* manderCSpace;
  NALabel* helpLinkLabel;
  NAButton* doneButton;
};



void bit_pressAboutDone(NAReaction reaction) {
  BitFiddleAboutController* con = (BitFiddleAboutController*)reaction.controller;
  naCloseWindow(con->window);
}



BitFiddleAboutController* bitAllocAboutController(void) {
  BitFiddleAboutController* con = naAlloc(BitFiddleAboutController);

  NAString* bundleApplicationName = naNewApplicationName();

  NARect windowRect = naMakeRectS(20, 300, 340, 300);
  const NAUTF8Char* aboutWindowTitleFormatString = bitTranslate(BitFiddleAbout);
  NAString* aboutWindowTitleString = naNewStringWithFormat(aboutWindowTitleFormatString, naGetStringUTF8Pointer(bundleApplicationName));

  con->window = naNewWindow(
    naGetStringUTF8Pointer(aboutWindowTitleString),
    windowRect,
    0);
  
  naSetWindowStorageTag(con->window, BIT_WINDOW_TAG_ABOUT);

  naDelete(aboutWindowTitleString);
  
  NASpace* space = naGetWindowContentSpace(con->window);

  NAString* iconPath = naNewApplicationIconPath();
  NAImage* iconImage = naCreateImageWithFilePath(naGetStringUTF8Pointer(iconPath));
  NAImageSet* iconImageSet = naCreateImageSet(iconImage, NA_UI_RESOLUTION_2x, NA_BLEND_ZERO);
  naRelease(iconImage);
  naDelete(iconPath);
  con->iconSpace = naNewImageSpace(iconImageSet, naMakeSize(128, 128));
  naAddSpaceChild(space, con->iconSpace, naMakePos(106., 150.));
  naRelease(iconImageSet);

  con->appNameLabel = naNewLabel(naGetStringUTF8Pointer(bundleApplicationName), 300);
  NAFont* titleFont = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_BIG);
  naSetLabelFont(con->appNameLabel, titleFont);
  naSetLabelTextAlignment(con->appNameLabel, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelHeight(con->appNameLabel, 24);
  naRelease(titleFont);
  naAddSpaceChild(space, con->appNameLabel, naMakePos(20., 116.));

  NAString* bundleVersionString = naNewApplicationVersionString();
  NAString* bundleBuildString = naNewApplicationBuildString();
  const NAUTF8Char* aboutVersionFormatString = bitTranslate(BitFiddleVersionBuild);
  NAString* aboutVersionString = naNewStringWithFormat(aboutVersionFormatString, naGetStringUTF8Pointer(bundleVersionString), naGetStringUTF8Pointer(bundleBuildString));
  con->appVersionLabel = naNewLabel(naGetStringUTF8Pointer(aboutVersionString), 300);
  naDelete(aboutVersionString);
  naDelete(bundleVersionString);
  naDelete(bundleBuildString);
  naSetLabelTextAlignment(con->appVersionLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(space, con->appVersionLabel, naMakePos(20., 90.));

  con->helpLinkLabel = naNewLabel(bitTranslate(BitFiddleOnlineHelp), 300);
  naSetLabelTextAlignment(con->helpLinkLabel, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelLink(con->helpLinkLabel, bitTranslate(BitFiddleApplicationHelpURL));
  naAddSpaceChild(space, con->helpLinkLabel, naMakePos(20., 54.));

  con->doneButton = naNewTextPushButton(bitTranslate(BitFiddleDone), 80);
  naAddUIReaction(con->doneButton, NA_UI_COMMAND_PRESSED, bit_pressAboutDone, con);
  naAddSpaceChild(space, con->doneButton, naMakePos(130., 20.));

  naSetButtonSubmit(con->doneButton, bit_pressAboutDone, con);
  naSetButtonAbort(con->doneButton, bit_pressAboutDone, con);

  naDelete(bundleApplicationName);
  
  return con;
}



void bitDeallocAboutController(BitFiddleAboutController* con) {
  naFree(con);
}



void bitShowAboutController(BitFiddleAboutController* con) {
  naShowWindow(con->window);
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
