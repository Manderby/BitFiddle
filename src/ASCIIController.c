
#include "ASCIIController.h"
#include BIT_NALIB_PATH(NAString.h)
#include "BitFiddleTranslations.h"
#include "BitFiddlePreferences.h"
#include "BitFiddleApplication.h"

struct BitASCIIController{
  NAWindow* window;
  NARadio* escapeRadio;
  NARadio* codeRadio;
  NARadio* hexRadio;
  NARadio* decRadio;
  NABool useHex;
  NABool useEscape;
  NASpace* spaces[128];
  NALabel* labels[128];
  NALabel* chars[128];
  NALabel* info1;
  NALabel* info2;
};

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

const char* unicodenames[128] = {
  "NULL",
  "START OF HEADING",
  "START OF TEXT",
  "END OF TEXT",
  "END OF TRANSMISSION",
  "ENQUIRY",
  "ACKNOWLEDGE",
  "BELL",
  "BACKSPACE",
  "HORIZONTAL TABULATION",
  "LINE FEED",
  "VERTICAL TABULATION",
  "FORM FEED",
  "CARRIAGE RETURN",
  "SHIFT OUT",
  "SHIFT IN",

  "DATA LINK ESCAPE",
  "DEVICE CONTROL ONE",
  "DEVICE CONTROL TWO",
  "DEVICE CONTROL THREE",
  "DEVICE CONTROL FOUR",
  "NEGATIVE ACKNOWLEDGE",
  "SYNCHRONOUS IDLE",
  "END OF TRANSMISSION BLOCK",
  "CANCEL",
  "END OF MEDIUM",
  "SUBSTITUTE",
  "ESCAPE",
  "FILE SEPARATOR",
  "GROUP SEPARATOR",
  "RECORD SEPARATOR",
  "UNIT SEPARATOR",

  "SPACE",
  "EXCLAMATION MARK",
  "QUOTATION MARK",
  "NUMBER SIGN",
  "DOLLAR SIGN",
  "PERCENT SIGN",
  "AMPERSAND",
  "APOSTROPHE",
  "LEFT PARENTHESIS",
  "RIGHT PARENTHESIS",
  "ASTERISK",
  "PLUS SIGN",
  "COMMA",
  "HYPHEN-MINUS",
  "FULL STOP",
  "SOLIDUS",

  "DIGIT ZERO",
  "DIGIT ONE",
  "DIGIT TWO",
  "DIGIT THREE",
  "DIGIT FOUR",
  "DIGIT FIVE",
  "DIGIT SIX",
  "DIGIT SEVEN",
  "DIGIT EIGHT",
  "DIGIT NINE",
  "COLON",
  "SEMICOLON",
  "LESS-THAN SIGN",
  "EQUALS SIGN",
  "GREATER-THAN SIGN",
  "QUESTION MARK",

  "COMMERCIAL AT",
  "LATIN CAPITAL LETTER A",
  "LATIN CAPITAL LETTER B",
  "LATIN CAPITAL LETTER C",
  "LATIN CAPITAL LETTER D",
  "LATIN CAPITAL LETTER E",
  "LATIN CAPITAL LETTER F",
  "LATIN CAPITAL LETTER G",
  "LATIN CAPITAL LETTER H",
  "LATIN CAPITAL LETTER I",
  "LATIN CAPITAL LETTER J",
  "LATIN CAPITAL LETTER K",
  "LATIN CAPITAL LETTER L",
  "LATIN CAPITAL LETTER M",
  "LATIN CAPITAL LETTER N",
  "LATIN CAPITAL LETTER O",

  "LATIN CAPITAL LETTER P",
  "LATIN CAPITAL LETTER Q",
  "LATIN CAPITAL LETTER R",
  "LATIN CAPITAL LETTER S",
  "LATIN CAPITAL LETTER T",
  "LATIN CAPITAL LETTER U",
  "LATIN CAPITAL LETTER V",
  "LATIN CAPITAL LETTER W",
  "LATIN CAPITAL LETTER X",
  "LATIN CAPITAL LETTER Y",
  "LATIN CAPITAL LETTER Z",
  "LEFT SQUARE BRACKET",
  "REVERSE SOLIDUS",
  "RIGHT SQUARE BRACKET",
  "CIRCUMFLEX ACCENT",
  "LOW LINE",

  "GRAVE ACCENT",
  "LATIN SMALL LETTER A",
  "LATIN SMALL LETTER B",
  "LATIN SMALL LETTER C",
  "LATIN SMALL LETTER D",
  "LATIN SMALL LETTER E",
  "LATIN SMALL LETTER F",
  "LATIN SMALL LETTER G",
  "LATIN SMALL LETTER H",
  "LATIN SMALL LETTER I",
  "LATIN SMALL LETTER J",
  "LATIN SMALL LETTER K",
  "LATIN SMALL LETTER L",
  "LATIN SMALL LETTER M",
  "LATIN SMALL LETTER N",
  "LATIN SMALL LETTER O",

  "LATIN SMALL LETTER P",
  "LATIN SMALL LETTER Q",
  "LATIN SMALL LETTER R",
  "LATIN SMALL LETTER S",
  "LATIN SMALL LETTER T",
  "LATIN SMALL LETTER U",
  "LATIN SMALL LETTER V",
  "LATIN SMALL LETTER W",
  "LATIN SMALL LETTER X",
  "LATIN SMALL LETTER Y",
  "LATIN SMALL LETTER Z",
  "LEFT CURLY BRACKET",
  "VERTICAL LINE",
  "RIGHT CURLY BRACKET",
  "TILDE",
  "DELETE",
};



