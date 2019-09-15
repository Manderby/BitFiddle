
#include "ConverterController.h"
#include "ASCIIController.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"
#include BIT_MANDERAPP_PATH(ManderAppAbout.h)
#include "BitFiddlePreferences.h"

struct BitConverterController{
  NABuffer* bitarray;
  NAWindow* window;

  NAButton* unsignedOption;
  NAButton* onesOption;
  NAButton* twosOption;
  NACheckBox* endiannessCheckBox;
  NAButton* helpButton;
  NAButton* preferencesButton;
  NAButton* asciiButton;

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



NABool valueChangeDec(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputhex, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uielement);
  con->bitarray = naCreateBitArrayWithDecString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool valueChangeHex(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputdec, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uielement);
  con->bitarray = naCreateBitArrayWithHexString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool valueChangeBin(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uielement);
  con->bitarray = naCreateBitArrayWithBinString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool valueChangeAsc(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}

  naRelease(con->bitarray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uielement);
  con->bitarray = naCreateBitArrayWithAscString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}


void resetComplementValues(BitConverterController* con){
  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitarray);
  con->bitarray = naNewBuffer(NA_FALSE);

  bitUpdateConverterController(con);
}



NABool closeConverterWindow(NAReaction reaction){
  NA_UNUSED(reaction);
  naStopApplication();
  return NA_TRUE;
}



