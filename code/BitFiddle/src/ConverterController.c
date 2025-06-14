
#include "ConverterController.h"
#include "AsciiController.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"
#include "BitFiddlePreferences.h"

struct BitConverterController{
  NABuffer* bitArray;
  NABool swapEndianness;
  BitConversionType conversionType;
  
  NAWindow* window;

  NAButton* unsignedOption;
  NAButton* onesOption;
  NAButton* twosOption;
  NACheckBox* endiannessCheckBox;
  NAButton* helpButton;
  NAButton* preferencesButton;
  NAButton* asciiButton;

  NATextField* inputDec;
  NALabel* labelDec;
  NALabel* bit8Dec;
  NALabel* bit16Dec;
  NALabel* bit32Dec;
  NATextBox* bit64Dec;
  NATextBox* bitnDec;

  NATextField* inputHex;
  NALabel* labelHex;
  NALabel* bit8Hex;
  NALabel* bit16Hex;
  NALabel* bit32Hex;
  NATextBox* bit64Hex;
  NATextBox* bitnHex;

  NATextField* inputBin;
  NALabel* labelBin;
  NALabel* bit8Bin;
  NALabel* bit16Bin;
  NALabel* bit32Bin;
  NATextBox* bit64Bin;
  NATextBox* bitnBin;

  NATextField* inputAsc;
  NALabel* labelAsc;
  NALabel* bit8Asc;
  NALabel* bit16Asc;
  NALabel* bit32Asc;
  NATextBox* bit64Asc;
  NATextBox* bitnAsc;
};



void bit_ValueChangeDec(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputHex, "");
  if(con->inputBin) {naSetTextFieldText(con->inputBin, "");}
  if(con->inputAsc) {naSetTextFieldText(con->inputAsc, "");}

  naRelease(con->bitArray);
  NAString* inString = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithDecString(inString);
  naDelete(inString);

  bitUpdateConverterController(con);
}



void bit_ValueChangeHex(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputDec, "");
  if(con->inputBin) {naSetTextFieldText(con->inputBin, "");}
  if(con->inputAsc) {naSetTextFieldText(con->inputAsc, "");}

  naRelease(con->bitArray);
  NAString* inString = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithHexString(inString);
  naDelete(inString);

  bitUpdateConverterController(con);
}



void bit_ValueChangeBin(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputDec, "");
  naSetTextFieldText(con->inputHex, "");
  if(con->inputAsc) {naSetTextFieldText(con->inputAsc, "");}

  naRelease(con->bitArray);
  NAString* inString = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithBinString(inString);
  naDelete(inString);

  bitUpdateConverterController(con);
}



void bit_ValueChangeAsc(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  naSetTextFieldText(con->inputDec, "");
  naSetTextFieldText(con->inputHex, "");
  if(con->inputBin) {naSetTextFieldText(con->inputBin, "");}

  naRelease(con->bitArray);
  NAString* inString = naNewStringWithTextFieldText(reaction.uiElement);
  con->bitArray = bitCreateBitArrayWithAscString(inString);
  naDelete(inString);

  bitUpdateConverterController(con);
}



void bit_CloseConverterWindow(NAReaction reaction) {
  NA_UNUSED(reaction);
  naStopApplication();
}



void bit_SetBitConversionType(
  BitConverterController* con,
  BitConversionType conversionType)
{
  con->conversionType = conversionType;
  bitSetPrefsComplementEncoding(conversionType);
  bitUpdateConverterController(con);
}



void bit_SwitchComplement(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  if(reaction.uiElement == con->unsignedOption) {
    bit_SetBitConversionType(con, COMPUTE_UNSIGNED);
  }else if(reaction.uiElement == con->onesOption) {
    bit_SetBitConversionType(con, COMPUTE_ONES_COMPLEMENT);
  }else if(reaction.uiElement == con->twosOption) {
    bit_SetBitConversionType(con, COMPUTE_TWOS_COMPLEMENT);
  }else{
    #if NA_DEBUG
      naError("Unknown conversion type");
    #endif
  }
  
  bitUpdateConverterController(con);
}



