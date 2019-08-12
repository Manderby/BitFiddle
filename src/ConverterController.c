
#include "ConverterController.h"
#include "ASCIIController.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"

struct BitConverterController{
  NAWindow* window;

  NARadio* unsignedRadio;
  NARadio* onesRadio;
  NARadio* twosRadio;
  NACheckbox* endiannessCheckbox;
  NAButton* helpButton;
  NAButton* preferencesButton;
  NAButton* asciiButton;

  NABuffer* bitarray;

  NATextField* inputdec;
  NATextField* inputhex;
  NATextField* inputbin;
  NATextField* inputasc;

  NALabel* bit8dec;
  NALabel* bit16dec;
  NALabel* bit32dec;
  NALabel* bit64dec;
  NATextBox* bitndec;

  NALabel* bit8hex;
  NALabel* bit16hex;
  NALabel* bit32hex;
  NALabel* bit64hex;
  NALabel* bitnhex;

  NALabel* bit8bin;
  NALabel* bit16bin;
  NALabel* bit32bin;
  NALabel* bit64bin;
  NALabel* bitnbin;

  NALabel* bit8asc;
  NALabel* bit16asc;
  NALabel* bit32asc;
  NALabel* bit64asc;
  NALabel* bitnasc;
};



NABool valueChangeDec(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  naSetTextFieldText(con->inputhex, "");
  naSetTextFieldText(con->inputbin, "");
  naSetTextFieldText(con->inputasc, "");

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  con->bitarray = naCreateBitArrayWithDecString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool valueChangeHex(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputbin, "");
  naSetTextFieldText(con->inputasc, "");

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  con->bitarray = naCreateBitArrayWithHexString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool valueChangeBin(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  naSetTextFieldText(con->inputasc, "");

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  con->bitarray = naCreateBitArrayWithBinString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool valueChangeAsc(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  naSetTextFieldText(con->inputbin, "");

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  con->bitarray = naCreateBitArrayWithAscString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}


void resetComplementValues(BitConverterController* con){
  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  naSetTextFieldText(con->inputbin, "");
  naSetTextFieldText(con->inputasc, "");

  naRelease(con->bitarray);
  con->bitarray = naNewBuffer(NA_FALSE);

  bitUpdateConverterController(con);
}



NABool switchComplement(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  if(uielement == con->unsignedRadio){
    bitSetConversionType(COMPUTE_UNSIGNED);
  }else if(uielement == con->onesRadio){
    bitSetConversionType(COMPUTE_ONES_COMPLEMENT);
  }else if(uielement == con->twosRadio){
    bitSetConversionType(COMPUTE_TWOS_COMPLEMENT);
  }else{
    naError("Unknown conversion type");
  }
  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool windowButtonPressed(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  if(uielement == con->helpButton){
  }else if(uielement == con->preferencesButton){
    bitShowPreferencesController();
  }else if(uielement == con->asciiButton){
    bitShowASCIIController();
  }else{
    naError("Unknown window button");
  }
  return NA_TRUE;
}



void fillOutputFieldWithString(NALabel* outputfield, NAString* string, NABool withdecsign){
  if(!string){
    naSetLabelText(outputfield, " ");
  }else if(naIsStringEmpty(string)){
    naSetLabelText(outputfield, "0");
  }else{
    if(withdecsign){
      NAString* signstring = naNewStringWithFormat("-%s", naGetStringUTF8Pointer(string)); 
      naSetLabelText(outputfield, naGetStringUTF8Pointer(signstring));
      naDelete(signstring);
    }else{
      naSetLabelText(outputfield, naGetStringUTF8Pointer(string));
    }
  }
}



void fillOutputTextBoxWithString(NALabel* outputtextbox, NAString* string, NABool withdecsign){
  if(!string){
    naSetTextBoxText(outputtextbox, " ");
  }else if(naIsStringEmpty(string)){
    naSetTextBoxText(outputtextbox, "0");
  }else{
    if(withdecsign){
      NAString* signstring = naNewStringWithFormat("-%s", naGetStringUTF8Pointer(string)); 
      naSetTextBoxText(outputtextbox, naGetStringUTF8Pointer(signstring));
      naDelete(signstring);
    }else{
      naSetTextBoxText(outputtextbox, naGetStringUTF8Pointer(string));
    }
  }
}



typedef enum{
  NUMBER_SYSTEM_DEC = 0,
  NUMBER_SYSTEM_HEX,
  NUMBER_SYSTEM_BIN,
  NUMBER_SYSTEM_ASC
} NumberSystem;



void fillOutputFieldWithBitArray(NAUIElement* outputfield, NumberSystem numbersystem, NABuffer* bitarray, NABool withdecsign){
  NAString* outstring;
  
  switch(numbersystem){
  case NUMBER_SYSTEM_DEC:
    if(bitarray && naGetBufferRange(bitarray).length && withdecsign && naGetBufferByteAtIndex(bitarray, naGetRangeiMax(naGetBufferRange(bitarray)))){
      NABuffer* twocomp = naNewBufferCopy(bitarray, naGetBufferRange(bitarray), NA_FALSE);
      naComputeBitArrayTwosComplement(twocomp);
      outstring = naNewStringDecWithBitArray(twocomp);
      naRelease(twocomp);
    }else{
      withdecsign = NA_FALSE;
      outstring = naNewStringDecWithBitArray(bitarray);
    }
    break;
  case NUMBER_SYSTEM_HEX:
    outstring = naNewStringHexWithBitArray(bitarray);
    break;
  case NUMBER_SYSTEM_BIN:
    outstring = naNewStringBinWithBitArray(bitarray);
    break;
  case NUMBER_SYSTEM_ASC:
    outstring = naNewStringAscWithBitArray(bitarray);
    break;
  }
  if(naGetUIElementType(outputfield) == NA_UI_LABEL){
    fillOutputFieldWithString(outputfield, outstring, withdecsign);
  }else{
    fillOutputTextBoxWithString(outputfield, outstring, withdecsign);
  }
  naDelete(outstring);
}



void bitUpdateConverterController(BitConverterController* con){
  NABuffer* bitarray8;
  NABuffer* bitarray16;
  NABuffer* bitarray32;
  NABuffer* bitarray64;
  NABuffer* bitarrayn;

  bitarray8  = naCreateBitArrayCopyWithFixedSize(con->bitarray, 8);
  bitarray16 = naCreateBitArrayCopyWithFixedSize(con->bitarray, 16);
  bitarray32 = naCreateBitArrayCopyWithFixedSize(con->bitarray, 32);
  bitarray64 = naCreateBitArrayCopyWithFixedSize(con->bitarray, 64);
  bitarrayn  = naCreateBitArrayCopyWithFixedSize(con->bitarray, -8);
  
  NABool byteswap = bitGetEndiannessSwap();
  naSetCheckboxState(con->endiannessCheckbox, byteswap);
  const NAUTF8Char* stringbyteswap = NA_NULL;
  if(byteswap){
    stringbyteswap = bitTranslate(BitFiddleConversionByteSwap);
    naComputeBitArraySwapBytes(bitarray8);
    naComputeBitArraySwapBytes(bitarray16);
    naComputeBitArraySwapBytes(bitarray32);
    naComputeBitArraySwapBytes(bitarray64);
    naComputeBitArraySwapBytes(bitarrayn);
  }

  ConversionType conversiontype = bitGetConversionType();
  naSetRadioState(con->unsignedRadio, conversiontype == COMPUTE_UNSIGNED);
  naSetRadioState(con->onesRadio, conversiontype == COMPUTE_ONES_COMPLEMENT);
  naSetRadioState(con->twosRadio, conversiontype == COMPUTE_TWOS_COMPLEMENT);
  
  const NAUTF8Char* stringconversion = NA_NULL;
  switch(conversiontype){
  case COMPUTE_UNSIGNED:
    stringconversion = bitTranslate(BitFiddleConversionUnsigned);
    break;
  case COMPUTE_ONES_COMPLEMENT:
    stringconversion = bitTranslate(BitFiddleConversionOnesComp);
    naComputeBitArrayOnesComplement(bitarray8);
    naComputeBitArrayOnesComplement(bitarray16);
    naComputeBitArrayOnesComplement(bitarray32);
    naComputeBitArrayOnesComplement(bitarray64);
    naComputeBitArrayOnesComplement(bitarrayn);
    break;
  case COMPUTE_TWOS_COMPLEMENT:
    stringconversion = bitTranslate(BitFiddleConversionTwosComp);
    naComputeBitArrayTwosComplement(bitarray8);
    naComputeBitArrayTwosComplement(bitarray16);
    naComputeBitArrayTwosComplement(bitarray32);
    naComputeBitArrayTwosComplement(bitarray64);
    naComputeBitArrayTwosComplement(bitarrayn);
    break;
  }
  
  NAString* windowtitle;
  if(stringbyteswap){
    windowtitle = naNewStringWithFormat("BitFiddle     %s     %s", stringconversion, stringbyteswap);
    naSetWindowTitle(windowtitle, naGetStringUTF8Pointer(windowtitle));
  }else{
    windowtitle = naNewStringWithFormat("BitFiddle     %s", stringconversion);
    naSetWindowTitle(con->window, naGetStringUTF8Pointer(windowtitle));
  }
  naDelete(windowtitle);
  
  if(conversiontype == COMPUTE_ONES_COMPLEMENT){
    fillOutputFieldWithString(con->bit8dec,  NA_NULL, NA_FALSE);
    fillOutputFieldWithString(con->bit16dec, NA_NULL, NA_FALSE);
    fillOutputFieldWithString(con->bit32dec, NA_NULL, NA_FALSE);
    fillOutputFieldWithString(con->bit64dec, NA_NULL, NA_FALSE);
    fillOutputTextBoxWithString(con->bitndec,  NA_NULL, NA_FALSE);
  }else if(conversiontype == COMPUTE_TWOS_COMPLEMENT){
    fillOutputFieldWithBitArray(con->bit8dec,  NUMBER_SYSTEM_DEC, bitarray8,  NA_TRUE);
    fillOutputFieldWithBitArray(con->bit16dec, NUMBER_SYSTEM_DEC, bitarray16, NA_TRUE);
    fillOutputFieldWithBitArray(con->bit32dec, NUMBER_SYSTEM_DEC, bitarray32, NA_TRUE);
    fillOutputFieldWithBitArray(con->bit64dec, NUMBER_SYSTEM_DEC, bitarray64, NA_TRUE);
    fillOutputFieldWithBitArray(con->bitndec,  NUMBER_SYSTEM_DEC, bitarrayn,  NA_TRUE);
  }else{
    fillOutputFieldWithBitArray(con->bit8dec,  NUMBER_SYSTEM_DEC, bitarray8,  NA_FALSE);
    fillOutputFieldWithBitArray(con->bit16dec, NUMBER_SYSTEM_DEC, bitarray16, NA_FALSE);
    fillOutputFieldWithBitArray(con->bit32dec, NUMBER_SYSTEM_DEC, bitarray32, NA_FALSE);
    fillOutputFieldWithBitArray(con->bit64dec, NUMBER_SYSTEM_DEC, bitarray64, NA_FALSE);
    fillOutputFieldWithBitArray(con->bitndec,  NUMBER_SYSTEM_DEC, bitarrayn,  NA_FALSE);
  }

  fillOutputFieldWithBitArray(con->bit8hex,  NUMBER_SYSTEM_HEX, bitarray8,  NA_FALSE);
  fillOutputFieldWithBitArray(con->bit16hex, NUMBER_SYSTEM_HEX, bitarray16, NA_FALSE);
  fillOutputFieldWithBitArray(con->bit32hex, NUMBER_SYSTEM_HEX, bitarray32, NA_FALSE);
  fillOutputFieldWithBitArray(con->bit64hex, NUMBER_SYSTEM_HEX, bitarray64, NA_FALSE);
  fillOutputFieldWithBitArray(con->bitnhex,  NUMBER_SYSTEM_HEX, bitarrayn,  NA_FALSE);

  fillOutputFieldWithBitArray(con->bit8bin,  NUMBER_SYSTEM_BIN, bitarray8,  NA_FALSE);
  fillOutputFieldWithBitArray(con->bit16bin, NUMBER_SYSTEM_BIN, bitarray16, NA_FALSE);
  fillOutputFieldWithBitArray(con->bit32bin, NUMBER_SYSTEM_BIN, bitarray32, NA_FALSE);
  fillOutputFieldWithBitArray(con->bit64bin, NUMBER_SYSTEM_BIN, bitarray64, NA_FALSE);
  fillOutputFieldWithBitArray(con->bitnbin,  NUMBER_SYSTEM_BIN, bitarrayn,  NA_FALSE);

  fillOutputFieldWithBitArray(con->bit8asc,  NUMBER_SYSTEM_ASC, bitarray8,  NA_FALSE);
  fillOutputFieldWithBitArray(con->bit16asc, NUMBER_SYSTEM_ASC, bitarray16, NA_FALSE);
  fillOutputFieldWithBitArray(con->bit32asc, NUMBER_SYSTEM_ASC, bitarray32, NA_FALSE);
  fillOutputFieldWithBitArray(con->bit64asc, NUMBER_SYSTEM_ASC, bitarray64, NA_FALSE);
  fillOutputFieldWithBitArray(con->bitnasc,  NUMBER_SYSTEM_ASC, bitarrayn,  NA_FALSE);

  naClearBitArray(bitarray8);
  naClearBitArray(bitarray16);
  naClearBitArray(bitarray32);
  naClearBitArray(bitarray64);
  naClearBitArray(bitarrayn);
}



NATextField* createBitInputField(BitConverterController* con, const NAUTF8Char* title, NARect rect, NAReactionHandler handler){
  NATextField* textfield = naNewTextField(title, rect);
  naSetTextFieldFontKind(textfield, NA_FONT_KIND_MONOSPACE);
  naSetTextFieldTextAlignment(textfield, NA_TEXT_ALIGNMENT_RIGHT);
  naAddUIReaction(con, textfield, NA_UI_COMMAND_EDITED, handler);
  return textfield;
}

NALabel* createBitLabelField(const NAUTF8Char* title, NARect rect){
  NALabel* labelfield = naNewLabel(title, rect);
  naSetLabelFontKind(labelfield, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelEnabled(labelfield, NA_FALSE);
  return labelfield;
}

NALabel* createBitOutputField(const NAUTF8Char* title, NARect rect){
  NALabel* outputfield = naNewLabel(title, rect);
  naSetLabelFontKind(outputfield, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(outputfield, NA_TEXT_ALIGNMENT_RIGHT);
  return outputfield;
}



BitConverterController* bitCreateConverterController(void){
  BitConverterController* con = naAlloc(BitConverterController);
  
  con->bitarray = naNewBuffer(NA_FALSE);

  NARect windowrect = naMakeRectS(60, 120, 777, 227);
  con->window = naNewWindow("Complement", windowrect, NA_FALSE);

  NASpace* space = naGetWindowContentSpace(con->window);
//  naAddUIReaction(con, space, NA_UI_COMMAND_KEYDOWN, keyDown);
  
  NASpace* settingspace = naNewSpace(naMakeRectS(0, 0, 120, 227));
  naSetSpaceAlternateBackground(settingspace, NA_FALSE);

  con->unsignedRadio = naNewRadio("Unsigned", naMakeRectS(10, 193, 100, 22));
  naAddUIReaction(con, con->unsignedRadio, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, con->unsignedRadio);

  con->onesRadio = naNewRadio("Ones", naMakeRectS(10, 171, 100, 22));
  naAddUIReaction(con, con->onesRadio, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, con->onesRadio);

  con->twosRadio = naNewRadio("Twos", naMakeRectS(10, 149, 100, 22));
  naAddUIReaction(con, con->twosRadio, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, con->twosRadio);

  con->endiannessCheckbox = naNewCheckbox("Endianness", naMakeRectS(10, 127, 100, 22));
  naAddUIReaction(con, con->endiannessCheckbox, NA_UI_COMMAND_PRESSED, bitSwitchAppEndianness);
  naAddSpaceChild(settingspace, con->endiannessCheckbox);

  con->helpButton = naNewButton("Help", naMakeRectS(10, 58, 100, 24));
  naAddUIReaction(con, con->helpButton, NA_UI_COMMAND_PRESSED, windowButtonPressed);
  naAddSpaceChild(settingspace, con->helpButton);

  con->preferencesButton = naNewButton("Preferences", naMakeRectS(10, 34, 100, 24));
  naAddUIReaction(con, con->preferencesButton, NA_UI_COMMAND_PRESSED, windowButtonPressed);
  naAddSpaceChild(settingspace, con->preferencesButton);

  con->asciiButton = naNewButton("ASCII", naMakeRectS(10, 10, 100, 24));
  naAddUIReaction(con, con->asciiButton, NA_UI_COMMAND_PRESSED, windowButtonPressed);
  naAddSpaceChild(settingspace, con->asciiButton);

  // ////////////////

  naAddSpaceChild(space, settingspace);

  NASpace* bitcountspace = naNewSpace(naMakeRectS(120, 0, 25, 227));
  naSetSpaceAlternateBackground(bitcountspace, NA_TRUE);

  NALabel* bit8label = createBitLabelField("8", naMakeRectS(0, 166, 25, 17));
  naAddSpaceChild(bitcountspace, bit8label);
  NALabel* bit16label = createBitLabelField("16", naMakeRectS(0, 144, 25, 17));
  naAddSpaceChild(bitcountspace, bit16label);
  NALabel* bit32label = createBitLabelField("32", naMakeRectS(0, 122, 25, 17));
  naAddSpaceChild(bitcountspace, bit32label);
  NALabel* bit64label = createBitLabelField("64", naMakeRectS(0, 83, 25, 34));
  naAddSpaceChild(bitcountspace, bit64label);
  NALabel* bitnlabel = createBitLabelField("n", naMakeRectS(0, 10, 25, 68));
  naAddSpaceChild(bitcountspace, bitnlabel);

  naAddSpaceChild(space, bitcountspace);

  // ////////////////

  NASpace* decspace = naNewSpace(naMakeRectS(142, 0, 150, 227));
  naSetSpaceAlternateBackground(decspace, NA_FALSE);

  con->inputdec = createBitInputField(con, "", naMakeRectS(10, 193, 130, 24), valueChangeDec);
  naAddSpaceChild(decspace, con->inputdec);

  con->bit8dec = createBitOutputField("-000", naMakeRectS(102, 166, 38, 17));
  naAddSpaceChild(decspace, con->bit8dec);
  con->bit16dec = createBitOutputField("-00 000", naMakeRectS(65, 144, 75, 17));
  naAddSpaceChild(decspace, con->bit16dec);
  con->bit32dec = createBitOutputField("-0 000 000 000", naMakeRectS(10, 122, 130, 17));
  naAddSpaceChild(decspace, con->bit32dec);
  con->bit64dec = createBitOutputField("-000 000 000 000\n000 000 000 000", naMakeRectS(10, 83, 130, 34));
  naAddSpaceChild(decspace, con->bit64dec);

  con->bitndec = naNewTextBox("-000 000 000 000\n000 000 000 000\n000 000 000 000\n000 000 000 000", naMakeRectS(10, 10, 130, 68));
  naSetTextBoxFontKind(con->bitndec, NA_FONT_KIND_MONOSPACE);
  naSetTextBoxTextAlignment(con->bitndec, NA_TEXT_ALIGNMENT_RIGHT);
  naAddSpaceChild(decspace, con->bitndec);

  naAddSpaceChild(space, decspace);

  // ////////////////

  NASpace* hexspace = naNewSpace(naMakeRectS(292, 0, 120, 227));
  naSetSpaceAlternateBackground(hexspace, NA_TRUE);

  con->inputhex = createBitInputField(con, "", naMakeRectS(10, 193, 100, 24), valueChangeHex);
  naAddSpaceChild(hexspace, con->inputhex);

  con->bit8hex = createBitOutputField("00", naMakeRectS(85, 166, 25, 17));
  naAddSpaceChild(hexspace, con->bit8hex);
  con->bit16hex = createBitOutputField("00 00", naMakeRectS(60, 144, 50, 17));
  naAddSpaceChild(hexspace, con->bit16hex);
  con->bit32hex = createBitOutputField("00 00 00 00", naMakeRectS(10, 122, 100, 17));
  naAddSpaceChild(hexspace, con->bit32hex);
  con->bit64hex = createBitOutputField("00 00 00 00\n00 00 00 00", naMakeRectS(10, 83, 100, 34));
  naAddSpaceChild(hexspace, con->bit64hex);
  con->bitnhex = createBitOutputField("00 00 00 00\n00 00 00 00\n00 00 00 00\n00 00 00 00", naMakeRectS(10, 10, 100, 68));
  naAddSpaceChild(hexspace, con->bitnhex);

  naAddSpaceChild(space, hexspace);

  // ////////////////

  NASpace* binspace = naNewSpace(naMakeRectS(412, 0, 290, 227));
  naSetSpaceAlternateBackground(binspace, NA_FALSE);

  con->inputbin = createBitInputField(con, "", naMakeRectS(10, 193, 270, 24), valueChangeBin);
  naAddSpaceChild(binspace, con->inputbin);

  con->bit8bin = createBitOutputField("00000000", naMakeRectS(212, 166, 68, 17));
  naAddSpaceChild(binspace, con->bit8bin);
  con->bit16bin = createBitOutputField("00000000 00000000", naMakeRectS(145, 144, 135, 17));
  naAddSpaceChild(binspace, con->bit16bin);
  con->bit32bin = createBitOutputField("00000000 00000000 00000000 00000000", naMakeRectS(10, 122, 270, 17));
  naAddSpaceChild(binspace, con->bit32bin);
  con->bit64bin = createBitOutputField("00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000", naMakeRectS(10, 83, 270, 34));
  naAddSpaceChild(binspace, con->bit64bin);
  con->bitnbin = createBitOutputField("00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000", naMakeRectS(10, 10, 270, 68));
  naAddSpaceChild(binspace, con->bitnbin);

  naAddSpaceChild(space, binspace);

  // ////////////////

  NASpace* ascspace = naNewSpace(naMakeRectS(702, 0, 75, 227));
  naSetSpaceAlternateBackground(ascspace, NA_TRUE);

  con->inputasc = createBitInputField(con, "", naMakeRectS(10, 193, 55, 24), valueChangeAsc);
  naAddSpaceChild(ascspace, con->inputasc);

  con->bit8asc = createBitOutputField("8", naMakeRectS(50, 166, 13, 17));
  naAddSpaceChild(ascspace, con->bit8asc);
  con->bit16asc = createBitOutputField("16", naMakeRectS(37, 144, 27, 17));
  naAddSpaceChild(ascspace, con->bit16asc);
  con->bit32asc = createBitOutputField("32", naMakeRectS(10, 122, 55, 17));
  naAddSpaceChild(ascspace, con->bit32asc);
  con->bit64asc = createBitOutputField("64\n64", naMakeRectS(10, 83, 55, 34));
  naAddSpaceChild(ascspace, con->bit64asc);
  con->bitnasc = createBitOutputField("n\nn\nn\nn", naMakeRectS(10, 10, 55, 68));
  naAddSpaceChild(ascspace, con->bitnasc);

  naAddSpaceChild(space, ascspace);

  // ////////////////

  bitUpdateConverterController(con);
  return con;
}



void bitClearConverterController(BitConverterController* con){
  naRelease(con->bitarray);
  naFree(con);
}



void naShowConverterController(BitConverterController* con){
  naShowWindow(con->window);
}



void bitKeepConverterOnTop(BitConverterController* con, NABool onTop){
  naKeepWindowOnTop(con->window, onTop);
}
