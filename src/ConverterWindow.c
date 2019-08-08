
#include "ConverterWindow.h"
#include "ASCIIWindow.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleCommon.h"
#include "BitFiddleApplication.h"

NAWindow* window;

NARadio* unsignedRadio;
NARadio* onesRadio;
NARadio* twosRadio;
NACheckbox* endiannessCheckbox;
NAButton* ASCIIButton;

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



NABool valueChangeDec(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  naSetTextFieldText(inputhex, "");
  naSetTextFieldText(inputbin, "");
  naSetTextFieldText(inputasc, "");

  naRelease(bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  bitarray = naCreateBitArrayWithDecString(instring);
  naDelete(instring);

  bitUpdateConverterWindow();
  return NA_TRUE;
}



NABool valueChangeHex(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  naSetTextFieldText(inputdec, "");
  naSetTextFieldText(inputbin, "");
  naSetTextFieldText(inputasc, "");

  naRelease(bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  bitarray = naCreateBitArrayWithHexString(instring);
  naDelete(instring);

  bitUpdateConverterWindow();
  return NA_TRUE;
}



NABool valueChangeBin(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  naSetTextFieldText(inputdec, "");
  naSetTextFieldText(inputhex, "");
  naSetTextFieldText(inputasc, "");

  naRelease(bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  bitarray = naCreateBitArrayWithBinString(instring);
  naDelete(instring);

  bitUpdateConverterWindow();
  return NA_TRUE;
}



NABool valueChangeAsc(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  naSetTextFieldText(inputdec, "");
  naSetTextFieldText(inputhex, "");
  naSetTextFieldText(inputbin, "");

  naRelease(bitarray);
  NAString* instring = naNewStringWithTextFieldText(uielement);
  bitarray = naCreateBitArrayWithAscString(instring);
  naDelete(instring);

  bitUpdateConverterWindow();
  return NA_TRUE;
}


void resetComplementValues(){
  naSetTextFieldText(inputdec, "");
  naSetTextFieldText(inputhex, "");
  naSetTextFieldText(inputbin, "");
  naSetTextFieldText(inputasc, "");

  naRelease(bitarray);
  bitarray = naNewBuffer(NA_FALSE);

  bitUpdateConverterWindow();
}



NABool switchComplement(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  if(uielement == unsignedRadio){
    bitSetConversionType(COMPUTE_UNSIGNED);
  }else if(uielement == onesRadio){
    bitSetConversionType(COMPUTE_ONES_COMPLEMENT);
  }else if(uielement == twosRadio){
    bitSetConversionType(COMPUTE_TWOS_COMPLEMENT);
  }else{
    naError("Unknown conversion type");
  }
  bitUpdateConverterWindow();
  return NA_TRUE;
}



NABool switchEndianness(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  bitSetEndiannessSwap(naGetCheckboxState(uielement));
  bitUpdateConverterWindow();
  return NA_TRUE;
}



NABool showASCII(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  bitShowASCIIWindow();
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



void bitUpdateConverterWindow(){
  NABuffer* bitarray8;
  NABuffer* bitarray16;
  NABuffer* bitarray32;
  NABuffer* bitarray64;
  NABuffer* bitarrayn;

  bitarray8  = naCreateBitArrayCopyWithFixedSize(bitarray, 8);
  bitarray16 = naCreateBitArrayCopyWithFixedSize(bitarray, 16);
  bitarray32 = naCreateBitArrayCopyWithFixedSize(bitarray, 32);
  bitarray64 = naCreateBitArrayCopyWithFixedSize(bitarray, 64);
  bitarrayn  = naCreateBitArrayCopyWithFixedSize(bitarray, -8);
  
  NABool byteswap = bitGetEndiannessSwap();
  naSetCheckboxState(endiannessCheckbox, byteswap);
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
  naSetRadioState(unsignedRadio, conversiontype == COMPUTE_UNSIGNED);
  naSetRadioState(onesRadio, conversiontype == COMPUTE_ONES_COMPLEMENT);
  naSetRadioState(twosRadio, conversiontype == COMPUTE_TWOS_COMPLEMENT);
  
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
    naSetWindowTitle(window, naGetStringUTF8Pointer(windowtitle));
  }
  naDelete(windowtitle);
  
  if(conversiontype == COMPUTE_ONES_COMPLEMENT){
    fillOutputFieldWithString(bit8dec,  NA_NULL, NA_FALSE);
    fillOutputFieldWithString(bit16dec, NA_NULL, NA_FALSE);
    fillOutputFieldWithString(bit32dec, NA_NULL, NA_FALSE);
    fillOutputFieldWithString(bit64dec, NA_NULL, NA_FALSE);
    fillOutputTextBoxWithString(bitndec,  NA_NULL, NA_FALSE);
  }else if(conversiontype == COMPUTE_TWOS_COMPLEMENT){
    fillOutputFieldWithBitArray(bit8dec,  NUMBER_SYSTEM_DEC, bitarray8,  NA_TRUE);
    fillOutputFieldWithBitArray(bit16dec, NUMBER_SYSTEM_DEC, bitarray16, NA_TRUE);
    fillOutputFieldWithBitArray(bit32dec, NUMBER_SYSTEM_DEC, bitarray32, NA_TRUE);
    fillOutputFieldWithBitArray(bit64dec, NUMBER_SYSTEM_DEC, bitarray64, NA_TRUE);
    fillOutputFieldWithBitArray(bitndec,  NUMBER_SYSTEM_DEC, bitarrayn,  NA_TRUE);
  }else{
    fillOutputFieldWithBitArray(bit8dec,  NUMBER_SYSTEM_DEC, bitarray8,  NA_FALSE);
    fillOutputFieldWithBitArray(bit16dec, NUMBER_SYSTEM_DEC, bitarray16, NA_FALSE);
    fillOutputFieldWithBitArray(bit32dec, NUMBER_SYSTEM_DEC, bitarray32, NA_FALSE);
    fillOutputFieldWithBitArray(bit64dec, NUMBER_SYSTEM_DEC, bitarray64, NA_FALSE);
    fillOutputFieldWithBitArray(bitndec,  NUMBER_SYSTEM_DEC, bitarrayn,  NA_FALSE);
  }

  fillOutputFieldWithBitArray(bit8hex,  NUMBER_SYSTEM_HEX, bitarray8,  NA_FALSE);
  fillOutputFieldWithBitArray(bit16hex, NUMBER_SYSTEM_HEX, bitarray16, NA_FALSE);
  fillOutputFieldWithBitArray(bit32hex, NUMBER_SYSTEM_HEX, bitarray32, NA_FALSE);
  fillOutputFieldWithBitArray(bit64hex, NUMBER_SYSTEM_HEX, bitarray64, NA_FALSE);
  fillOutputFieldWithBitArray(bitnhex,  NUMBER_SYSTEM_HEX, bitarrayn,  NA_FALSE);

  fillOutputFieldWithBitArray(bit8bin,  NUMBER_SYSTEM_BIN, bitarray8,  NA_FALSE);
  fillOutputFieldWithBitArray(bit16bin, NUMBER_SYSTEM_BIN, bitarray16, NA_FALSE);
  fillOutputFieldWithBitArray(bit32bin, NUMBER_SYSTEM_BIN, bitarray32, NA_FALSE);
  fillOutputFieldWithBitArray(bit64bin, NUMBER_SYSTEM_BIN, bitarray64, NA_FALSE);
  fillOutputFieldWithBitArray(bitnbin,  NUMBER_SYSTEM_BIN, bitarrayn,  NA_FALSE);

  fillOutputFieldWithBitArray(bit8asc,  NUMBER_SYSTEM_ASC, bitarray8,  NA_FALSE);
  fillOutputFieldWithBitArray(bit16asc, NUMBER_SYSTEM_ASC, bitarray16, NA_FALSE);
  fillOutputFieldWithBitArray(bit32asc, NUMBER_SYSTEM_ASC, bitarray32, NA_FALSE);
  fillOutputFieldWithBitArray(bit64asc, NUMBER_SYSTEM_ASC, bitarray64, NA_FALSE);
  fillOutputFieldWithBitArray(bitnasc,  NUMBER_SYSTEM_ASC, bitarrayn,  NA_FALSE);

  naClearBitArray(bitarray8);
  naClearBitArray(bitarray16);
  naClearBitArray(bitarray32);
  naClearBitArray(bitarray64);
  naClearBitArray(bitarrayn);
}



NATextField* createBitInputField(const NAUTF8Char* title, NARect rect, NAReactionHandler handler){
  NATextField* textfield = naNewTextField(title, rect);
  naSetTextFieldFontKind(textfield, NA_FONT_KIND_MONOSPACE);
  naSetTextFieldTextAlignment(textfield, NA_TEXT_ALIGNMENT_RIGHT);
  naAddUIReaction(NA_NULL, textfield, NA_UI_COMMAND_EDITED, handler);
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



NAWindow* bitCreateConverterWindow(){
  bitarray = naNewBuffer(NA_FALSE);

  NARect windowrect = naMakeRectS(60, 120, 777, 227);
  window = naNewWindow("Complement", windowrect, NA_FALSE);
  
  NASpace* space = naGetWindowContentSpace(window);
  
  NASpace* settingspace = naNewSpace(naMakeRectS(0, 0, 120, 227));
  naSetSpaceAlternateBackground(settingspace, NA_FALSE);

  unsignedRadio = naNewRadio("Unsigned", naMakeRectS(10, 193, 100, 22));
  naAddUIReaction(NA_NULL, unsignedRadio, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, unsignedRadio);

  onesRadio = naNewRadio("Ones", naMakeRectS(10, 171, 100, 22));
  naAddUIReaction(NA_NULL, onesRadio, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, onesRadio);

  twosRadio = naNewRadio("Twos", naMakeRectS(10, 149, 100, 22));
  naAddUIReaction(NA_NULL, twosRadio, NA_UI_COMMAND_PRESSED, switchComplement);
  naAddSpaceChild(settingspace, twosRadio);

  endiannessCheckbox = naNewCheckbox("Endianness", naMakeRectS(10, 127, 100, 22));
  naAddUIReaction(NA_NULL, endiannessCheckbox, NA_UI_COMMAND_PRESSED, switchEndianness);
  naAddSpaceChild(settingspace, endiannessCheckbox);

  ASCIIButton = naNewButton("ASCII", naMakeRectS(10, 10, 100, 24));
  naAddUIReaction(NA_NULL, ASCIIButton, NA_UI_COMMAND_PRESSED, showASCII);
  naAddSpaceChild(settingspace, ASCIIButton);

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

  inputdec = createBitInputField("", naMakeRectS(10, 193, 130, 24), valueChangeDec);
  naAddSpaceChild(decspace, inputdec);

  bit8dec = createBitOutputField("-000", naMakeRectS(102, 166, 38, 17));
  naAddSpaceChild(decspace, bit8dec);
  bit16dec = createBitOutputField("-00 000", naMakeRectS(65, 144, 75, 17));
  naAddSpaceChild(decspace, bit16dec);
  bit32dec = createBitOutputField("-0 000 000 000", naMakeRectS(10, 122, 130, 17));
  naAddSpaceChild(decspace, bit32dec);
  bit64dec = createBitOutputField("-000 000 000 000\n000 000 000 000", naMakeRectS(10, 83, 130, 34));
  naAddSpaceChild(decspace, bit64dec);

  bitndec = naNewTextBox("-000 000 000 000\n000 000 000 000\n000 000 000 000\n000 000 000 000", naMakeRectS(10, 10, 130, 68));
  naSetTextBoxFontKind(bitndec, NA_FONT_KIND_MONOSPACE);
  naSetTextBoxTextAlignment(bitndec, NA_TEXT_ALIGNMENT_RIGHT);
  naAddSpaceChild(decspace, bitndec);

  naAddSpaceChild(space, decspace);

  // ////////////////

  NASpace* hexspace = naNewSpace(naMakeRectS(292, 0, 120, 227));
  naSetSpaceAlternateBackground(hexspace, NA_TRUE);

  inputhex = createBitInputField("", naMakeRectS(10, 193, 100, 24), valueChangeHex);
  naAddSpaceChild(hexspace, inputhex);

  bit8hex = createBitOutputField("00", naMakeRectS(85, 166, 25, 17));
  naAddSpaceChild(hexspace, bit8hex);
  bit16hex = createBitOutputField("00 00", naMakeRectS(60, 144, 50, 17));
  naAddSpaceChild(hexspace, bit16hex);
  bit32hex = createBitOutputField("00 00 00 00", naMakeRectS(10, 122, 100, 17));
  naAddSpaceChild(hexspace, bit32hex);
  bit64hex = createBitOutputField("00 00 00 00\n00 00 00 00", naMakeRectS(10, 83, 100, 34));
  naAddSpaceChild(hexspace, bit64hex);
  bitnhex = createBitOutputField("00 00 00 00\n00 00 00 00\n00 00 00 00\n00 00 00 00", naMakeRectS(10, 10, 100, 68));
  naAddSpaceChild(hexspace, bitnhex);

  naAddSpaceChild(space, hexspace);

  // ////////////////

  NASpace* binspace = naNewSpace(naMakeRectS(412, 0, 290, 227));
  naSetSpaceAlternateBackground(binspace, NA_FALSE);

  inputbin = createBitInputField("", naMakeRectS(10, 193, 270, 24), valueChangeBin);
  naAddSpaceChild(binspace, inputbin);

  bit8bin = createBitOutputField("00000000", naMakeRectS(212, 166, 68, 17));
  naAddSpaceChild(binspace, bit8bin);
  bit16bin = createBitOutputField("00000000 00000000", naMakeRectS(145, 144, 135, 17));
  naAddSpaceChild(binspace, bit16bin);
  bit32bin = createBitOutputField("00000000 00000000 00000000 00000000", naMakeRectS(10, 122, 270, 17));
  naAddSpaceChild(binspace, bit32bin);
  bit64bin = createBitOutputField("00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000", naMakeRectS(10, 83, 270, 34));
  naAddSpaceChild(binspace, bit64bin);
  bitnbin = createBitOutputField("00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000\n00000000 00000000 00000000 00000000", naMakeRectS(10, 10, 270, 68));
  naAddSpaceChild(binspace, bitnbin);

  naAddSpaceChild(space, binspace);

  // ////////////////

  NASpace* ascspace = naNewSpace(naMakeRectS(702, 0, 75, 227));
  naSetSpaceAlternateBackground(ascspace, NA_TRUE);

  inputasc = createBitInputField("", naMakeRectS(10, 193, 55, 24), valueChangeAsc);
  naAddSpaceChild(ascspace, inputasc);

  bit8asc = createBitOutputField("8", naMakeRectS(50, 166, 13, 17));
  naAddSpaceChild(ascspace, bit8asc);
  bit16asc = createBitOutputField("16", naMakeRectS(37, 144, 27, 17));
  naAddSpaceChild(ascspace, bit16asc);
  bit32asc = createBitOutputField("32", naMakeRectS(10, 122, 55, 17));
  naAddSpaceChild(ascspace, bit32asc);
  bit64asc = createBitOutputField("64\n64", naMakeRectS(10, 83, 55, 34));
  naAddSpaceChild(ascspace, bit64asc);
  bitnasc = createBitOutputField("n\nn\nn\nn", naMakeRectS(10, 10, 55, 68));
  naAddSpaceChild(ascspace, bitnasc);

  naAddSpaceChild(space, ascspace);

  // ////////////////

  bitUpdateConverterWindow();
  return window;
}

