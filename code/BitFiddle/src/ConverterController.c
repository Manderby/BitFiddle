
#include "ConverterController.h"
#include "ASCIIController.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"

struct BitConverterController{
  NABuffer* bitArray;
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
  NATextBox* bit64dec;
  NATextBox* bitndec;

  NATextField* inputhex;
  NALabel* labelhex;
  NALabel* bit8hex;
  NALabel* bit16hex;
  NALabel* bit32hex;
  NATextBox* bit64hex;
  NATextBox* bitnhex;

  NATextField* inputbin;
  NALabel* labelbin;
  NALabel* bit8bin;
  NALabel* bit16bin;
  NALabel* bit32bin;
  NATextBox* bit64bin;
  NATextBox* bitnbin;

  NATextField* inputasc;
  NALabel* labelasc;
  NALabel* bit8asc;
  NALabel* bit16asc;
  NALabel* bit32asc;
  NATextBox* bit64asc;
  NATextBox* bitnasc;
};



NABool bit_ValueChangeDec(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputhex, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitArray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithDecString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool bit_ValueChangeHex(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputdec, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitArray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithHexString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool bit_ValueChangeBin(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}

  naRelease(con->bitArray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithBinString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool bit_ValueChangeAsc(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputdec, "");
  naSetTextFieldText(con->inputhex, "");
  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}

  naRelease(con->bitArray);
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithAscString(instring);
  naDelete(instring);

  bitUpdateConverterController(con);
  return NA_TRUE;
}


//void bit_ResetComplementValues(BitConverterController* con){
//  naSetTextFieldText(con->inputdec, "");
//  naSetTextFieldText(con->inputhex, "");
//  if(con->inputbin){naSetTextFieldText(con->inputbin, "");}
//  if(con->inputasc){naSetTextFieldText(con->inputasc, "");}
//
//  naRelease(con->bitArray);
//  con->bitArray = naCreateBuffer(NA_FALSE);
//
//  bitUpdateConverterController(con);
//}



NABool bit_CloseConverterWindow(NAReaction reaction){
  NA_UNUSED(reaction);
  naStopApplication();
  return NA_TRUE;
}



NABool bit_SwitchComplement(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  if(reaction.uiElement == con->unsignedOption){
    bitSetBitConversionType(COMPUTE_UNSIGNED);
  }else if(reaction.uiElement == con->onesOption){
    bitSetBitConversionType(COMPUTE_ONES_COMPLEMENT);
  }else if(reaction.uiElement == con->twosOption){
    bitSetBitConversionType(COMPUTE_TWOS_COMPLEMENT);
  }else{
    #if NA_DEBUG
      naError("Unknown conversion type");
    #endif
  }
  bitUpdateConverterController(con);
  return NA_TRUE;
}



NABool bit_ButtonPressed(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  if(reaction.uiElement == con->helpButton){
    bitShowApplicationAboutController();
  }else if(reaction.uiElement == con->preferencesButton){
    bitShowApplicationPreferencesController();
  }else if(reaction.uiElement == con->asciiButton){
    bitShowApplicationASCIIController();
  }else{
    #if NA_DEBUG
      naError("Unknown window button");
    #endif
  }
  return NA_TRUE;
}



void bit_FillOutputFieldWithString(NALabel* outputField, NAString* string, NABool withDecSign){
  if(!outputField){return;}
  if(!string){
    naSetLabelText(outputField, " ");
  }else if(naIsStringEmpty(string)){
    naSetLabelText(outputField, "0");
  }else{
    if(withDecSign){
      NAString* signstring = naNewStringWithFormat("-%s", naGetStringUTF8Pointer(string)); 
      naSetLabelText(outputField, naGetStringUTF8Pointer(signstring));
      naDelete(signstring);
    }else{
      naSetLabelText(outputField, naGetStringUTF8Pointer(string));
    }
  }
}



void bit_FillOutputTextBoxWithString(NATextBox* outputTextBox, NAString* string, NABool withDecSign){
  if(!outputTextBox){return;}
  if(!string){
    naSetTextBoxText(outputTextBox, " ");
  }else if(naIsStringEmpty(string)){
    naSetTextBoxText(outputTextBox, "0");
  }else{
    if(withDecSign){
      NAString* signstring = naNewStringWithFormat("-%s", naGetStringUTF8Pointer(string)); 
      naSetTextBoxText(outputTextBox, naGetStringUTF8Pointer(signstring));
      naDelete(signstring);
    }else{
      naSetTextBoxText(outputTextBox, naGetStringUTF8Pointer(string));
    }
  }
}



typedef enum{
  NUMBER_SYSTEM_DEC = 0,
  NUMBER_SYSTEM_HEX,
  NUMBER_SYSTEM_BIN,
  NUMBER_SYSTEM_ASC
} BitNumberSystem;



void bit_FillOutputFieldWithBitArray(void* outputField, BitNumberSystem numberSystem, NABuffer* bitArray, NABool withDecSign){
  NAString* outstring;
  if(!outputField){return;}
  
  switch(numberSystem){
  case NUMBER_SYSTEM_DEC:
    if(bitArray && naGetBufferRange(bitArray).length && withDecSign && naGetBufferByteAtIndex(bitArray, (size_t)naGetRangeiMax(naGetBufferRange(bitArray)))){
      NABuffer* twocomp = naCreateBufferCopy(bitArray, naGetBufferRange(bitArray), NA_FALSE);
      bitComputeBitArrayTwosComplement(twocomp);
      outstring = bitNewStringDecWithBitArray(twocomp);
      naRelease(twocomp);
    }else{
      withDecSign = NA_FALSE;
      outstring = bitNewStringDecWithBitArray(bitArray);
    }
    break;
  case NUMBER_SYSTEM_HEX:
    outstring = bitNewStringHexWithBitArray(bitArray);
    break;
  case NUMBER_SYSTEM_BIN:
    outstring = bitNewStringBinWithBitArray(bitArray);
    break;
  case NUMBER_SYSTEM_ASC:
    outstring = bitNewStringAscWithBitArray(bitArray);
    break;
  default:
    #if NA_DEBUG
      naError("Invalid number system");
    #endif
    outstring = bitNewStringHexWithBitArray(bitArray);
    break;
  }
  if(naGetUIElementType(outputField) == NA_UI_LABEL){
    bit_FillOutputFieldWithString(outputField, outstring, withDecSign);
  }else{
    bit_FillOutputTextBoxWithString(outputField, outstring, withDecSign);
  }
  naDelete(outstring);
}



void bitUpdateConverterController(BitConverterController* con){
  NABuffer* bitArray8;
  NABuffer* bitArray16;
  NABuffer* bitArray32;
  NABuffer* bitArray64;
  NABuffer* bitArrayn;

  if(naIsBufferEmpty(con->bitArray))
  {
    naRelease(con->bitArray);
    con->bitArray = naCreateBufferWithConstData("\0", 1);
  }

  bitArray8  = bitCreateBitArrayCopyWithFixedSize(con->bitArray, 8);
  bitArray16 = bitCreateBitArrayCopyWithFixedSize(con->bitArray, 16);
  bitArray32 = bitCreateBitArrayCopyWithFixedSize(con->bitArray, 32);
  bitArray64 = bitCreateBitArrayCopyWithFixedSize(con->bitArray, 64);
  bitArrayn  = bitCreateBitArrayCopyWithFixedSize(con->bitArray, -8);
  
  NABool byteSwap = bitGetEndiannessSwap();
  naSetCheckBoxState(con->endiannessCheckBox, byteSwap);
  const NAUTF8Char* stringByteSwap = NA_NULL;
  if(byteSwap){
    stringByteSwap = bitTranslate(BitFiddleConversionByteSwap);
    bitComputeBitArraySwapBytes(bitArray8);
    bitComputeBitArraySwapBytes(bitArray16);
    bitComputeBitArraySwapBytes(bitArray32);
    bitComputeBitArraySwapBytes(bitArray64);
    bitComputeBitArraySwapBytes(bitArrayn);
  }

  BitConversionType conversionType = bitGetBitConversionType();
  naSetButtonState(con->unsignedOption, conversionType == COMPUTE_UNSIGNED);
  naSetButtonState(con->onesOption, conversionType == COMPUTE_ONES_COMPLEMENT);
  naSetButtonState(con->twosOption, conversionType == COMPUTE_TWOS_COMPLEMENT);
  
  const NAUTF8Char* stringConversion = NA_NULL;
  switch(conversionType){
  case COMPUTE_UNSIGNED:
    stringConversion = bitTranslate(BitFiddleConversionUnsigned);
    break;
  case COMPUTE_ONES_COMPLEMENT:
    stringConversion = bitTranslate(BitFiddleConversionOnesComp);
    bitComputeBitArrayOnesComplement(bitArray8);
    bitComputeBitArrayOnesComplement(bitArray16);
    bitComputeBitArrayOnesComplement(bitArray32);
    bitComputeBitArrayOnesComplement(bitArray64);
    bitComputeBitArrayOnesComplement(bitArrayn);
    break;
  case COMPUTE_TWOS_COMPLEMENT:
    stringConversion = bitTranslate(BitFiddleConversionTwosComp);
    bitComputeBitArrayTwosComplement(bitArray8);
    bitComputeBitArrayTwosComplement(bitArray16);
    bitComputeBitArrayTwosComplement(bitArray32);
    bitComputeBitArrayTwosComplement(bitArray64);
    bitComputeBitArrayTwosComplement(bitArrayn);
    break;
  }
  
  NAString* windowTitle;
  if(stringByteSwap){
    windowTitle = naNewStringWithFormat("BitFiddle     %s     %s", stringConversion, stringByteSwap);
    naSetWindowTitle(con->window, naGetStringUTF8Pointer(windowTitle));
  }else{
    windowTitle = naNewStringWithFormat("BitFiddle     %s", stringConversion);
    naSetWindowTitle(con->window, naGetStringUTF8Pointer(windowTitle));
  }
  naDelete(windowTitle);
  
  if(conversionType == COMPUTE_ONES_COMPLEMENT){
    bit_FillOutputFieldWithString(con->bit8dec,  NA_NULL, NA_FALSE);
    bit_FillOutputFieldWithString(con->bit16dec, NA_NULL, NA_FALSE);
    bit_FillOutputFieldWithString(con->bit32dec, NA_NULL, NA_FALSE);
    bit_FillOutputTextBoxWithString(con->bit64dec, NA_NULL, NA_FALSE);
    bit_FillOutputTextBoxWithString(con->bitndec,  NA_NULL, NA_FALSE);
  }else if(conversionType == COMPUTE_TWOS_COMPLEMENT){
    bit_FillOutputFieldWithBitArray(con->bit8dec,  NUMBER_SYSTEM_DEC, bitArray8,  NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bit16dec, NUMBER_SYSTEM_DEC, bitArray16, NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bit32dec, NUMBER_SYSTEM_DEC, bitArray32, NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bit64dec, NUMBER_SYSTEM_DEC, bitArray64, NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bitndec,  NUMBER_SYSTEM_DEC, bitArrayn,  NA_TRUE);
  }else{
    bit_FillOutputFieldWithBitArray(con->bit8dec,  NUMBER_SYSTEM_DEC, bitArray8,  NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit16dec, NUMBER_SYSTEM_DEC, bitArray16, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit32dec, NUMBER_SYSTEM_DEC, bitArray32, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit64dec, NUMBER_SYSTEM_DEC, bitArray64, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bitndec,  NUMBER_SYSTEM_DEC, bitArrayn,  NA_FALSE);
  }

  bit_FillOutputFieldWithBitArray(con->bit8hex,  NUMBER_SYSTEM_HEX, bitArray8,  NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit16hex, NUMBER_SYSTEM_HEX, bitArray16, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit32hex, NUMBER_SYSTEM_HEX, bitArray32, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit64hex, NUMBER_SYSTEM_HEX, bitArray64, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bitnhex,  NUMBER_SYSTEM_HEX, bitArrayn,  NA_FALSE);

  bit_FillOutputFieldWithBitArray(con->bit8bin,  NUMBER_SYSTEM_BIN, bitArray8,  NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit16bin, NUMBER_SYSTEM_BIN, bitArray16, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit32bin, NUMBER_SYSTEM_BIN, bitArray32, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit64bin, NUMBER_SYSTEM_BIN, bitArray64, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bitnbin,  NUMBER_SYSTEM_BIN, bitArrayn,  NA_FALSE);

  bit_FillOutputFieldWithBitArray(con->bit8asc,  NUMBER_SYSTEM_ASC, bitArray8,  NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit16asc, NUMBER_SYSTEM_ASC, bitArray16, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit32asc, NUMBER_SYSTEM_ASC, bitArray32, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit64asc, NUMBER_SYSTEM_ASC, bitArray64, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bitnasc,  NUMBER_SYSTEM_ASC, bitArrayn,  NA_FALSE);

  bitClearBitArray(bitArray8);
  bitClearBitArray(bitArray16);
  bitClearBitArray(bitArray32);
  bitClearBitArray(bitArray64);
  bitClearBitArray(bitArrayn);
}



NATextField* bit_CreateBitInputField(BitConverterController* con, double width, NAReactionHandler handler){
  NATextField* textfield = naNewTextField(width);
  NAFont* monoFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetTextFieldFont(textfield, monoFont);
  naSetTextFieldTextAlignment(textfield, NA_TEXT_ALIGNMENT_RIGHT);
  naAddUIReaction(textfield, NA_UI_COMMAND_EDITED, handler, con);
  naRelease(monoFont);
  return textfield;
}

NALabel* bit_CreateBitLabelField(const NAUTF8Char* title, double width){
  NALabel* labelfield = naNewLabel(title, width);
  NAFont* monoFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(labelfield, monoFont);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelEnabled(labelfield, NA_FALSE);
  naRelease(monoFont);
  return labelfield;
}

NALabel* bit_CreateSystemLabelField(const NAUTF8Char* title, double width){
  NALabel* labelfield = naNewLabel(title, width);
  NAFont* monoFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(labelfield, monoFont);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_LEFT);
  naSetLabelEnabled(labelfield, NA_FALSE);
  naRelease(monoFont);
  return labelfield;
}