void bit_ButtonPressed(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  if(reaction.uiElement == con->helpButton) {
    bitShowApplicationAboutController();
  }else if(reaction.uiElement == con->preferencesButton) {
    bitShowApplicationPreferencesController();
  }else if(reaction.uiElement == con->asciiButton) {
    bitShowApplicationAsciiController();
  }else{
    #if NA_DEBUG
      naError("Unknown window button");
    #endif
  }
}



void bit_FillOutputFieldWithString(
  NALabel* outputField,
  NAString* string,
  NABool withDecSign)
{
  if(!outputField) {return;}
  if(!string) {
    naSetLabelText(outputField, " ");
  }else if(naIsStringEmpty(string)) {
    naSetLabelText(outputField, "0");
  }else{
    if(withDecSign) {
      NAUTF8Char* signString = naAllocSprintf(
        NA_TRUE,
        "-%s",
        naGetStringUTF8Pointer(string)); 
      naSetLabelText(outputField, signString);
    }else{
      naSetLabelText(outputField, naGetStringUTF8Pointer(string));
    }
  }
}



void bit_FillOutputTextBoxWithString(
  NATextBox* outputTextBox,
  NAString* string,
  NABool withDecSign)
{
  if(!outputTextBox) {return;}
  if(!string) {
    naSetTextBoxText(outputTextBox, " ");
  }else if(naIsStringEmpty(string)) {
    naSetTextBoxText(outputTextBox, "0");
  }else{
    if(withDecSign) {
      NAUTF8Char* signString = naAllocSprintf(
        NA_TRUE,
        "-%s",
        naGetStringUTF8Pointer(string)); 
      naSetTextBoxText(outputTextBox, signString);
    }else{
      naSetTextBoxText(outputTextBox, naGetStringUTF8Pointer(string));
    }
  }
}



void bit_FillOutputFieldWithBitArray(
  void* outputField,
  BitNumberSystem numberSystem,
  NABuffer* bitArray,
  NABool withDecSign)
{
  if(!outputField) {return;}
  
  NAString* outString = NA_NULL;
  
  if(bitArray) {
    NABool signBit;
    
    switch(numberSystem) {
    case NUMBER_SYSTEM_DEC:
      signBit = naGetBufferByteAtIndex(
        bitArray,
        (size_t)naGetRangei64Max(naGetBufferRange(bitArray)));
      if(naGetBufferRange(bitArray).length && withDecSign && signBit) {
        NABuffer* twoComp = naCreateBufferCopy(
          bitArray,
          naGetBufferRange(bitArray),
          NA_FALSE);
        bitComputeBitArrayTwosComplement(twoComp);
        outString = bitNewStringDecWithBitArray(twoComp);
        naRelease(twoComp);
      }else{
        withDecSign = NA_FALSE;
        outString = bitNewStringDecWithBitArray(bitArray);
      }
      break;
    case NUMBER_SYSTEM_HEX:
      outString = bitNewStringHexWithBitArray(bitArray);
      break;
    case NUMBER_SYSTEM_BIN:
      outString = bitNewStringBinWithBitArray(bitArray);
      break;
    case NUMBER_SYSTEM_ASC:
      outString = bitNewStringAscWithBitArray(bitArray);
      break;
    default:
      #if NA_DEBUG
        naError("Invalid number system");
      #endif
      outString = bitNewStringHexWithBitArray(bitArray);
      break;
    }
  }
  
  if(naGetUIElementType(outputField) == NA_UI_LABEL) {
    bit_FillOutputFieldWithString(outputField, outString, withDecSign);
  }else{
    bit_FillOutputTextBoxWithString(outputField, outString, withDecSign);
  }
  
  if(outString)
    naDelete(outString);
}



