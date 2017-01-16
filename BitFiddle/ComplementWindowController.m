

#import "ComplementWindowController.h"
#import "BitFiddleApplication.h"
#include "NAString.h"
#include "BitArray.h"


@implementation ComplementWindowController


- (void)awakeFromNib{

  [[self window] setDelegate:self];

  NSString* stringu = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionUnsigned" value:nil table:nil];
  NSString* string1 = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionOnesComp" value:nil table:nil];
  NSString* string2 = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionTwosComp" value:nil table:nil];
  [segcontrol setLabel:stringu forSegment:0];
  [segcontrol setLabel:string1 forSegment:1];
  [segcontrol setLabel:string2 forSegment:2];
  [segcontrol sizeToFit];

  naInitBitArray(&bitarray);
  naInitBitArray(&bitarray8);
  naInitBitArray(&bitarray16);
  naInitBitArray(&bitarray32);
  naInitBitArray(&bitarray64);
  naInitBitArray(&bitarrayn);
  naInitByteArray(&bytearraychr8);
  naInitByteArray(&bytearraychr16);
  naInitByteArray(&bytearraychr32);
  naInitByteArray(&bytearraychr64);
  naInitByteArray(&bytearraychrn);
  chrstr8 = naNewString();
  chrstr16 = naNewString();
  chrstr32 = naNewString();
  chrstr64 = naNewString();
  chrstrn = naNewString();
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  [outdec8  setSystem:NUMBER_SYSTEM_DEC withBitCount:8];
  [outdec16 setSystem:NUMBER_SYSTEM_DEC withBitCount:16];
  [outdec32 setSystem:NUMBER_SYSTEM_DEC withBitCount:32];
  [outdec64 setSystem:NUMBER_SYSTEM_DEC withBitCount:64];
  [outdecn  setSystem:NUMBER_SYSTEM_DEC withBitCount:0];
  [outhex8  setSystem:NUMBER_SYSTEM_HEX withBitCount:8];
  [outhex16 setSystem:NUMBER_SYSTEM_HEX withBitCount:16];
  [outhex32 setSystem:NUMBER_SYSTEM_HEX withBitCount:32];
  [outhex64 setSystem:NUMBER_SYSTEM_HEX withBitCount:64];
  [outhexn  setSystem:NUMBER_SYSTEM_HEX withBitCount:0];
  [outbin8  setSystem:NUMBER_SYSTEM_BIN withBitCount:8];
  [outbin16 setSystem:NUMBER_SYSTEM_BIN withBitCount:16];
  [outbin32 setSystem:NUMBER_SYSTEM_BIN withBitCount:32];
  [outbin64 setSystem:NUMBER_SYSTEM_BIN withBitCount:64];
  [outbinn  setSystem:NUMBER_SYSTEM_BIN withBitCount:0];

  [outchr8  setSystem:NUMBER_SYSTEM_BIN withBitCount:8];
  [outchr16  setSystem:NUMBER_SYSTEM_BIN withBitCount:16];
  [outchr32  setSystem:NUMBER_SYSTEM_BIN withBitCount:32];
  [outchr64  setSystem:NUMBER_SYSTEM_BIN withBitCount:64];
  [outchrn  setSystem:NUMBER_SYSTEM_BIN withBitCount:0];
}


- (void)setMini:(NABool)ismini{
  isMini = ismini;
}


- (void)showDialog{
  [[self window] makeKeyAndOrderFront:self];
  [self update];
}

- (void)hideDialog{
  [[self window] orderOut:nil];
}


