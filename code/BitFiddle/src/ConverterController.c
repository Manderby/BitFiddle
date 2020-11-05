
#include "ConverterController.h"
#include "ASCIIController.h"
#include "BitArray.h"
#include "BitFiddleTranslations.h"
#include "BitFiddleApplication.h"
#include "ManderAppAbout.h"
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
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
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
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
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
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
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
  NAString* instring = naNewStringWithTextFieldText(reaction.uiElement);
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
  BitConverterController* con = reaction.controller;
  naRelease(con->bitarray);
  naStopApplication();
  return NA_TRUE;
  // todo: when set to na_true, sometimes, it crashes.
}



NABool switchComplement(NAReaction reaction){
  BitConverterController* con = reaction.controller;

  if(reaction.uiElement == con->unsignedOption){
    bitSetConversionType(COMPUTE_UNSIGNED);
  }else if(reaction.uiElement == con->onesOption){
    bitSetConversionType(COMPUTE_ONES_COMPLEMENT);
  }else if(reaction.uiElement == con->twosOption){
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

  if(reaction.uiElement == con->helpButton){
    mandShowAboutController();
  }else if(reaction.uiElement == con->preferencesButton){
    bitShowPreferencesController();
  }else if(reaction.uiElement == con->asciiButton){
    bitShowASCIIController();
  }else{
    #ifndef NDEBUG
      naError("Unknown window button");
    #endif
  }
  return NA_TRUE;
}



void fillOutputFieldWithString(NALabel* outputField, NAString* string, NABool withDecSign){
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



void fillOutputTextBoxWithString(NATextBox* outputTextBox, NAString* string, NABool withDecSign){
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
} NumberSystem;



void fillOutputFieldWithBitArray(void* outputField, NumberSystem numbersystem, NABuffer* bitarray, NABool withDecSign){
  NAString* outstring;
  if(!outputField){return;}
  
  switch(numbersystem){
  case NUMBER_SYSTEM_DEC:
    if(bitarray && naGetBufferRange(bitarray).length && withDecSign && naGetBufferByteAtIndex(bitarray, naGetRangeiMax(naGetBufferRange(bitarray)))){
      NABuffer* twocomp = naNewBufferCopy(bitarray, naGetBufferRange(bitarray), NA_FALSE);
      naComputeBitArrayTwosComplement(twocomp);
      outstring = naNewStringDecWithBitArray(twocomp);
      naRelease(twocomp);
    }else{
      withDecSign = NA_FALSE;
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
  if(naGetUIElementType(outputField) == NA_UI_LABEL){
    fillOutputFieldWithString(outputField, outstring, withDecSign);
  }else{
    fillOutputTextBoxWithString(outputField, outstring, withDecSign);
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



NATextField* createBitInputField(BitConverterController* con, NASize size, NAReactionHandler handler){
  NATextField* textfield = naNewTextField(size);
  naSetTextFieldFontKind(textfield, NA_FONT_KIND_MONOSPACE);
  naSetTextFieldTextAlignment(textfield, NA_TEXT_ALIGNMENT_RIGHT);
  naAddUIReaction(textfield, NA_UI_COMMAND_EDITED, handler, con);
  return textfield;
}

NALabel* createBitLabelField(const NAUTF8Char* title, NASize size){
  NALabel* labelfield = naNewLabel(title, size);
  naSetLabelFontKind(labelfield, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelEnabled(labelfield, NA_FALSE);
  return labelfield;
}

NALabel* createSystemLabelField(const NAUTF8Char* title, NASize size){
  NALabel* labelfield = naNewLabel(title, size);
  naSetLabelFontKind(labelfield, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(labelfield, NA_TEXT_ALIGNMENT_LEFT);
  naSetLabelEnabled(labelfield, NA_FALSE);
  return labelfield;
}

NALabel* createBitOutputField(NASize size){
  NALabel* outputField = naNewLabel("", size);
  naSetLabelFontKind(outputField, NA_FONT_KIND_MONOSPACE);
  naSetLabelTextAlignment(outputField, NA_TEXT_ALIGNMENT_RIGHT);
  return outputField;
}

NATextBox* createBitOutputBox(NASize size){
  NATextBox* outputbox = naNewTextBox(size);
  naSetTextBoxFontKind(outputbox, NA_FONT_KIND_MONOSPACE);
  naSetTextBoxTextAlignment(outputbox, NA_TEXT_ALIGNMENT_RIGHT);
  naSetTextBoxEditable(outputbox, NA_FALSE);
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

  NARect windowrect = naMakeRectS(20., 20., 777., (double)yspaceheight);
  con->window = naNewWindow("Complement", windowrect, NA_FALSE, BIT_WINDOW_TAG_CONVERTER);
  naAddUIReaction(con->window, NA_UI_COMMAND_CLOSES, closeConverterWindow, con);

  NASpace* space = naGetWindowContentSpace(con->window);
  double offsetx = 0;
  int alternateblock = 0;
  
  // /////////////////////

  NASpace* settingspace = naNewSpace(naMakeSize(110., (double)yspaceheight));
  naSetSpaceAlternateBackground(settingspace, alternateblock % 2);
  alternateblock++;

  con->unsignedOption = naNewTextOptionButton("U", naMakeSize(24., 24.));
  naAddUIReaction(con->unsignedOption, NA_UI_COMMAND_PRESSED, switchComplement, con);
  naAddSpaceChild(settingspace, con->unsignedOption, naMakePos(13., (double)yposinput));

  con->onesOption = naNewTextOptionButton("1", naMakeSize(24., 24.));
  naAddUIReaction(con->onesOption, NA_UI_COMMAND_PRESSED, switchComplement, con);
  naAddSpaceChild(settingspace, con->onesOption, naMakePos(43., (double)yposinput));

  con->twosOption = naNewTextOptionButton("2", naMakeSize(24., 24.));
  naAddUIReaction(con->twosOption, NA_UI_COMMAND_PRESSED, switchComplement, con);
  naAddSpaceChild(settingspace, con->twosOption, naMakePos(73., (double)yposinput));

  con->endiannessCheckBox = naNewCheckBox(bitTranslate(BitFiddleConversionByteSwap), naMakeSize(90, 22));
  naAddUIReaction(con->endiannessCheckBox, NA_UI_COMMAND_PRESSED, bitSwitchAppEndianness, bitGetApplication());
  #if NA_OS == NA_OS_WINDOWS
    naAddSpaceChild(settingspace, con->endiannessCheckBox, naMakePos(15., (double)ypos8));
  #else
    naAddSpaceChild(settingspace, con->endiannessCheckBox, naMakePos(10., (double)ypos8));
  #endif

  NAUIImage* helpuiimage = bitGetImageAsset(BIT_IMAGE_ASSET_HELP_BUTTON);
  con->helpButton = naNewImageButton(helpuiimage, naMakeSize(20., 20.));
  naAddUIReaction(con->helpButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(settingspace, con->helpButton, naMakePos(13., 10.));

  NAUIImage* prefuiimage = bitGetImageAsset(BIT_IMAGE_ASSET_PREFS_BUTTON);
  con->preferencesButton = naNewImageButton(prefuiimage, naMakeSize(20., 20.));
  naAddUIReaction(con->preferencesButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(settingspace, con->preferencesButton, naMakePos(43., 10.));

  NAUIImage* ascuiimage = bitGetImageAsset(BIT_IMAGE_ASSET_ASCII_BUTTON);
  con->asciiButton = naNewImageButton(ascuiimage, naMakeSize(20., 20.));
  naAddUIReaction(con->asciiButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(settingspace, con->asciiButton, naMakePos(73., 10.));

  naAddSpaceChild(space, settingspace, naMakePos(offsetx, 0.));
  offsetx += 110;

  // ////////////////

  NASpace* bitcountspace = naNewSpace(naMakeSize(25, (double)yspaceheight));
  naSetSpaceAlternateBackground(bitcountspace, alternateblock % 2);
  alternateblock++;

  NALabel* bit8label = createBitLabelField("8", naMakeSize(25., 17.));
  naAddSpaceChild(bitcountspace, bit8label, naMakePos(0., (double)ypos8));
  if(show16Bits){
    NALabel* bit16label = createBitLabelField("16", naMakeSize(25., 17.));
    naAddSpaceChild(bitcountspace, bit16label, naMakePos(0., (double)ypos16));
  }
  NALabel* bit32label = createBitLabelField("32", naMakeSize(25., 17.));
  naAddSpaceChild(bitcountspace, bit32label, naMakePos(0., (double)ypos32));
  NALabel* bit64label = createBitLabelField("64", naMakeSize(25., 34.));
  naAddSpaceChild(bitcountspace, bit64label, naMakePos(0., (double)ypos64));
  if(showNBits){
    NALabel* bitnlabel = createBitLabelField("n", naMakeSize(25., 68.));
    naAddSpaceChild(bitcountspace, bitnlabel, naMakePos(0., (double)yposn));
  }

  naAddSpaceChild(space, bitcountspace, naMakePos((double)offsetx, 0.));
  offsetx += 25;

  // ////////////////

  NASpace* decspace = naNewSpace(naMakeSize(140., (double)yspaceheight));
  naSetSpaceAlternateBackground(decspace, alternateblock % 2);
  alternateblock++;

  con->inputdec = createBitInputField(con, naMakeSize(120., 24.), valueChangeDec);
  naAddSpaceChild(decspace, con->inputdec, naMakePos(10., (double)yposinput));
  con->labeldec = createSystemLabelField("Dec", naMakeSize(30., 17.));
  naAddSpaceChild(decspace, con->labeldec, naMakePos(10., (double)ypos8));
  con->bit8dec = createBitOutputField(naMakeSize(38., 17.));
  naAddSpaceChild(decspace, con->bit8dec, naMakePos(92., (double)ypos8));
  if(show16Bits){
    con->bit16dec = createBitOutputField(naMakeSize(75., 17.));
    naAddSpaceChild(decspace, con->bit16dec, naMakePos(55., (double)ypos16));
  }
  con->bit32dec = createBitOutputField(naMakeSize(120., 17.));
  naAddSpaceChild(decspace, con->bit32dec, naMakePos(10., (double)ypos32));
  con->bit64dec = createBitOutputField(naMakeSize(120., 34.));
  naAddSpaceChild(decspace, con->bit64dec, naMakePos(10., (double)ypos64));
  if(showNBits){
    con->bitndec = createBitOutputBox(naMakeSize(120., 68.));
    naAddSpaceChild(decspace, con->bitndec, naMakePos(10., (double)yposn));
  }

  naAddSpaceChild(space, decspace, naMakePos((double)offsetx, 0.));
  offsetx += 140;

  // ////////////////

  NASpace* hexspace = naNewSpace(naMakeSize(120., (double)yspaceheight));
  naSetSpaceAlternateBackground(hexspace, alternateblock % 2);
  alternateblock++;

  con->inputhex = createBitInputField(con, naMakeSize(100., 24.), valueChangeHex);
  naAddSpaceChild(hexspace, con->inputhex, naMakePos(10., (double)yposinput));
  con->labelhex = createSystemLabelField("Hex", naMakeSize(30., 17.));
  naAddSpaceChild(hexspace, con->labelhex, naMakePos(10., (double)ypos8));
  con->bit8hex = createBitOutputField(naMakeSize(25., 17.));
  naAddSpaceChild(hexspace, con->bit8hex, naMakePos(85., (double)ypos8));
  if(show16Bits){
    con->bit16hex = createBitOutputField(naMakeSize(50., 17.));
    naAddSpaceChild(hexspace, con->bit16hex, naMakePos(60., (double)ypos16));
  }
  con->bit32hex = createBitOutputField(naMakeSize(100., 17.));
  naAddSpaceChild(hexspace, con->bit32hex, naMakePos(10., (double)ypos32));
  con->bit64hex = createBitOutputField(naMakeSize(100., 34.));
  naAddSpaceChild(hexspace, con->bit64hex, naMakePos(10., (double)ypos64));
  if(showNBits){
    con->bitnhex = createBitOutputBox(naMakeSize(100., 68.));
    naAddSpaceChild(hexspace, con->bitnhex, naMakePos(10., (double)yposn));
  }

  naAddSpaceChild(space, hexspace, naMakePos((double)offsetx, 0.));
  offsetx += 120;

  // ////////////////

  if(showBin){
    NASpace* binspace = naNewSpace(naMakeSize(305., (double)yspaceheight));
    naSetSpaceAlternateBackground(binspace, alternateblock % 2);
  alternateblock++;

    con->inputbin = createBitInputField(con, naMakeSize(285., 24.), valueChangeBin);
    naAddSpaceChild(binspace, con->inputbin, naMakePos(10., (double)yposinput));
    con->labelbin = createSystemLabelField("Bin", naMakeSize(30., 17.));
    naAddSpaceChild(binspace, con->labelbin, naMakePos(10., (double)ypos8));
    con->bit8bin = createBitOutputField(naMakeSize(75., 17.));
    naAddSpaceChild(binspace, con->bit8bin, naMakePos(220., (double)ypos8));
    if(show16Bits){
      con->bit16bin = createBitOutputField(naMakeSize(145., 17.));
      naAddSpaceChild(binspace, con->bit16bin, naMakePos(150., (double)ypos16));
    }
    con->bit32bin = createBitOutputField(naMakeSize(285., 17.));
    naAddSpaceChild(binspace, con->bit32bin, naMakePos(10., (double)ypos32));
    con->bit64bin = createBitOutputField(naMakeSize(285., 34.));
    naAddSpaceChild(binspace, con->bit64bin, naMakePos(10., (double)ypos64));
    if(showNBits){
      con->bitnbin = createBitOutputBox(naMakeSize(285., 68.));
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

    con->inputasc = createBitInputField(con, naMakeSize(55., 24.), valueChangeAsc);
    naAddSpaceChild(ascspace, con->inputasc, naMakePos(10., (double)yposinput));
    con->labelasc = createSystemLabelField("Asc", naMakeSize(30., 17.));
    naAddSpaceChild(ascspace, con->labelasc, naMakePos(10., (double)ypos8));
    con->bit8asc = createBitOutputField(naMakeSize(30., 17.));
    naAddSpaceChild(ascspace, con->bit8asc, naMakePos(35., (double)ypos8));
    if(show16Bits){
      con->bit16asc = createBitOutputField(naMakeSize(30., 17.));
      naAddSpaceChild(ascspace, con->bit16asc, naMakePos(35., (double)ypos16));
    }
    con->bit32asc = createBitOutputField(naMakeSize(55., 17.));
    naAddSpaceChild(ascspace, con->bit32asc, naMakePos(10., (double)ypos32));
    con->bit64asc = createBitOutputField(naMakeSize(55., 34.));
    naAddSpaceChild(ascspace, con->bit64asc, naMakePos(10., (double)ypos64));
    if(showNBits){
      con->bitnasc = createBitOutputBox(naMakeSize(55., 68.));
      naAddSpaceChild(ascspace, con->bitnasc, naMakePos(10., (double)yposn));
    }

    naAddSpaceChild(space, ascspace, naMakePos((double)offsetx, 0.));
    offsetx += 75;
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
  naDelete(con->window);
  naFree(con);
}



void naShowConverterController(BitConverterController* con){
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