void bitUpdateConverterController(BitConverterController* con) {
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
  
  NABool byteSwap = con->swapEndianness;
  naSetCheckBoxState(con->endiannessCheckBox, byteSwap);
  const NAUTF8Char* stringByteSwap = NA_NULL;
  if(byteSwap) {
    stringByteSwap = bitTranslate(BitFiddleConversionByteSwap);
    bitComputeBitArraySwapBytes(bitArray8);
    bitComputeBitArraySwapBytes(bitArray16);
    bitComputeBitArraySwapBytes(bitArray32);
    bitComputeBitArraySwapBytes(bitArray64);
    bitComputeBitArraySwapBytes(bitArrayn);
  }

  BitConversionType conversionType = con->conversionType;
  naSetButtonState(con->unsignedOption, conversionType == COMPUTE_UNSIGNED);
  naSetButtonState(con->onesOption, conversionType == COMPUTE_ONES_COMPLEMENT);
  naSetButtonState(con->twosOption, conversionType == COMPUTE_TWOS_COMPLEMENT);
  
  const NAUTF8Char* stringConversion = NA_NULL;
  switch(conversionType) {
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
  default:
    break;
  }
  
  NAString* windowTitle;
  if(stringByteSwap) {
    windowTitle = naNewStringWithFormat(
      "BitFiddle     %s     %s",
      stringConversion,
      stringByteSwap);
    naSetWindowTitle(con->window, naGetStringUTF8Pointer(windowTitle));
  }else{
    windowTitle = naNewStringWithFormat(
      "BitFiddle     %s",
      stringConversion);
    naSetWindowTitle(con->window, naGetStringUTF8Pointer(windowTitle));
  }
  naDelete(windowTitle);
  
  if(conversionType == COMPUTE_ONES_COMPLEMENT) {
    bit_FillOutputFieldWithBitArray(con->bit8Dec,  NUMBER_SYSTEM_DEC, NA_NULL, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit16Dec, NUMBER_SYSTEM_DEC, NA_NULL, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit32Dec, NUMBER_SYSTEM_DEC, NA_NULL, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit64Dec, NUMBER_SYSTEM_DEC, NA_NULL, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bitnDec,  NUMBER_SYSTEM_DEC, NA_NULL, NA_FALSE);
  }else if(conversionType == COMPUTE_TWOS_COMPLEMENT) {
    bit_FillOutputFieldWithBitArray(con->bit8Dec,  NUMBER_SYSTEM_DEC, bitArray8,  NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bit16Dec, NUMBER_SYSTEM_DEC, bitArray16, NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bit32Dec, NUMBER_SYSTEM_DEC, bitArray32, NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bit64Dec, NUMBER_SYSTEM_DEC, bitArray64, NA_TRUE);
    bit_FillOutputFieldWithBitArray(con->bitnDec,  NUMBER_SYSTEM_DEC, bitArrayn,  NA_TRUE);
  }else{
    bit_FillOutputFieldWithBitArray(con->bit8Dec,  NUMBER_SYSTEM_DEC, bitArray8,  NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit16Dec, NUMBER_SYSTEM_DEC, bitArray16, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit32Dec, NUMBER_SYSTEM_DEC, bitArray32, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bit64Dec, NUMBER_SYSTEM_DEC, bitArray64, NA_FALSE);
    bit_FillOutputFieldWithBitArray(con->bitnDec,  NUMBER_SYSTEM_DEC, bitArrayn,  NA_FALSE);
  }

  bit_FillOutputFieldWithBitArray(con->bit8Hex,  NUMBER_SYSTEM_HEX, bitArray8,  NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit16Hex, NUMBER_SYSTEM_HEX, bitArray16, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit32Hex, NUMBER_SYSTEM_HEX, bitArray32, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit64Hex, NUMBER_SYSTEM_HEX, bitArray64, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bitnHex,  NUMBER_SYSTEM_HEX, bitArrayn,  NA_FALSE);

  bit_FillOutputFieldWithBitArray(con->bit8Bin,  NUMBER_SYSTEM_BIN, bitArray8,  NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit16Bin, NUMBER_SYSTEM_BIN, bitArray16, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit32Bin, NUMBER_SYSTEM_BIN, bitArray32, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit64Bin, NUMBER_SYSTEM_BIN, bitArray64, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bitnBin,  NUMBER_SYSTEM_BIN, bitArrayn,  NA_FALSE);

  bit_FillOutputFieldWithBitArray(con->bit8Asc,  NUMBER_SYSTEM_ASC, bitArray8,  NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit16Asc, NUMBER_SYSTEM_ASC, bitArray16, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit32Asc, NUMBER_SYSTEM_ASC, bitArray32, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bit64Asc, NUMBER_SYSTEM_ASC, bitArray64, NA_FALSE);
  bit_FillOutputFieldWithBitArray(con->bitnAsc,  NUMBER_SYSTEM_ASC, bitArrayn,  NA_FALSE);

  bitClearBitArray(bitArray8);
  bitClearBitArray(bitArray16);
  bitClearBitArray(bitArray32);
  bitClearBitArray(bitArray64);
  bitClearBitArray(bitArrayn);
}