NALabel* bit_CreateBitOutputField(double width){
  NALabel* outputField = naNewLabel("", width);
  NAFont* monoFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(outputField, monoFont);
  naSetLabelTextAlignment(outputField, NA_TEXT_ALIGNMENT_RIGHT);
  naRelease(monoFont);
  return outputField;
}

NATextBox* bit_CreateBitOutputBox(NASize size, NABool withScrolling){
  NATextBox* outputbox = naNewTextBox(size);
  NAFont* monoFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetTextBoxFont(outputbox, monoFont);
  naSetTextBoxTextAlignment(outputbox, NA_TEXT_ALIGNMENT_RIGHT);
  naSetTextBoxEditable(outputbox, NA_FALSE);
  naSetTextBoxUseVerticalScrolling(outputbox, withScrolling);
  naRelease(monoFont);
  return outputbox;
}



BitConverterController* bitCreateConverterController(void){
  BitConverterController* con = naAlloc(BitConverterController);
  naZeron(con, sizeof(BitConverterController));
  
  con->bitArray = naCreateBuffer(NA_FALSE);

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

  NARect windowrect = naMakeRectS(20., 20., 777., (double)yspaceheight);
  con->window = naNewWindow("Complement", windowrect, NA_FALSE, BIT_WINDOW_TAG_CONVERTER);
  naAddUIReaction(con->window, NA_UI_COMMAND_CLOSES, bit_CloseConverterWindow, con);

  NASpace* space = naGetWindowContentSpace(con->window);
  double offsetx = 0;
  int alternateblock = 0;
  
  // /////////////////////

  NASpace* settingSpace = naNewSpace(naMakeSize(110., (double)yspaceheight));
  naSetSpaceAlternateBackground(settingSpace, alternateblock % 2);
  alternateblock++;

  con->unsignedOption = naNewTextStateButton("U", NA_NULL, 28.);
  naAddUIReaction(con->unsignedOption, NA_UI_COMMAND_PRESSED, bit_SwitchComplement, con);
  naAddSpaceChild(settingSpace, con->unsignedOption, naMakePos(11., (double)yposinput));

  con->onesOption = naNewTextStateButton("1", NA_NULL, 28.);
  naAddUIReaction(con->onesOption, NA_UI_COMMAND_PRESSED, bit_SwitchComplement, con);
  naAddSpaceChild(settingSpace, con->onesOption, naMakePos(41., (double)yposinput));

  con->twosOption = naNewTextStateButton("2", NA_NULL, 28.);
  naAddUIReaction(con->twosOption, NA_UI_COMMAND_PRESSED, bit_SwitchComplement, con);
  naAddSpaceChild(settingSpace, con->twosOption, naMakePos(71., (double)yposinput));

  con->endiannessCheckBox = naNewCheckBox(bitTranslate(BitFiddleConversionByteSwap), 90);
  naAddUIReaction(con->endiannessCheckBox, NA_UI_COMMAND_PRESSED, bitSwitchAppEndianness, bitGetApplication());
  #if NA_OS == NA_OS_WINDOWS
    naAddSpaceChild(settingSpace, con->endiannessCheckBox, naMakePos(15., (double)ypos8));
  #else
    naAddSpaceChild(settingSpace, con->endiannessCheckBox, naMakePos(10., (double)ypos8));
  #endif

  NAUIImage* helpUIImage = bitGetImageAsset(BIT_IMAGE_ASSET_HELP_BUTTON);
  con->helpButton = naNewImagePushButton(helpUIImage, naMakeSize(20., 20.));
  naAddUIReaction(con->helpButton, NA_UI_COMMAND_PRESSED, bit_ButtonPressed, con);
  naAddSpaceChild(settingSpace, con->helpButton, naMakePos(13., 10.));

  NAUIImage* prefUIImage = bitGetImageAsset(BIT_IMAGE_ASSET_PREFS_BUTTON);
  con->preferencesButton = naNewImagePushButton(prefUIImage, naMakeSize(20., 20.));
  naAddUIReaction(con->preferencesButton, NA_UI_COMMAND_PRESSED, bit_ButtonPressed, con);
  naAddSpaceChild(settingSpace, con->preferencesButton, naMakePos(43., 10.));

  NAUIImage* ascUIImage = bitGetImageAsset(BIT_IMAGE_ASSET_ASCII_BUTTON);
  con->asciiButton = naNewImagePushButton(ascUIImage, naMakeSize(20., 20.));
  naAddUIReaction(con->asciiButton, NA_UI_COMMAND_PRESSED, bit_ButtonPressed, con);
  naAddSpaceChild(settingSpace, con->asciiButton, naMakePos(73., 10.));

  naAddSpaceChild(space, settingSpace, naMakePos(offsetx, 0.));
  offsetx += 110;

  // ////////////////

  NASpace* bitCountspace = naNewSpace(naMakeSize(25, (double)yspaceheight));
  naSetSpaceAlternateBackground(bitCountspace, alternateblock % 2);
  alternateblock++;

  NALabel* bit8label = bit_CreateBitLabelField("8", 25.);
  naAddSpaceChild(bitCountspace, bit8label, naMakePos(0., (double)ypos8));
  if(show16Bits){
    NALabel* bit16label = bit_CreateBitLabelField("16", 25.);
    naAddSpaceChild(bitCountspace, bit16label, naMakePos(0., (double)ypos16));
  }
  NALabel* bit32label = bit_CreateBitLabelField("32", 25.);
  naAddSpaceChild(bitCountspace, bit32label, naMakePos(0., (double)ypos32));
  NALabel* bit64label = bit_CreateBitLabelField("64", 25.);
  naAddSpaceChild(bitCountspace, bit64label, naMakePos(0., (double)ypos64));
  if(showNBits){
    NALabel* bitnlabel = bit_CreateBitLabelField("n", 25.);
    naAddSpaceChild(bitCountspace, bitnlabel, naMakePos(0., (double)yposn));
  }

  naAddSpaceChild(space, bitCountspace, naMakePos((double)offsetx, 0.));
  offsetx += 25;

  // ////////////////

  NASpace* decspace = naNewSpace(naMakeSize(140., (double)yspaceheight));
  naSetSpaceAlternateBackground(decspace, alternateblock % 2);
  alternateblock++;

  con->inputdec = bit_CreateBitInputField(con, 120., bit_ValueChangeDec);
  naAddSpaceChild(decspace, con->inputdec, naMakePos(10., (double)yposinput));
  con->labeldec = bit_CreateSystemLabelField("Dec", 30.);
  naAddSpaceChild(decspace, con->labeldec, naMakePos(10., (double)ypos8));
  con->bit8dec = bit_CreateBitOutputField(38.);
  naAddSpaceChild(decspace, con->bit8dec, naMakePos(92., (double)ypos8));
  if(show16Bits){
    con->bit16dec = bit_CreateBitOutputField(75.);
    naAddSpaceChild(decspace, con->bit16dec, naMakePos(55., (double)ypos16));
  }
  con->bit32dec = bit_CreateBitOutputField(120.);
  naAddSpaceChild(decspace, con->bit32dec, naMakePos(10., (double)ypos32));
  con->bit64dec = bit_CreateBitOutputBox(naMakeSize(120., 34.), NA_FALSE);
  naAddSpaceChild(decspace, con->bit64dec, naMakePos(10., (double)ypos64));
  if(showNBits){
    con->bitndec = bit_CreateBitOutputBox(naMakeSize(120., 68.), NA_TRUE);
    naAddSpaceChild(decspace, con->bitndec, naMakePos(10., (double)yposn));
  }

  naAddSpaceChild(space, decspace, naMakePos((double)offsetx, 0.));
  offsetx += 140;

  // ////////////////

  NASpace* hexspace = naNewSpace(naMakeSize(120., (double)yspaceheight));
  naSetSpaceAlternateBackground(hexspace, alternateblock % 2);
  alternateblock++;

  con->inputhex = bit_CreateBitInputField(con, 100., bit_ValueChangeHex);
  naAddSpaceChild(hexspace, con->inputhex, naMakePos(10., (double)yposinput));
  con->labelhex = bit_CreateSystemLabelField("Hex", 30.);
  naAddSpaceChild(hexspace, con->labelhex, naMakePos(10., (double)ypos8));
  con->bit8hex = bit_CreateBitOutputField(25.);
  naAddSpaceChild(hexspace, con->bit8hex, naMakePos(85., (double)ypos8));
  if(show16Bits){
    con->bit16hex = bit_CreateBitOutputField(50.);
    naAddSpaceChild(hexspace, con->bit16hex, naMakePos(60., (double)ypos16));
  }
  con->bit32hex = bit_CreateBitOutputField(100.);
  naAddSpaceChild(hexspace, con->bit32hex, naMakePos(10., (double)ypos32));
  con->bit64hex = bit_CreateBitOutputBox(naMakeSize(100., 34.), NA_FALSE);
  naAddSpaceChild(hexspace, con->bit64hex, naMakePos(10., (double)ypos64));
  if(showNBits){
    con->bitnhex = bit_CreateBitOutputBox(naMakeSize(100., 68.), NA_TRUE);
    naAddSpaceChild(hexspace, con->bitnhex, naMakePos(10., (double)yposn));
  }

  naAddSpaceChild(space, hexspace, naMakePos((double)offsetx, 0.));
  offsetx += 120;

  // ////////////////

  if(showBin){
    NASpace* binspace = naNewSpace(naMakeSize(305., (double)yspaceheight));
    naSetSpaceAlternateBackground(binspace, alternateblock % 2);
  alternateblock++;

    con->inputbin = bit_CreateBitInputField(con, 285., bit_ValueChangeBin);
    naAddSpaceChild(binspace, con->inputbin, naMakePos(10., (double)yposinput));
    con->labelbin = bit_CreateSystemLabelField("Bin", 30.);
    naAddSpaceChild(binspace, con->labelbin, naMakePos(10., (double)ypos8));
    con->bit8bin = bit_CreateBitOutputField(75.);
    naAddSpaceChild(binspace, con->bit8bin, naMakePos(220., (double)ypos8));
    if(show16Bits){
      con->bit16bin = bit_CreateBitOutputField(145.);
      naAddSpaceChild(binspace, con->bit16bin, naMakePos(150., (double)ypos16));
    }
    con->bit32bin = bit_CreateBitOutputField(285.);
    naAddSpaceChild(binspace, con->bit32bin, naMakePos(10., (double)ypos32));
    con->bit64bin = bit_CreateBitOutputBox(naMakeSize(285., 34.), NA_FALSE);
    naAddSpaceChild(binspace, con->bit64bin, naMakePos(10., (double)ypos64));
    if(showNBits){
      con->bitnbin = bit_CreateBitOutputBox(naMakeSize(285., 68.), NA_TRUE);
      naAddSpaceChild(binspace, con->bitnbin, naMakePos(10., (double)yposn));
    }

    naAddSpaceChild(space, binspace, naMakePos((double)offsetx, 0.));
    offsetx += 305;
  }

  // ////////////////

  if(showAsc){
    NASpace* ascspace = naNewSpace(naMakeSize(75., (double)yspaceheight));
    naSetSpaceAlternateBackground(ascspace, alternateblock % 2);
  alternateblock++;

    con->inputasc = bit_CreateBitInputField(con, 55., bit_ValueChangeAsc);
    naAddSpaceChild(ascspace, con->inputasc, naMakePos(10., (double)yposinput));
    con->labelasc = bit_CreateSystemLabelField("Asc", 30.);
    naAddSpaceChild(ascspace, con->labelasc, naMakePos(10., (double)ypos8));
    con->bit8asc = bit_CreateBitOutputField(30.);
    naAddSpaceChild(ascspace, con->bit8asc, naMakePos(35., (double)ypos8));
    if(show16Bits){
      con->bit16asc = bit_CreateBitOutputField(30.);
      naAddSpaceChild(ascspace, con->bit16asc, naMakePos(35., (double)ypos16));
    }
    con->bit32asc = bit_CreateBitOutputField(55.);
    naAddSpaceChild(ascspace, con->bit32asc, naMakePos(10., (double)ypos32));
    con->bit64asc = bit_CreateBitOutputBox(naMakeSize(55., 34.), NA_FALSE);
    naAddSpaceChild(ascspace, con->bit64asc, naMakePos(10., (double)ypos64));
    if(showNBits){
      con->bitnasc = bit_CreateBitOutputBox(naMakeSize(55., 68.), NA_TRUE);
      naAddSpaceChild(ascspace, con->bitnasc, naMakePos(10., (double)yposn));
    }

    naAddSpaceChild(space, ascspace, naMakePos((double)offsetx, 0.));
    offsetx += 75;
  }

  // ////////////////

  windowrect = naGetUIElementRect(con->window);
  windowrect.size.width = offsetx;

  naSetUIElementRect(con->window, windowrect);

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
  naDelete(con->window);
  naRelease(con->bitArray);
  naFree(con);
}



void bitShowConverterController(BitConverterController* con){
  naShowWindow(con->window);
}



void bitKeepConverterOnTop(BitConverterController* con, NABool onTop){
  naKeepWindowOnTop(con->window, onTop);
}



// Copyright(c) Tobias Stamm
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this softwareand associated documentation files(the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and /or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
