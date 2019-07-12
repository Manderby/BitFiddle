
#include "ASCIIWindow.h"
#include "NAString.h"


NAButton* escapeButton;
NAButton* codeButton;
NAButton* hexButton;
NAButton* decButton;
NABool useHex = NA_FALSE;
NABool useCode = NA_TRUE;
NALabel* labels[128];
NALabel* chars[128];

const char* asciicodes[34] = {
  "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
  "BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI",
  "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
  "CAN", "EM", "SUB",  "ESC", "FS", "GS", "RS", "US",
  "SPC",
  "DEL"
};

const char* asciiescapes[34] = {
  "\\0", "", "", "", "", "", "", "",
  "\\b", "\\t", "\\n", "\\v", "\\f", "\\r", "", "",
  "", "", "", "", "", "", "", "",
  "", "", "", "(\\e)", "", "", "", "",
  "space",
  ""
};

void redrawWindow(){
  naSetRadioButtonState(codeButton, useCode);
  naSetRadioButtonState(escapeButton, !useCode);
  naSetRadioButtonState(hexButton, useHex);
  naSetRadioButtonState(decButton, !useHex);
  
  for(NAInt i = 0; i < 128; i++){
    NAString* labelstr;
    if(useHex){
      labelstr = naNewStringWithFormat("%02x", (int)i);
    }else{
      labelstr = naNewStringWithFormat("%d", (int)i);
    }
    naSetLabelText(labels[i], naGetStringUTF8Pointer(labelstr));
    naDelete(labelstr);
        
    NAString* charstr;
    if(i < 33){
      charstr = naNewStringWithFormat(useCode ? asciicodes[i] : asciiescapes[i]);
    }else if(i < 127){
      charstr = naNewStringWithFormat("%c", (char)i);
    }else{
      charstr = naNewStringWithFormat(useCode ? asciicodes[33] : asciiescapes[33]);
    }
    naSetLabelText(chars[i], naGetStringUTF8Pointer(charstr));
    naDelete(charstr);
  }
}



NABool switchDisplayMode(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  if(uielement == escapeButton){
    useCode = NA_FALSE;
  }else if(uielement == codeButton){
    useCode = NA_TRUE;
  }else if(uielement == hexButton){
    useHex = NA_TRUE;
  }else if(uielement == decButton){
    useHex = NA_FALSE;
  }else{
    #ifndef NDEBUG
      naError("Unknown uielement sent message");
    #endif
  }
  redrawWindow();
  return NA_TRUE;
}



NAWindow* createASCIIWindow(){
  NARect windowrect = naMakeRectS(20, 20, 776, 430);
  NAWindow* window = naNewWindow("ASCII", windowrect, NA_FALSE);
  
  NASpace* space = naGetWindowContentSpace(window);
  NAInt curindex = 0;
  for(NAInt x = 0; x < 8; x++){
    NARect rect = naMakeRectS(x * 97, 64, 97, 366);
    NASpace* columnspace = naNewSpace(rect);
    naSetSpaceAlternateBackground(columnspace, (x % 2 == 0));

    for(NAInt y = 0; y < 16; y++){
      NARect rect;
      
      rect = naMakeRectS(5, (15 - y) * 22 + 5, 32, 22);
      labels[curindex] = naNewLabel("", rect);
      naSetLabelFontKind(labels[curindex], NA_FONT_KIND_MONOSPACE);
      naSetLabelTextAlignment(labels[curindex], NA_TEXT_ALIGNMENT_RIGHT);
      naAddSpaceChild(columnspace, labels[curindex]);

      rect = naMakeRectS(42, (15 - y) * 22 + 5, 50, 22);
      chars[curindex] = naNewLabel("", rect);
      naSetLabelFontKind(chars[curindex], NA_FONT_KIND_MONOSPACE);
      naSetLabelTextAlignment(chars[curindex], NA_TEXT_ALIGNMENT_CENTER);
      naAddSpaceChild(columnspace, chars[curindex]);
      
      curindex++;
    }

    naAddSpaceChild(space, columnspace);
  }
  
  escapeButton = naNewRadioButton("Escape", naMakeRectS(15, 10, 64, 22));
  naAddUIReaction(NA_NULL, escapeButton, NA_UI_COMMAND_PRESSED, switchDisplayMode);
  naAddSpaceChild(space, escapeButton);

  codeButton = naNewRadioButton("Code", naMakeRectS(15, 32, 64, 22));
  naAddUIReaction(NA_NULL, codeButton, NA_UI_COMMAND_PRESSED, switchDisplayMode);
  naAddSpaceChild(space, codeButton);

  hexButton = naNewRadioButton("Hex", naMakeRectS(112, 10, 64, 22));
  naAddUIReaction(NA_NULL, hexButton, NA_UI_COMMAND_PRESSED, switchDisplayMode);
  naAddSpaceChild(space, hexButton);

  decButton = naNewRadioButton("Dec", naMakeRectS(112, 32, 64, 22));
  naAddUIReaction(NA_NULL, decButton, NA_UI_COMMAND_PRESSED, switchDisplayMode);
  naAddSpaceChild(space, decButton);
  
  redrawWindow();
  naShowWindow(window);
  return window;
}
