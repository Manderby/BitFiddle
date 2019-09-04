
#include "ConverterController.h"
#include "ASCIIController.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"
#include "ManderAppAbout.h"

struct BitConverterController{
  NAWindow* window;

  NAButton* unsignedOption;
  NAButton* onesOption;
  NAButton* twosOption;
  NACheckBox* endiannessCheckBox;
  NAButton* helpButton;
  NAButton* preferencesButton;
  NAButton* asciiButton;

  NABuffer* bitarray;

  NATextField* inputdec;
  NALabel* labeldec;
  NALabel* bit8dec;
  NALabel* bit16dec;
  NALabel* bit32dec;
  NALabel* bit64dec;
  NATextBox* bitndec;

  NATextField* inputhex;
  NALabel* labelhex;
  NALabel* bit8hex;
  NALabel* bit16hex;
  NALabel* bit32hex;
  NALabel* bit64hex;
  NATextBox* bitnhex;

  NATextField* inputbin;
  NALabel* labelbin;
  NALabel* bit8bin;
  NALabel* bit16bin;
  NALabel* bit32bin;
  NALabel* bit64bin;
  NATextBox* bitnbin;

  NATextField* inputasc;
  NALabel* labelasc;
  NALabel* bit8asc;
  NALabel* bit16asc;
  NALabel* bit32asc;
  NALabel* bit64asc;
  NATextBox* bitnasc;
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



NABool closeConverterWindow(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controller);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
  naStopApplication();
//  NABool* shouldClose = arg;
//  *shouldClose = NA_TRUE; 
  return NA_TRUE;
}