void redrawASCIIController(BitASCIIController* con){
  naSetRadioState(con->escapeRadio, con->useEscape);
  naSetRadioState(con->codeRadio, !con->useEscape);
  naSetRadioState(con->hexRadio, con->useHex);
  naSetRadioState(con->decRadio, !con->useHex);
  
  for(NAInt i = 0; i < 128; i++){
    NAString* labelstr;
    if(con->useHex){
      labelstr = naNewStringWithFormat("%02x", (int)i);
    }else{
      labelstr = naNewStringWithFormat("%d", (int)i);
    }
    //if(con->labels[i])  // todo remove this if.
      naSetLabelText(con->labels[i], naGetStringUTF8Pointer(labelstr));
    naDelete(labelstr);
        
    NAString* charstr;
    if(i < 33){
      charstr = naNewStringWithFormat(con->useEscape ? asciiescapes[i] : asciicodes[i]);
    }else if(i < 127){
      charstr = naNewStringWithFormat("%c", (char)i);
    }else{
      charstr = naNewStringWithFormat(con->useEscape ? asciiescapes[33] : asciicodes[33]);
    }
    //if(con->chars[i])  // todo remove this if.
      naSetLabelText(con->chars[i], naGetStringUTF8Pointer(charstr));
    naDelete(charstr);
  }
}



NAInt getUISpaceIndex(BitASCIIController* con, NAUIElement* uielement){
  NAInt itemIndex;
  for(itemIndex = 0; itemIndex < 128; itemIndex++){
    if(con->spaces[itemIndex] == uielement){break;}
  }
  #ifndef NDEBUG
    if(itemIndex == 128)
      naError("Hovered element not recognized");
  #endif
  return itemIndex;
}



NABool hoverItem(NAReaction reaction){
  BitASCIIController* con = reaction.controller;
  NAInt itemIndex = getUISpaceIndex(con, reaction.uielement);
  
  naSetSpaceAlternateBackground(con->spaces[itemIndex], NA_TRUE);
  
  NAString* info1string;
  if(itemIndex < 32){
    info1string = naNewStringWithFormat("%i   0x%02x   %s\n%s", itemIndex, itemIndex, asciicodes[itemIndex], unicodenames[itemIndex]);
  }else if(itemIndex < 127){
    info1string = naNewStringWithFormat("%i   0x%02x   \'%c\'\n%s", itemIndex, itemIndex, itemIndex, unicodenames[itemIndex]);
  }else{
    info1string = naNewStringWithFormat("%i   0x%02x   %s\n%s", itemIndex, itemIndex, asciicodes[33], unicodenames[127]);
  }
  naSetLabelText(con->info1, naGetStringUTF8Pointer(info1string));
  naDelete(info1string);
  
  naSetLabelText(con->info2, bitTranslate(BitFiddleAdditionalInfo00 + itemIndex));
  return NA_TRUE;
}



NABool unhoverItem(NAReaction reaction){
  BitASCIIController* con = reaction.controller;
  NAInt itemIndex = getUISpaceIndex(con, reaction.uielement);  
  naSetSpaceAlternateBackground(con->spaces[itemIndex], NA_FALSE);
  return NA_TRUE;
}



NABool switchASCIIDisplayMode(NAReaction reaction){
  BitASCIIController* con = reaction.controller;
  if(reaction.uielement == con->escapeRadio){
    con->useEscape = NA_TRUE;
    naSetPreferencesBool(BitPrefs[UseASCIIEscape], NA_TRUE);
  }else if(reaction.uielement == con->codeRadio){
    con->useEscape = NA_FALSE;
    naSetPreferencesBool(BitPrefs[UseASCIIEscape], NA_FALSE);
  }else if(reaction.uielement == con->hexRadio){
    con->useHex = NA_TRUE;
    naSetPreferencesBool(BitPrefs[UseASCIIHex], NA_TRUE);
  }else if(reaction.uielement == con->decRadio){
    con->useHex = NA_FALSE;
    naSetPreferencesBool(BitPrefs[UseASCIIHex], NA_FALSE);
  }else{
    #ifndef NDEBUG
      naError("Unknown uielement sent message");
    #endif
  }
  redrawASCIIController(con);
  return NA_TRUE;
}