- (void)update{

  NAInt ncount = naGetBitArrayCount(&bitarray);
  
  naClearBitArray(&bitarray8);
  naClearBitArray(&bitarray16);
  naClearBitArray(&bitarray32);
  naClearBitArray(&bitarray64);
  naClearBitArray(&bitarrayn);
  naClearByteArray(&bytearraychr8);
  naClearByteArray(&bytearraychr16);
  naClearByteArray(&bytearraychr32);
  naClearByteArray(&bytearraychr64);
  naClearByteArray(&bytearraychrn);
  naDelete(chrstr8);
  naDelete(chrstr16);
  naDelete(chrstr32);
  naDelete(chrstr64);
  naDelete(chrstrn);
  naInitBitArrayExtraction(&bitarrayn, &bitarray, 0, -1);
  if(ncount < 8){
    naInitBitArrayShiftExtension(&bitarray8, &bitarray, 0, 8);
  }else{
    naInitBitArrayExtraction(&bitarray8, &bitarray, 0, 8);
  }
  if(ncount < 16){
    naInitBitArrayShiftExtension(&bitarray16, &bitarray, 0, 16);
  }else{
    naInitBitArrayExtraction(&bitarray16, &bitarray, 0, 16);
  }
  if(ncount < 32){
    naInitBitArrayShiftExtension(&bitarray32, &bitarray, 0, 32);
  }else{
    naInitBitArrayExtraction(&bitarray32, &bitarray, 0, 32);
  }
  if(ncount < 64){
    naInitBitArrayShiftExtension(&bitarray64, &bitarray, 0, 64);
  }else{
    naInitBitArrayExtraction(&bitarray64, &bitarray, 0, 64);
  }
  
  NABool byteswap = [(BitFiddleApplication*)NSApp byteswap];
  [byteswapcheckbox setState:byteswap?NSOnState:NSOffState];
  NSString* stringbyteswap = nil;
  if(byteswap){
    stringbyteswap = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionByteSwap" value:nil table:nil];
    naDecoupleBitArray(&bitarray8);
    naDecoupleBitArray(&bitarray16);
    naDecoupleBitArray(&bitarray32);
    naDecoupleBitArray(&bitarray64);
    naDecoupleBitArray(&bitarrayn);
    naComputeBitArraySwapBytes(&bitarray8);
    naComputeBitArraySwapBytes(&bitarray16);
    naComputeBitArraySwapBytes(&bitarray32);
    naComputeBitArraySwapBytes(&bitarray64);
    naComputeBitArraySwapBytes(&bitarrayn);
  }

  ConversionType conversiontype = [(BitFiddleApplication*)NSApp conversiontype];
  [segcontrol setSelectedSegment:conversiontype];
  
  NSString* stringconversion = nil;
  switch(conversiontype){
  case COMPUTE_UNSIGNED:
    stringconversion = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionUnsigned" value:nil table:nil];
    break;
  case COMPUTE_ONES_COMPLEMENT:
    stringconversion = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionOnesComp" value:nil table:nil];
    naDecoupleBitArray(&bitarray8);
    naDecoupleBitArray(&bitarray16);
    naDecoupleBitArray(&bitarray32);
    naDecoupleBitArray(&bitarray64);
    naDecoupleBitArray(&bitarrayn);
    naComputeBitArrayOnesComplement(&bitarray8);
    naComputeBitArrayOnesComplement(&bitarray16);
    naComputeBitArrayOnesComplement(&bitarray32);
    naComputeBitArrayOnesComplement(&bitarray64);
    naComputeBitArrayOnesComplement(&bitarrayn);
    break;
  case COMPUTE_TWOS_COMPLEMENT:
    stringconversion = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionTwosComp" value:nil table:nil];
    naDecoupleBitArray(&bitarray8);
    naDecoupleBitArray(&bitarray16);
    naDecoupleBitArray(&bitarray32);
    naDecoupleBitArray(&bitarray64);
    naDecoupleBitArray(&bitarrayn);
    naComputeBitArrayTwosComplement(&bitarray8);
    naComputeBitArrayTwosComplement(&bitarray16);
    naComputeBitArrayTwosComplement(&bitarray32);
    naComputeBitArrayTwosComplement(&bitarray64);
    naComputeBitArrayTwosComplement(&bitarrayn);
    break;
  }
  
  if(isMini){
    if(stringbyteswap){
      [[self window] setTitle:[NSString stringWithFormat:@"BitFiddle     %@     %@", stringconversion, stringbyteswap]];
    }else{
      [[self window] setTitle:[NSString stringWithFormat:@"BitFiddle     %@", stringconversion]];
    }
  }
  

  [outhex8  fillWithBitArray:&bitarray8 withDecSign:NA_FALSE];
  [outhex16 fillWithBitArray:&bitarray16 withDecSign:NA_FALSE];
  [outhex32 fillWithBitArray:&bitarray32 withDecSign:NA_FALSE];
  [outhex64 fillWithBitArray:&bitarray64 withDecSign:NA_FALSE];
  [outhexn  fillWithBitArray:&bitarrayn withDecSign:NA_FALSE];
  [outbin8  fillWithBitArray:&bitarray8 withDecSign:NA_FALSE];
  [outbin16 fillWithBitArray:&bitarray16 withDecSign:NA_FALSE];
  [outbin32 fillWithBitArray:&bitarray32 withDecSign:NA_FALSE];
  [outbin64 fillWithBitArray:&bitarray64 withDecSign:NA_FALSE];
  [outbinn  fillWithBitArray:&bitarrayn withDecSign:NA_FALSE];
  if(conversiontype == COMPUTE_ONES_COMPLEMENT){
    [outdec8  fillWithString:NULL withDecSign:NA_FALSE];
    [outdec16 fillWithString:NULL withDecSign:NA_FALSE];
    [outdec32 fillWithString:NULL withDecSign:NA_FALSE];
    [outdec64 fillWithString:NULL withDecSign:NA_FALSE];
    [outdecn  fillWithString:NULL withDecSign:NA_FALSE];
  }else if(conversiontype == COMPUTE_TWOS_COMPLEMENT){
    [outdec8  fillWithBitArray:&bitarray8 withDecSign:NA_TRUE];
    [outdec16 fillWithBitArray:&bitarray16 withDecSign:NA_TRUE];
    [outdec32 fillWithBitArray:&bitarray32 withDecSign:NA_TRUE];
    [outdec64 fillWithBitArray:&bitarray64 withDecSign:NA_TRUE];
    [outdecn  fillWithBitArray:&bitarrayn withDecSign:NA_TRUE];
  }else{
    [outdec8  fillWithBitArray:&bitarray8 withDecSign:NA_FALSE];
    [outdec16 fillWithBitArray:&bitarray16 withDecSign:NA_FALSE];
    [outdec32 fillWithBitArray:&bitarray32 withDecSign:NA_FALSE];
    [outdec64 fillWithBitArray:&bitarray64 withDecSign:NA_FALSE];
    [outdecn  fillWithBitArray:&bitarrayn withDecSign:NA_FALSE];
  }

  naInitByteArrayFromBitArray(&bytearraychr8, &bitarray8);
  if(naIsByteArrayEmpty(&bytearraychr8)){
    chrstr8 = naNewStringWithUTF8CStringLiteral("");
  }else{
    NAInt arraysize = naGetByteArrayBytesize(&bytearraychr8);
    NAByte* curbyte = naGetByteArrayMutablePointer(&bytearraychr8);
    while(arraysize){
      if((*curbyte < 32) || (*curbyte > 126)){*curbyte = '?';}
      curbyte++;
      arraysize--;
    }
    chrstr8 = naNewStringWithUTF8CStringLiteral((NAUTF8Char*)naGetByteArrayConstPointer(&bytearraychr8));
  }
  [outchr8 fillWithString:chrstr8 withDecSign:NA_FALSE];


  naInitByteArrayFromBitArray(&bytearraychr16, &bitarray16);
  if(naIsByteArrayEmpty(&bytearraychr16)){
    chrstr16 = naNewStringWithUTF8CStringLiteral("");
  }else{
    NAInt arraysize = naGetByteArrayBytesize(&bytearraychr16);
    NAByte* curbyte = naGetByteArrayMutablePointer(&bytearraychr16);
    while(arraysize){
      if((*curbyte < 32) || (*curbyte > 126)){*curbyte = '?';}
      curbyte++;
      arraysize--;
    }
    chrstr16 = naNewStringWithUTF8CStringLiteral((NAUTF8Char*)naGetByteArrayConstPointer(&bytearraychr16));
  }
  [outchr16 fillWithString:chrstr16 withDecSign:NA_FALSE];


  naInitByteArrayFromBitArray(&bytearraychr32, &bitarray32);
  if(naIsByteArrayEmpty(&bytearraychr32)){
    chrstr32 = naNewStringWithUTF8CStringLiteral("");
  }else{
    NAInt arraysize = naGetByteArrayBytesize(&bytearraychr32);
    NAByte* curbyte = naGetByteArrayMutablePointer(&bytearraychr32);
    while(arraysize){
      if((*curbyte < 32) || (*curbyte > 126)){*curbyte = '?';}
      curbyte++;
      arraysize--;
    }
    chrstr32 = naNewStringWithUTF8CStringLiteral((NAUTF8Char*)naGetByteArrayConstPointer(&bytearraychr32));
  }
  [outchr32 fillWithString:chrstr32 withDecSign:NA_FALSE];


  naInitByteArrayFromBitArray(&bytearraychr64, &bitarray64);
  if(naIsByteArrayEmpty(&bytearraychr64)){
    chrstr64 = naNewStringWithUTF8CStringLiteral("");
  }else{
    NAInt arraysize = naGetByteArrayBytesize(&bytearraychr64);
    NAInt separators = (arraysize-1) / 4;
    NAUTF8Char* stringbuf = naMalloc(-(arraysize + separators));
    chrstr64 = naNewStringWithMutableUTF8Buffer(stringbuf, -(arraysize + separators), NA_MEMORY_CLEANUP_FREE);
    NAUTF8Char* curchar = stringbuf;
    const NAByte* curbyte = naGetByteArrayConstPointer(&bytearraychr64);
    for(NAInt i=0; i<arraysize; i++){
      if(i && (i % 4) == 0){*curchar++ = '\n';}
      if((*curbyte < 32) || (*curbyte > 126)){*curchar = '?';}else{*curchar = *curbyte;}
      curbyte++;
      curchar++;
    }
  }
  [outchr64 fillWithString:chrstr64 withDecSign:NA_FALSE];


  naInitByteArrayFromBitArray(&bytearraychrn, &bitarrayn);
  if(naIsByteArrayEmpty(&bytearraychrn)){
    chrstrn = naNewStringWithUTF8CStringLiteral("");
  }else{
    NAInt arraysize = naGetByteArrayBytesize(&bytearraychrn);
    NAInt separators = (arraysize-1) / 4;
    NAUTF8Char* stringbuf = naMalloc(-(arraysize + separators));
    chrstrn = naNewStringWithMutableUTF8Buffer(stringbuf, -(arraysize + separators), NA_MEMORY_CLEANUP_FREE);
    NAUTF8Char* curchar = stringbuf;
    const NAByte* curbyte = naGetByteArrayConstPointer(&bytearraychrn);
    for(NAInt i=0; i<arraysize; i++){
      if(i && (i % 4) == 0){*curchar++ = '\n';}
      if((*curbyte < 32) || (*curbyte > 126)){*curchar = '?';}else{*curchar = *curbyte;}
      curbyte++;
      curchar++;
    }
  }
  [outchrn fillWithString:chrstrn withDecSign:NA_FALSE];


}