NABool switchComplement(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  if(uielement == con->unsignedOption){
    bitSetConversionType(COMPUTE_UNSIGNED);
  }else if(uielement == con->onesOption){
    bitSetConversionType(COMPUTE_ONES_COMPLEMENT);
  }else if(uielement == con->twosOption){
    bitSetConversionType(COMPUTE_TWOS_COMPLEMENT);
  }else{
    naError("Unknown conversion type");
  }
  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool buttonPressed(void* controller, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(command);
  NA_UNUSED(arg);
  BitConverterController* con = controller;

  if(uielement == con->helpButton){
    mandShowAboutController();
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
  naSetCheckBoxState(con->endiannessCheckBox, byteswap);
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
  naSetButtonState(con->unsignedOption, conversiontype == COMPUTE_UNSIGNED);
  naSetButtonState(con->onesOption, conversiontype == COMPUTE_ONES_COMPLEMENT);
  naSetButtonState(con->twosOption, conversiontype == COMPUTE_TWOS_COMPLEMENT);
  
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
    naSetWindowTitle(con->window, naGetStringUTF8Pointer(windowtitle));
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



NATextField* createBitInputField(BitConverterController* con, NARect rect, NAReactionHandler handler){
  NATextField* textfield = naNewTextField(rect);
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

NALabel* createSystemLabelField(const NAUTF8Char* title, NARect rect){
  NALabel* labelfield = naNewLabel(title, rect);
  naSetLabelFontKind(labelfield, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_LEFT);
  naSetLabelEnabled(labelfield, NA_FALSE);
  return labelfield;
}

NALabel* createBitOutputField(NARect rect){
  NALabel* outputfield = naNewLabel("", rect);
  naSetLabelFontKind(outputfield, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(outputfield, NA_TEXT_ALIGNMENT_RIGHT);
  return outputfield;
}

NATextBox* createBitOutputBox(NARect rect){
  NATextBox* outputbox = naNewTextBox(rect);
  naSetLabelFontKind(outputbox, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(outputbox, NA_TEXT_ALIGNMENT_RIGHT);
  return outputbox;
}



BitConverterController* bitCreateConverterController(void){
  BitConverterController* con = naAlloc(BitConverterController);
  
  con->bitarray = naNewBuffer(NA_FALSE);

  NARect windowrect = naMakeRectS(60, 120, 777, 227);
  con->window = naNewWindow("Complement", windowrect, NA_FALSE);
  naAddUIReaction(con, con->window, NA_UI_COMMAND_CLOSES, closeConverterWindow);

  NASpace* space = naGetWindowContentSpace(con->window);
  double offsetx = 0;
  
  NASpace* settingspace = naNewSpace(naMakeRectS(offsetx, 0, 105, 227));
  naSetSpaceAlternateBackground(settingspace, NA_FALSE);

  con->unsignedOption = naNewTextOptionButton("U", naMakeRectS(10, 193, 24, 24));
  naAddUIReaction(con, con->unsignedOption, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, con->unsignedOption);

  con->onesOption = naNewTextOptionButton("1", naMakeRectS(40, 193, 24, 24));
  naAddUIReaction(con, con->onesOption, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, con->onesOption);

  con->twosOption = naNewTextOptionButton("2", naMakeRectS(70, 193, 24, 24));
  naAddUIReaction(con, con->twosOption, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, con->twosOption);

  con->endiannessCheckBox = naNewCheckBox(bitTranslate(BitFiddleConversionByteSwap), naMakeRectS(10, 163, 85, 22));
  naAddUIReaction(bitGetApplication(), con->endiannessCheckBox, NA_UI_COMMAND_PRESSED, bitSwitchAppEndianness);
  naAddSpaceChild(settingspace, con->endiannessCheckBox);

  NAString* helpImagePath = naNewBundleResourcePath("help", "png");
  con->helpButton = naNewImageButton(naGetStringUTF8Pointer(helpImagePath), naMakeRectS(10, 10, 20, 20));
  naAddUIReaction(con, con->helpButton, NA_UI_COMMAND_PRESSED, buttonPressed);
  naAddSpaceChild(settingspace, con->helpButton);
  naDelete(helpImagePath);

  NAString* prefImagePath = naNewBundleResourcePath("preferences", "png");
  con->preferencesButton = naNewImageButton(naGetStringUTF8Pointer(prefImagePath), naMakeRectS(40, 10, 20, 20));
  naAddUIReaction(con, con->preferencesButton, NA_UI_COMMAND_PRESSED, buttonPressed);
  naAddSpaceChild(settingspace, con->preferencesButton);
  naDelete(prefImagePath);

  NAString* ascImagePath = naNewBundleResourcePath("ascii", "png");
  con->asciiButton = naNewImageButton(naGetStringUTF8Pointer(ascImagePath), naMakeRectS(70, 10, 20, 20));
  naAddUIReaction(con, con->asciiButton, NA_UI_COMMAND_PRESSED, buttonPressed);
  naAddSpaceChild(settingspace, con->asciiButton);
  naDelete(ascImagePath);

  naAddSpaceChild(space, settingspace);
  offsetx += 105;

  // ////////////////

  NASpace* bitcountspace = naNewSpace(naMakeRectS(offsetx, 0, 25, 227));
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
  offsetx += 25;

  // ////////////////

  NASpace* decspace = naNewSpace(naMakeRectS(offsetx, 0, 140, 227));
  naSetSpaceAlternateBackground(decspace, NA_FALSE);

  con->inputdec = createBitInputField(con, naMakeRectS(10, 193, 120, 24), valueChangeDec);
  naAddSpaceChild(decspace, con->inputdec);
  con->labeldec = createSystemLabelField("Dec", naMakeRectS(10, 166, 30, 17));
  naAddSpaceChild(decspace, con->labeldec);
  con->bit8dec = createBitOutputField(naMakeRectS(92, 166, 38, 17));
  naAddSpaceChild(decspace, con->bit8dec);
  con->bit16dec = createBitOutputField(naMakeRectS(55, 144, 75, 17));
  naAddSpaceChild(decspace, con->bit16dec);
  con->bit32dec = createBitOutputField(naMakeRectS(10, 122, 120, 17));
  naAddSpaceChild(decspace, con->bit32dec);
  con->bit64dec = createBitOutputField(naMakeRectS(10, 83, 120, 34));
  naAddSpaceChild(decspace, con->bit64dec);
  con->bitndec = createBitOutputBox(naMakeRectS(10, 10, 120, 68));
  naAddSpaceChild(decspace, con->bitndec);

  naAddSpaceChild(space, decspace);
  offsetx += 140;

  // ////////////////

  NASpace* hexspace = naNewSpace(naMakeRectS(offsetx, 0, 120, 227));
  naSetSpaceAlternateBackground(hexspace, NA_TRUE);

  con->inputhex = createBitInputField(con, naMakeRectS(10, 193, 100, 24), valueChangeHex);
  naAddSpaceChild(hexspace, con->inputhex);
  con->labelhex = createSystemLabelField("Hex", naMakeRectS(10, 166, 30, 17));
  naAddSpaceChild(hexspace, con->labelhex);
  con->bit8hex = createBitOutputField(naMakeRectS(85, 166, 25, 17));
  naAddSpaceChild(hexspace, con->bit8hex);
  con->bit16hex = createBitOutputField(naMakeRectS(60, 144, 50, 17));
  naAddSpaceChild(hexspace, con->bit16hex);
  con->bit32hex = createBitOutputField(naMakeRectS(10, 122, 100, 17));
  naAddSpaceChild(hexspace, con->bit32hex);
  con->bit64hex = createBitOutputField(naMakeRectS(10, 83, 100, 34));
  naAddSpaceChild(hexspace, con->bit64hex);
  con->bitnhex = createBitOutputBox(naMakeRectS(10, 10, 100, 68));
  naAddSpaceChild(hexspace, con->bitnhex);

  naAddSpaceChild(space, hexspace);
  offsetx += 120;

  // ////////////////

  NASpace* binspace = naNewSpace(naMakeRectS(offsetx, 0, 305, 227));
  naSetSpaceAlternateBackground(binspace, NA_FALSE);

  con->inputbin = createBitInputField(con, naMakeRectS(10, 193, 285, 24), valueChangeBin);
  naAddSpaceChild(binspace, con->inputbin);
  con->labelbin = createSystemLabelField("Bin", naMakeRectS(10, 166, 30, 17));
  naAddSpaceChild(binspace, con->labelbin);
  con->bit8bin = createBitOutputField(naMakeRectS(220, 166, 75, 17));
  naAddSpaceChild(binspace, con->bit8bin);
  con->bit16bin = createBitOutputField(naMakeRectS(150, 144, 145, 17));
  naAddSpaceChild(binspace, con->bit16bin);
  con->bit32bin = createBitOutputField(naMakeRectS(10, 122, 285, 17));
  naAddSpaceChild(binspace, con->bit32bin);
  con->bit64bin = createBitOutputField(naMakeRectS(10, 83, 285, 34));
  naAddSpaceChild(binspace, con->bit64bin);
  con->bitnbin = createBitOutputBox(naMakeRectS(10, 10, 285, 68));
  naAddSpaceChild(binspace, con->bitnbin);

  naAddSpaceChild(space, binspace);
  offsetx += 305;

  // ////////////////

  NASpace* ascspace = naNewSpace(naMakeRectS(offsetx, 0, 65, 227));
  naSetSpaceAlternateBackground(ascspace, NA_TRUE);

  con->inputasc = createBitInputField(con, naMakeRectS(10, 193, 45, 24), valueChangeAsc);
  naAddSpaceChild(ascspace, con->inputasc);
  con->labelasc = createSystemLabelField("Asc", naMakeRectS(10, 166, 30, 17));
  naAddSpaceChild(ascspace, con->labelasc);
  con->bit8asc = createBitOutputField(naMakeRectS(35, 166, 20, 17));
  naAddSpaceChild(ascspace, con->bit8asc);
  con->bit16asc = createBitOutputField(naMakeRectS(25, 144, 30, 17));
  naAddSpaceChild(ascspace, con->bit16asc);
  con->bit32asc = createBitOutputField(naMakeRectS(10, 122, 45, 17));
  naAddSpaceChild(ascspace, con->bit32asc);
  con->bit64asc = createBitOutputField(naMakeRectS(10, 83, 45, 34));
  naAddSpaceChild(ascspace, con->bit64asc);
  con->bitnasc = createBitOutputBox(naMakeRectS(10, 10, 45, 68));
  naAddSpaceChild(ascspace, con->bitnasc);

  naAddSpaceChild(space, ascspace);
  offsetx += 65;

  // ////////////////

  windowrect.size.width = offsetx;
  naSetWindowRect(con->window, windowrect);

  naSetWindowFirstTabElement(con->window, con->inputdec);
  naSetUIElementNextTabElement(con->inputdec, con->inputhex);
  naSetUIElementNextTabElement(con->inputhex, con->inputbin);
  naSetUIElementNextTabElement(con->inputbin, con->inputasc);
  naSetUIElementNextTabElement(con->inputasc, con->inputdec);
  
  // ////////////////

  bitUpdateConverterController(con);
  return con;
}



void bitClearConverterController(BitConverterController* con){
  naRelease(con->bitarray);
  naReleaseUIElement(con->window);
  naFree(con);
}



void naShowConverterController(BitConverterController* con){
  naShowWindow(con->window);
}



void bitKeepConverterOnTop(BitConverterController* con, NABool onTop){
  naKeepWindowOnTop(con->window, onTop);
}