NATextField* bit_CreateBitInputField(
  BitConverterController* con,
  double width,
  NAReactionCallback callback)
{
  NATextField* textfield = naNewTextField(width);
  NAFont* monoFont = bitGetMonospaceFont();
  naSetTextFieldFont(textfield, monoFont);
  naSetTextFieldTextAlignment(textfield, NA_TEXT_ALIGNMENT_RIGHT);
  naAddUIReaction(textfield, NA_UI_COMMAND_EDITED, callback, con);
  return textfield;
}

NALabel* bit_CreateBitLabelField(
  const NAUTF8Char* title,
  double width)
{
  NALabel* labelfield = naNewLabel(title, width);
  NAFont* monoFont = bitGetMonospaceFont();
  naSetLabelFont(labelfield, monoFont);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelEnabled(labelfield, NA_FALSE);
  return labelfield;
}

NALabel* bit_CreateSystemLabelField(
  const NAUTF8Char* title,
  double width)
{
  NALabel* labelfield = naNewLabel(title, width);
  NAFont* monoFont = bitGetMonospaceFont();
  naSetLabelFont(labelfield, monoFont);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_LEFT);
  naSetLabelEnabled(labelfield, NA_FALSE);
  return labelfield;
}

NALabel* bit_CreateBitOutputField(
  double width)
{
  NALabel* outputField = naNewLabel("", width);
  NAFont* monoFont = bitGetMonospaceFont();
  naSetLabelFont(outputField, monoFont);
  naSetLabelTextAlignment(outputField, NA_TEXT_ALIGNMENT_RIGHT);
  return outputField;
}

NATextBox* bit_CreateBitOutputBox(
  NASize size,
  NABool withScrolling)
{
  NATextBox* outputbox = naNewTextBox(size);
  NAFont* monoFont = bitGetMonospaceFont();
  naSetTextBoxFont(outputbox, monoFont);
  naSetTextBoxTextAlignment(outputbox, NA_TEXT_ALIGNMENT_RIGHT);
  naSetTextBoxEditable(outputbox, NA_FALSE);
  naSetTextBoxUseVerticalScrolling(outputbox, withScrolling);
  return outputbox;
}

void bit_SwitchEndianness(NAReaction reaction) {
  BitConverterController* con = reaction.controller;
  
  con->swapEndianness = !con->swapEndianness;
  bitSetPrefsSwapEndianness(con->swapEndianness);
  bitUpdateConverterController(con);
}

void bit_SwitchBitConversionType(NAReaction reaction) {
  BitConverterController* con = reaction.controller;

  const NAKeyStroke* keyStroke = naGetCurrentKeyStroke();
  switch(naGetKeyStrokeKeyCode(keyStroke)) {
  case NA_KEYCODE_0:
    bit_SetBitConversionType(con, COMPUTE_UNSIGNED);
    break;
  case NA_KEYCODE_1:
    bit_SetBitConversionType(con, COMPUTE_ONES_COMPLEMENT);
    break;
  case NA_KEYCODE_2:
    bit_SetBitConversionType(con, COMPUTE_TWOS_COMPLEMENT);
    break;
  default:
    #if NA_DEBUG
      naError("Undefined keyCode");
    #endif
    break;
  }
}