NABool switchComplement(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  if(reaction.uielement == con->unsignedOption){
    bitSetConversionType(COMPUTE_UNSIGNED);
  }else if(reaction.uielement == con->onesOption){
    bitSetConversionType(COMPUTE_ONES_COMPLEMENT);
  }else if(reaction.uielement == con->twosOption){
    bitSetConversionType(COMPUTE_TWOS_COMPLEMENT);
  }else{
    #ifndef NDEBUG
      naError("Unknown conversion type");
    #endif
  }
  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool buttonPressed(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  if(reaction.uielement == con->helpButton){
    mandShowAboutController();
  }else if(reaction.uielement == con->preferencesButton){
    bitShowPreferencesController();
  }else if(reaction.uielement == con->asciiButton){
    bitShowASCIIController();
  }else{
    #ifndef NDEBUG
      naError("Unknown window button");
    #endif
  }
  return NA_TRUE;
}



void fillOutputFieldWithString(NALabel* outputfield, NAString* string, NABool withdecsign){
  if(!outputfield){return;}
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
  if(!outputtextbox){return;}
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
  if(!outputfield){return;}
  
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
  default:
    #ifndef NDEBUG
      naError("Invalid number system");
    #endif
    outstring = naNewStringHexWithBitArray(bitarray);
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
  naAddUIReaction(textfield, NA_UI_COMMAND_EDITED, handler, con);
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
  naZeron(con, naSizeof(BitConverterController));
  
  con->bitarray = naNewBuffer(NA_FALSE);

  NABool show16Bits = naGetPreferencesBool(BitPrefs[Show16Bits]);
  NABool showNBits = naGetPreferencesBool(BitPrefs[ShowNBits]);
  NABool showBin = naGetPreferencesBool(BitPrefs[ShowBin]);
  NABool showAsc = naGetPreferencesBool(BitPrefs[ShowAsc]);

  NAInt yposn = 10;
  NAInt ypos64 = showNBits ? yposn + 73 : yposn;
  NAInt ypos32 = ypos64 + 39;
  NAInt ypos16 = show16Bits ? ypos32 + 22 : ypos32;
  NAInt ypos8 = ypos16 + 22;
  NAInt yposinput = ypos8 + 27;
  NAInt yspaceheight = yposinput + 22 + 10;

  NARect windowrect = naMakeRectS(60, 120, 777, yspaceheight);
  con->window = naNewWindow("Complement", windowrect, NA_FALSE, BIT_WINDOW_TAG_CONVERTER);
  naAddUIReaction(con->window, NA_UI_COMMAND_CLOSES, closeConverterWindow, con);

  NASpace* space = naGetWindowContentSpace(con->window);
  double offsetx = 0;
  int alternateblock = 0;
  
  NASpace* settingspace = naNewSpace(naMakeRectS(offsetx, 0, 105, yspaceheight));
  naSetSpaceAlternateBackground(settingspace, alternateblock % 2);
  alternateblock++;

  con->unsignedOption = naNewTextOptionButton("U", naMakeRectS(10, yposinput, 24, 24));
  naAddUIReaction(con->unsignedOption, NA_UI_COMMAND_PRESSED, switchComplement, con);
  naAddSpaceChild(settingspace, con->unsignedOption);

  con->onesOption = naNewTextOptionButton("1", naMakeRectS(40, yposinput, 24, 24));
  naAddUIReaction(con->onesOption, NA_UI_COMMAND_PRESSED, switchComplement, con);
  naAddSpaceChild(settingspace, con->onesOption);

  con->twosOption = naNewTextOptionButton("2", naMakeRectS(70, yposinput, 24, 24));
  naAddUIReaction(con->twosOption, NA_UI_COMMAND_PRESSED, switchComplement, con);
  naAddSpaceChild(settingspace, con->twosOption);

  con->endiannessCheckBox = naNewCheckBox(bitTranslate(BitFiddleConversionByteSwap), naMakeRectS(10, ypos8, 85, 22));
  naAddUIReaction(con->endiannessCheckBox, NA_UI_COMMAND_PRESSED, bitSwitchAppEndianness, bitGetApplication());
  naAddSpaceChild(settingspace, con->endiannessCheckBox);

  NAUIImage* helpuiimage = bitGetImageAsset(BIT_IMAGE_ASSET_HELP_BUTTON);
  con->helpButton = naNewImageButton(helpuiimage, naMakeRectS(10, 10, 20, 20));
  naAddUIReaction(con->helpButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(settingspace, con->helpButton);

  NAUIImage* prefuiimage = bitGetImageAsset(BIT_IMAGE_ASSET_PREFS_BUTTON);
  con->preferencesButton = naNewImageButton(prefuiimage, naMakeRectS(40, 10, 20, 20));
  naAddUIReaction(con->preferencesButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(settingspace, con->preferencesButton);

  NAUIImage* ascuiimage = bitGetImageAsset(BIT_IMAGE_ASSET_ASCII_BUTTON);
  con->asciiButton = naNewImageButton(ascuiimage, naMakeRectS(70, 10, 20, 20));
  naAddUIReaction(con->asciiButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(settingspace, con->asciiButton);

  naAddSpaceChild(space, settingspace);
  offsetx += 105;

  // ////////////////

  NASpace* bitcountspace = naNewSpace(naMakeRectS(offsetx, 0, 25, yspaceheight));
  naSetSpaceAlternateBackground(bitcountspace, alternateblock % 2);
  alternateblock++;

  NALabel* bit8label = createBitLabelField("8", naMakeRectS(0, ypos8, 25, 17));
  naAddSpaceChild(bitcountspace, bit8label);
  if(show16Bits){
    NALabel* bit16label = createBitLabelField("16", naMakeRectS(0, ypos16, 25, 17));
    naAddSpaceChild(bitcountspace, bit16label);
  }
  NALabel* bit32label = createBitLabelField("32", naMakeRectS(0, ypos32, 25, 17));
  naAddSpaceChild(bitcountspace, bit32label);
  NALabel* bit64label = createBitLabelField("64", naMakeRectS(0, ypos64, 25, 34));
  naAddSpaceChild(bitcountspace, bit64label);
  if(showNBits){
    NALabel* bitnlabel = createBitLabelField("n", naMakeRectS(0, yposn, 25, 68));
    naAddSpaceChild(bitcountspace, bitnlabel);
  }

  naAddSpaceChild(space, bitcountspace);
  offsetx += 25;

  // ////////////////

  NASpace* decspace = naNewSpace(naMakeRectS(offsetx, 0, 140, yspaceheight));
  naSetSpaceAlternateBackground(decspace, alternateblock % 2);
  alternateblock++;

  con->inputdec = createBitInputField(con, naMakeRectS(10, yposinput, 120, 24), valueChangeDec);
  naAddSpaceChild(decspace, con->inputdec);
  con->labeldec = createSystemLabelField("Dec", naMakeRectS(10, ypos8, 30, 17));
  naAddSpaceChild(decspace, con->labeldec);
  con->bit8dec = createBitOutputField(naMakeRectS(92, ypos8, 38, 17));
  naAddSpaceChild(decspace, con->bit8dec);
  if(show16Bits){
    con->bit16dec = createBitOutputField(naMakeRectS(55, ypos16, 75, 17));
    naAddSpaceChild(decspace, con->bit16dec);
  }
  con->bit32dec = createBitOutputField(naMakeRectS(10, ypos32, 120, 17));
  naAddSpaceChild(decspace, con->bit32dec);
  con->bit64dec = createBitOutputField(naMakeRectS(10, ypos64, 120, 34));
  naAddSpaceChild(decspace, con->bit64dec);
  if(showNBits){
    con->bitndec = createBitOutputBox(naMakeRectS(10, yposn, 120, 68));
    naAddSpaceChild(decspace, con->bitndec);
  }

  naAddSpaceChild(space, decspace);
  offsetx += 140;

  // ////////////////

  NASpace* hexspace = naNewSpace(naMakeRectS(offsetx, 0, 120, yspaceheight));
  naSetSpaceAlternateBackground(hexspace, alternateblock % 2);
  alternateblock++;

  con->inputhex = createBitInputField(con, naMakeRectS(10, yposinput, 100, 24), valueChangeHex);
  naAddSpaceChild(hexspace, con->inputhex);
  con->labelhex = createSystemLabelField("Hex", naMakeRectS(10, ypos8, 30, 17));
  naAddSpaceChild(hexspace, con->labelhex);
  con->bit8hex = createBitOutputField(naMakeRectS(85, ypos8, 25, 17));
  naAddSpaceChild(hexspace, con->bit8hex);
  if(show16Bits){
    con->bit16hex = createBitOutputField(naMakeRectS(60, ypos16, 50, 17));
    naAddSpaceChild(hexspace, con->bit16hex);
  }
  con->bit32hex = createBitOutputField(naMakeRectS(10, ypos32, 100, 17));
  naAddSpaceChild(hexspace, con->bit32hex);
  con->bit64hex = createBitOutputField(naMakeRectS(10, ypos64, 100, 34));
  naAddSpaceChild(hexspace, con->bit64hex);
  if(showNBits){
    con->bitnhex = createBitOutputBox(naMakeRectS(10, yposn, 100, 68));
    naAddSpaceChild(hexspace, con->bitnhex);
  }

  naAddSpaceChild(space, hexspace);
  offsetx += 120;

  // ////////////////

  if(showBin){
    NASpace* binspace = naNewSpace(naMakeRectS(offsetx, 0, 305, yspaceheight));
    naSetSpaceAlternateBackground(binspace, alternateblock % 2);
  alternateblock++;

    con->inputbin = createBitInputField(con, naMakeRectS(10, yposinput, 285, 24), valueChangeBin);
    naAddSpaceChild(binspace, con->inputbin);
    con->labelbin = createSystemLabelField("Bin", naMakeRectS(10, ypos8, 30, 17));
    naAddSpaceChild(binspace, con->labelbin);
    con->bit8bin = createBitOutputField(naMakeRectS(220, ypos8, 75, 17));
    naAddSpaceChild(binspace, con->bit8bin);
    if(show16Bits){
      con->bit16bin = createBitOutputField(naMakeRectS(150, ypos16, 145, 17));
      naAddSpaceChild(binspace, con->bit16bin);
    }
    con->bit32bin = createBitOutputField(naMakeRectS(10, ypos32, 285, 17));
    naAddSpaceChild(binspace, con->bit32bin);
    con->bit64bin = createBitOutputField(naMakeRectS(10, ypos64, 285, 34));
    naAddSpaceChild(binspace, con->bit64bin);
    if(showNBits){
      con->bitnbin = createBitOutputBox(naMakeRectS(10, yposn, 285, 68));
      naAddSpaceChild(binspace, con->bitnbin);
    }

    naAddSpaceChild(space, binspace);
    offsetx += 305;
  }

  // ////////////////

  if(showAsc){
    NASpace* ascspace = naNewSpace(naMakeRectS(offsetx, 0, 65, yspaceheight));
    naSetSpaceAlternateBackground(ascspace, alternateblock % 2);
  alternateblock++;

    con->inputasc = createBitInputField(con, naMakeRectS(10, yposinput, 45, 24), valueChangeAsc);
    naAddSpaceChild(ascspace, con->inputasc);
    con->labelasc = createSystemLabelField("Asc", naMakeRectS(10, ypos8, 30, 17));
    naAddSpaceChild(ascspace, con->labelasc);
    con->bit8asc = createBitOutputField(naMakeRectS(35, ypos8, 20, 17));
    naAddSpaceChild(ascspace, con->bit8asc);
    if(show16Bits){
      con->bit16asc = createBitOutputField(naMakeRectS(25, ypos16, 30, 17));
      naAddSpaceChild(ascspace, con->bit16asc);
    }
    con->bit32asc = createBitOutputField(naMakeRectS(10, ypos32, 45, 17));
    naAddSpaceChild(ascspace, con->bit32asc);
    con->bit64asc = createBitOutputField(naMakeRectS(10, ypos64, 45, 34));
    naAddSpaceChild(ascspace, con->bit64asc);
    if(showNBits){
      con->bitnasc = createBitOutputBox(naMakeRectS(10, yposn, 45, 68));
      naAddSpaceChild(ascspace, con->bitnasc);
    }

    naAddSpaceChild(space, ascspace);
    offsetx += 65;
  }

  // ////////////////

  windowrect = naGetUIElementRect(con->window, NA_NULL, NA_FALSE);
  windowrect.size.width = offsetx;
  naSetWindowRect(con->window, windowrect);

  NATextField* nexttab;
  naSetWindowFirstTabElement(con->window, con->inputdec);
  naSetUIElementNextTabElement(con->inputdec, con->inputhex);
  nexttab = con->inputbin;
  if(!nexttab){nexttab = con->inputasc;}
  if(!nexttab){nexttab = con->inputdec;}
  naSetUIElementNextTabElement(con->inputhex, nexttab);
  if(showBin){
    nexttab = con->inputasc;
    if(!nexttab){nexttab = con->inputdec;}
    naSetUIElementNextTabElement(con->inputbin, nexttab);
  }
  if(showAsc){
    naSetUIElementNextTabElement(con->inputasc, con->inputdec);
  }
  
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