- (void)controlTextDidChange:(NSNotification *)note{
  NSTextField * changedField = [note object];
  if      (changedField == indec){[self valueChangeDec:changedField];}
  else if (changedField == inhex){[self valueChangeHex:changedField];}
  else if (changedField == inbin){[self valueChangeBin:changedField];}
  else if (changedField == inasc){[self valueChangeAsc:changedField];}
}


- (void)valueChangeDec:(NSControl*)sender{
  NAString* instring;
  instring = naNewStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  naInitBitArrayWithDecString(&bitarray, instring, -8);
  naDelete(instring);
  [self update];
}

- (void)valueChangeHex:(NSControl*)sender{
  NAString* instring;
  instring = naNewStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  [indec setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  naInitBitArrayWithHexString(&bitarray, instring, -8);
  naDelete(instring);
  [self update];
}

- (void)valueChangeBin:(NSControl*)sender{
  NAString* instring;
  instring = naNewStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inasc setStringValue:@""];
  naInitBitArrayWithBinString(&bitarray, instring, -8);
  naDelete(instring);
  [self update];
}

- (void)valueChangeAsc:(NSControl*)sender{
  NAString* instring;
  NAByteArray inarray;
  instring = naNewStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  NAUInt stringsize = naGetStringLength(instring);
  if(stringsize){
    naInitByteArrayWithConstBuffer(&inarray, naGetStringChar(instring, 0), stringsize);
  }else{
    naInitByteArray(&inarray);
  }
  naDecoupleByteArray(&inarray, NA_FALSE);
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  naInitBitArrayWithByteArray(&bitarray, &inarray, -8);
  naDelete(instring);
  naClearByteArray(&inarray);
  [self update];
}


- (void)resetValue{
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  NAString* instring = naNewStringWithFormat("");
  naInitBitArrayWithBinString(&bitarray, instring, -8);
  naDelete(instring);
  [self update];
}


- (IBAction)segmentControlChange:(NSControl*)sender{
  ConversionType conversiontype = (ConversionType)[segcontrol selectedSegment];
  switch(conversiontype){
  case COMPUTE_UNSIGNED: [(BitFiddleApplication*)NSApp switchToUnsigned:sender]; break;
  case COMPUTE_ONES_COMPLEMENT: [(BitFiddleApplication*)NSApp switchToOnesComplement:sender]; break;
  case COMPUTE_TWOS_COMPLEMENT: [(BitFiddleApplication*)NSApp switchToTwosComplement:sender]; break;
  }
}



@end