BitASCIIController* bitCreateASCIIController(void){
  BitASCIIController* con = naAlloc(BitASCIIController);

  con->useHex = NA_FALSE;
  con->useEscape = NA_FALSE;

  NARect windowrect = naMakeRectS(20, 20, 776, 430);
  con->window = naNewWindow("ASCII", windowrect, NA_FALSE, BIT_WINDOW_TAG_ASCII);
  
  NASpace* space = naGetWindowContentSpace(con->window);
  NAInt curindex = 0;
  for(NAInt x = 0; x < 8; x++){
    NASpace* columnspace = naNewSpace(naMakeSize(97, 366));
    naSetSpaceAlternateBackground(columnspace, (x % 2 == 0));

    for(NAInt y = 0; y < 16; y++){
      con->spaces[curindex] = naNewSpace(naMakeSize(87, 22));
      //if(x != 4 && y != 6){
      //  con->labels[curindex] = NA_NULL;
      //  con->spaces[curindex] = NA_NULL;
      //  con->chars[curindex] = NA_NULL;
      //  curindex++;
      //  continue;
      //}
      naAddUIReaction(con->spaces[curindex], NA_UI_COMMAND_MOUSE_ENTERED, hoverItem, con);
      naAddUIReaction(con->spaces[curindex], NA_UI_COMMAND_MOUSE_EXITED, unhoverItem, con);

      con->labels[curindex] = naNewLabel("", naMakeSize(32, 22));
      naSetLabelFontKind(con->labels[curindex], NA_FONT_KIND_MONOSPACE);
      naSetLabelTextAlignment(con->labels[curindex], NA_TEXT_ALIGNMENT_RIGHT);
      naSetLabelEnabled(con->labels[curindex], NA_FALSE);
      naAddSpaceChild(con->spaces[curindex], con->labels[curindex], naMakePos(0, 0));

      con->chars[curindex] = naNewLabel("", naMakeSize(50, 22));
      naSetLabelFontKind(con->chars[curindex], NA_FONT_KIND_MONOSPACE);
      naSetLabelTextAlignment(con->chars[curindex], NA_TEXT_ALIGNMENT_CENTER);
      naAddSpaceChild(con->spaces[curindex], con->chars[curindex], naMakePos(37, 0));
      
      naAddSpaceChild(columnspace, con->spaces[curindex], naMakePos(5, (15 - y) * 22 + 5));
      curindex++;
    }

    naAddSpaceChild(space, columnspace, naMakePos(x * 97, 64));
  }
  
  con->codeRadio = naNewRadio("Code", naMakeSize(64, 22));
  naAddUIReaction(con->codeRadio, NA_UI_COMMAND_PRESSED, switchASCIIDisplayMode, con);
  naAddSpaceChild(space, con->codeRadio, naMakePos(15, 10));

  con->escapeRadio = naNewRadio("Escape", naMakeSize(64, 22));
  naAddUIReaction(con->escapeRadio, NA_UI_COMMAND_PRESSED, switchASCIIDisplayMode, con);
  naAddSpaceChild(space, con->escapeRadio, naMakePos(15, 32));

  con->hexRadio = naNewRadio("Hex", naMakeSize(64, 22));
  naAddUIReaction(con->hexRadio, NA_UI_COMMAND_PRESSED, switchASCIIDisplayMode, con);
  naAddSpaceChild(space, con->hexRadio, naMakePos(112, 10));

  con->decRadio = naNewRadio("Dec", naMakeSize(64, 22));
  naAddUIReaction(con->decRadio, NA_UI_COMMAND_PRESSED, switchASCIIDisplayMode, con);
  naAddSpaceChild(space, con->decRadio, naMakePos(112, 32));
  
  con->info1 = naNewLabel("", naMakeSize(184, 44));
  naAddSpaceChild(space, con->info1, naMakePos(209, 5));

  con->info2 = naNewLabel("", naMakeSize(368, 44));
  naAddSpaceChild(space, con->info2, naMakePos(403, 5));

  con->useEscape = naGetPreferencesBool(BitPrefs[UseASCIIEscape]);
  con->useHex = naGetPreferencesBool(BitPrefs[UseASCIIHex]);

  redrawASCIIController(con);
  return con;
}



void bitClearASCIIController(BitASCIIController* con){
  naFree(con);
}



void naShowASCIIController(BitASCIIController* con){
  naShowWindow(con->window);
}