BitConverterController* bitAllocConverterController(void) {
  BitConverterController* con = naAlloc(BitConverterController);
  naZeron(con, sizeof(BitConverterController));
  
  con->bitArray = naCreateBuffer(NA_FALSE);
  con->swapEndianness = bitGetPrefsSwapEndianness();
  con->conversionType = bitGetPrefsComplementEncoding();

  NABool show16Bits = bitGetPrefsShow16Bits();
  NABool showNBits = bitGetPrefsShowNBits();
  NABool showBin = bitGetPrefsShowBin();
  NABool showAsc = bitGetPrefsShowAsc();

  int64 yPosn = 10;
  int64 yPos64 = showNBits ? yPosn + 73 : yPosn;
  int64 yPos32 = yPos64 + 39;
  int64 yPos16 = show16Bits ? yPos32 + 22 : yPos32;
  int64 yPos8 = yPos16 + 22;
  int64 yPosInput = yPos8 + 27;
  int64 ySpaceHeight = yPosInput + 22 + 10;

  NARect windowRect = naMakeRectS(20., 20., 777., (double)ySpaceHeight);
  con->window = naNewWindow(
    "Complement",
    windowRect,
    0);
    
  naSetWindowStorageTag(con->window, BIT_WINDOW_TAG_CONVERTER);
    
  naAddUIReaction(
    con->window,
    NA_UI_COMMAND_CLOSES,
    bit_CloseConverterWindow,
    con);

  uint32 modifier = 0;
  #if NA_OS == NA_OS_MAC_OS_X
    modifier = NA_KEY_MODIFIER_COMMAND;
  #elif NA_OS == NA_OS_WINDOWS
    modifier = NA_KEY_MODIFIER_CONTROL;
  #endif
  naAddUIKeyboardShortcut(
    con->window,
    naNewKeyStroke(NA_KEYCODE_E, modifier),
    bit_SwitchEndianness,
    con);
  naAddUIKeyboardShortcut(
    con->window,
    naNewKeyStroke(NA_KEYCODE_0, modifier),
    bit_SwitchBitConversionType,
    con);
  naAddUIKeyboardShortcut(
    con->window,
    naNewKeyStroke(NA_KEYCODE_1, modifier),
    bit_SwitchBitConversionType,
    con);
  naAddUIKeyboardShortcut(
    con->window,
    naNewKeyStroke(NA_KEYCODE_2, modifier),
    bit_SwitchBitConversionType,
    con);

  NASpace* space = naGetWindowContentSpace(con->window);
  double offsetX = 0;
  int alternateBlock = 0;
  
  // /////////////////////

  NASpace* settingSpace = naNewSpace(naMakeSize(120., (double)ySpaceHeight));
  naSetSpaceAlternateBackground(settingSpace, alternateBlock % 2);
  alternateBlock++;

  con->unsignedOption = naNewTextStateButton("U", NA_NULL, 28.);
  naAddUIReaction(
    con->unsignedOption,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchComplement,
    con);
  naAddSpaceChild(
    settingSpace,
    con->unsignedOption,
    naMakePos(11., (double)yPosInput));

  con->onesOption = naNewTextStateButton("1", NA_NULL, 28.);
  naAddUIReaction(
    con->onesOption,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchComplement,
    con);
  naAddSpaceChild(
    settingSpace,
    con->onesOption,
    naMakePos(46., (double)yPosInput));

  con->twosOption = naNewTextStateButton("2", NA_NULL, 28.);
  naAddUIReaction(
    con->twosOption,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchComplement,
    con);
  naAddSpaceChild(
    settingSpace,
    con->twosOption,
    naMakePos(81., (double)yPosInput));

  con->endiannessCheckBox = naNewCheckBox(
    bitTranslate(BitFiddleConversionByteSwap),
    100);
  naAddUIReaction(
    con->endiannessCheckBox,
    NA_UI_COMMAND_PRESSED,
    bit_SwitchEndianness,
    con);
  #if NA_OS == NA_OS_WINDOWS
    naAddSpaceChild(
      settingSpace,
      con->endiannessCheckBox,
      naMakePos(15., (double)yPos8));
  #else
    naAddSpaceChild(
      settingSpace,
      con->endiannessCheckBox,
      naMakePos(10., (double)yPos8));
  #endif

  con->helpButton = naNewImagePushButton(
    bitGetImageAsset(BIT_IMAGE_ASSET_HELP_BUTTON),
    naMakeSize(20., 20.));
  naAddUIReaction(
    con->helpButton,
    NA_UI_COMMAND_PRESSED,
    bit_ButtonPressed,
    con);
  naAddSpaceChild(
    settingSpace,
    con->helpButton,
    naMakePos(13., 10.));

  con->preferencesButton = naNewImagePushButton(
    bitGetImageAsset(BIT_IMAGE_ASSET_PREFS_BUTTON),
    naMakeSize(20., 20.));
  naAddUIReaction(
    con->preferencesButton,
    NA_UI_COMMAND_PRESSED,
    bit_ButtonPressed,
    con);
  naAddSpaceChild(
    settingSpace,
    con->preferencesButton,
    naMakePos(48., 10.));

  con->asciiButton = naNewImagePushButton(
    bitGetImageAsset(BIT_IMAGE_ASSET_ASCII_BUTTON),
    naMakeSize(20., 20.));
  naAddUIReaction(
    con->asciiButton,
    NA_UI_COMMAND_PRESSED,
    bit_ButtonPressed,
    con);
  naAddSpaceChild(
    settingSpace,
    con->asciiButton,
    naMakePos(83., 10.));

  naAddSpaceChild(space, settingSpace, naMakePos(offsetX, 0.));
  offsetX += 120;

  // ////////////////

  NASpace* bitCountspace = naNewSpace(naMakeSize(25, (double)ySpaceHeight));
  naSetSpaceAlternateBackground(bitCountspace, alternateBlock % 2);
  alternateBlock++;

  NALabel* bit8label = bit_CreateBitLabelField("8", 25.);
  naAddSpaceChild(bitCountspace, bit8label, naMakePos(0., (double)yPos8));
  if(show16Bits) {
    NALabel* bit16label = bit_CreateBitLabelField("16", 25.);
    naAddSpaceChild(bitCountspace, bit16label, naMakePos(0., (double)yPos16));
  }
  NALabel* bit32label = bit_CreateBitLabelField("32", 25.);
  naAddSpaceChild(bitCountspace, bit32label, naMakePos(0., (double)yPos32));
  NALabel* bit64label = bit_CreateBitLabelField("64", 25.);
  naAddSpaceChild(bitCountspace, bit64label, naMakePos(0., (double)yPos64));
  if(showNBits) {
    NALabel* bitnlabel = bit_CreateBitLabelField("n", 25.);
    naAddSpaceChild(bitCountspace, bitnlabel, naMakePos(0., (double)yPosn));
  }

  naAddSpaceChild(space, bitCountspace, naMakePos((double)offsetX, 0.));
  offsetX += 25;

  // ////////////////

  NASpace* decSpace = naNewSpace(naMakeSize(140., (double)ySpaceHeight));
  naSetSpaceAlternateBackground(decSpace, alternateBlock % 2);
  alternateBlock++;

  con->inputDec = bit_CreateBitInputField(con, 120., bit_ValueChangeDec);
  naAddSpaceChild(decSpace, con->inputDec, naMakePos(10., (double)yPosInput));
  con->labelDec = bit_CreateSystemLabelField("Dec", 30.);
  naAddSpaceChild(decSpace, con->labelDec, naMakePos(10., (double)yPos8));
  con->bit8Dec = bit_CreateBitOutputField(38.);
  naAddSpaceChild(decSpace, con->bit8Dec, naMakePos(92., (double)yPos8));
  if(show16Bits) {
    con->bit16Dec = bit_CreateBitOutputField(75.);
    naAddSpaceChild(decSpace, con->bit16Dec, naMakePos(55., (double)yPos16));
  }
  con->bit32Dec = bit_CreateBitOutputField(120.);
  naAddSpaceChild(decSpace, con->bit32Dec, naMakePos(10., (double)yPos32));
  con->bit64Dec = bit_CreateBitOutputBox(naMakeSize(120., 34.), NA_FALSE);
  naAddSpaceChild(decSpace, con->bit64Dec, naMakePos(10., (double)yPos64));
  if(showNBits) {
    con->bitnDec = bit_CreateBitOutputBox(naMakeSize(120., 68.), NA_TRUE);
    naAddSpaceChild(decSpace, con->bitnDec, naMakePos(10., (double)yPosn));
  }

  naAddSpaceChild(space, decSpace, naMakePos((double)offsetX, 0.));
  offsetX += 140;

  // ////////////////

  NASpace* hexSpace = naNewSpace(naMakeSize(120., (double)ySpaceHeight));
  naSetSpaceAlternateBackground(hexSpace, alternateBlock % 2);
  alternateBlock++;

  con->inputHex = bit_CreateBitInputField(con, 100., bit_ValueChangeHex);
  naAddSpaceChild(hexSpace, con->inputHex, naMakePos(10., (double)yPosInput));
  con->labelHex = bit_CreateSystemLabelField("Hex", 30.);
  naAddSpaceChild(hexSpace, con->labelHex, naMakePos(10., (double)yPos8));
  con->bit8Hex = bit_CreateBitOutputField(25.);
  naAddSpaceChild(hexSpace, con->bit8Hex, naMakePos(85., (double)yPos8));
  if(show16Bits) {
    con->bit16Hex = bit_CreateBitOutputField(50.);
    naAddSpaceChild(hexSpace, con->bit16Hex, naMakePos(60., (double)yPos16));
  }
  con->bit32Hex = bit_CreateBitOutputField(100.);
  naAddSpaceChild(hexSpace, con->bit32Hex, naMakePos(10., (double)yPos32));
  con->bit64Hex = bit_CreateBitOutputBox(naMakeSize(100., 34.), NA_FALSE);
  naAddSpaceChild(hexSpace, con->bit64Hex, naMakePos(10., (double)yPos64));
  if(showNBits) {
    con->bitnHex = bit_CreateBitOutputBox(naMakeSize(100., 68.), NA_TRUE);
    naAddSpaceChild(hexSpace, con->bitnHex, naMakePos(10., (double)yPosn));
  }

  naAddSpaceChild(space, hexSpace, naMakePos((double)offsetX, 0.));
  offsetX += 120;

  // ////////////////

  if(showBin) {
    NASpace* binSpace = naNewSpace(naMakeSize(305., (double)ySpaceHeight));
    naSetSpaceAlternateBackground(binSpace, alternateBlock % 2);
  alternateBlock++;

    con->inputBin = bit_CreateBitInputField(con, 285., bit_ValueChangeBin);
    naAddSpaceChild(binSpace, con->inputBin, naMakePos(10., (double)yPosInput));
    con->labelBin = bit_CreateSystemLabelField("Bin", 30.);
    naAddSpaceChild(binSpace, con->labelBin, naMakePos(10., (double)yPos8));
    con->bit8Bin = bit_CreateBitOutputField(75.);
    naAddSpaceChild(binSpace, con->bit8Bin, naMakePos(220., (double)yPos8));
    if(show16Bits) {
      con->bit16Bin = bit_CreateBitOutputField(145.);
      naAddSpaceChild(binSpace, con->bit16Bin, naMakePos(150., (double)yPos16));
    }
    con->bit32Bin = bit_CreateBitOutputField(285.);
    naAddSpaceChild(binSpace, con->bit32Bin, naMakePos(10., (double)yPos32));
    con->bit64Bin = bit_CreateBitOutputBox(naMakeSize(285., 34.), NA_FALSE);
    naAddSpaceChild(binSpace, con->bit64Bin, naMakePos(10., (double)yPos64));
    if(showNBits) {
      con->bitnBin = bit_CreateBitOutputBox(naMakeSize(285., 68.), NA_TRUE);
      naAddSpaceChild(binSpace, con->bitnBin, naMakePos(10., (double)yPosn));
    }

    naAddSpaceChild(space, binSpace, naMakePos((double)offsetX, 0.));
    offsetX += 305;
  }

  // ////////////////

  if(showAsc) {
    NASpace* ascSpace = naNewSpace(naMakeSize(75., (double)ySpaceHeight));
    naSetSpaceAlternateBackground(ascSpace, alternateBlock % 2);
  alternateBlock++;

    con->inputAsc = bit_CreateBitInputField(con, 55., bit_ValueChangeAsc);
    naAddSpaceChild(ascSpace, con->inputAsc, naMakePos(10., (double)yPosInput));
    con->labelAsc = bit_CreateSystemLabelField("Asc", 30.);
    naAddSpaceChild(ascSpace, con->labelAsc, naMakePos(10., (double)yPos8));
    con->bit8Asc = bit_CreateBitOutputField(30.);
    naAddSpaceChild(ascSpace, con->bit8Asc, naMakePos(35., (double)yPos8));
    if(show16Bits) {
      con->bit16Asc = bit_CreateBitOutputField(30.);
      naAddSpaceChild(ascSpace, con->bit16Asc, naMakePos(35., (double)yPos16));
    }
    con->bit32Asc = bit_CreateBitOutputField(55.);
    naAddSpaceChild(ascSpace, con->bit32Asc, naMakePos(10., (double)yPos32));
    con->bit64Asc = bit_CreateBitOutputBox(naMakeSize(55., 34.), NA_FALSE);
    naAddSpaceChild(ascSpace, con->bit64Asc, naMakePos(10., (double)yPos64));
    if(showNBits) {
      con->bitnAsc = bit_CreateBitOutputBox(naMakeSize(55., 68.), NA_TRUE);
      naAddSpaceChild(ascSpace, con->bitnAsc, naMakePos(10., (double)yPosn));
    }

    naAddSpaceChild(space, ascSpace, naMakePos((double)offsetX, 0.));
    offsetX += 75;
  }

  // ////////////////

  windowRect = naGetUIElementRect(con->window);
  windowRect.size.width = offsetX;

  naSetUIElementRect(con->window, windowRect);

  NATextField* nextTab;
  naSetWindowFirstTabElement(con->window, con->inputDec);
  naSetUIElementNextTabElement(con->inputDec, con->inputHex);
  nextTab = con->inputBin;
  if(!nextTab) {nextTab = con->inputAsc;}
  if(!nextTab) {nextTab = con->inputDec;}
  naSetUIElementNextTabElement(con->inputHex, nextTab);
  if(showBin) {
    nextTab = con->inputAsc;
    if(!nextTab) {nextTab = con->inputDec;}
    naSetUIElementNextTabElement(con->inputBin, nextTab);
  }
  if(showAsc) {
    naSetUIElementNextTabElement(con->inputAsc, con->inputDec);
  }
  
  // ////////////////

  bitUpdateConverterController(con);
  return con;
}



void bitDeallocConverterController(BitConverterController* con) {
  naDelete(con->window);
  naRelease(con->bitArray);
  naFree(con);
}



void bitShowConverterController(BitConverterController* con) {
  naShowWindow(con->window);
}



void bitKeepConverterOnTop(BitConverterController* con, NABool keepOnTop) {
  naKeepWindowOnTop(con->window, keepOnTop);
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
