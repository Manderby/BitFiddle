

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

  bitarray = naCreateBufferPlain();
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  
  [outdec8  setSystem:NUMBER_SYSTEM_DEC];
  [outdec16 setSystem:NUMBER_SYSTEM_DEC];
  [outdec32 setSystem:NUMBER_SYSTEM_DEC];
  [outdec64 setSystem:NUMBER_SYSTEM_DEC];
  [outdecn  setSystem:NUMBER_SYSTEM_DEC];

  [outhex8  setSystem:NUMBER_SYSTEM_HEX];
  [outhex16 setSystem:NUMBER_SYSTEM_HEX];
  [outhex32 setSystem:NUMBER_SYSTEM_HEX];
  [outhex64 setSystem:NUMBER_SYSTEM_HEX];
  [outhexn  setSystem:NUMBER_SYSTEM_HEX];

  [outbin8  setSystem:NUMBER_SYSTEM_BIN];
  [outbin16 setSystem:NUMBER_SYSTEM_BIN];
  [outbin32 setSystem:NUMBER_SYSTEM_BIN];
  [outbin64 setSystem:NUMBER_SYSTEM_BIN];
  [outbinn  setSystem:NUMBER_SYSTEM_BIN];

  [outasc8  setSystem:NUMBER_SYSTEM_ASC];
  [outasc16 setSystem:NUMBER_SYSTEM_ASC];
  [outasc32 setSystem:NUMBER_SYSTEM_ASC];
  [outasc64 setSystem:NUMBER_SYSTEM_ASC];
  [outascn  setSystem:NUMBER_SYSTEM_ASC];
}


- (void)dealloc{
  naReleaseBuffer(bitarray);
  [super dealloc];
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

  NABuffer* bitarray8;
  NABuffer* bitarray16;
  NABuffer* bitarray32;
  NABuffer* bitarray64;
  NABuffer* bitarrayn;

  bitarray8 = naCreateBitArrayCopyWithFixedSize(bitarray, 8);
  bitarray16 = naCreateBitArrayCopyWithFixedSize(bitarray, 16);
  bitarray32 = naCreateBitArrayCopyWithFixedSize(bitarray, 32);
  bitarray64 = naCreateBitArrayCopyWithFixedSize(bitarray, 64);
  bitarrayn = naCreateBitArrayCopyWithFixedSize(bitarray, -8);
  
  NABool byteswap = [(BitFiddleApplication*)NSApp byteswap];
  [byteswapcheckbox setState:byteswap?NSOnState:NSOffState];
  NSString* stringbyteswap = nil;
  if(byteswap){
    stringbyteswap = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionByteSwap" value:nil table:nil];
    naComputeBitArraySwapBytes(bitarray8);
    naComputeBitArraySwapBytes(bitarray16);
    naComputeBitArraySwapBytes(bitarray32);
    naComputeBitArraySwapBytes(bitarray64);
    naComputeBitArraySwapBytes(bitarrayn);
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
    naComputeBitArrayOnesComplement(bitarray8);
    naComputeBitArrayOnesComplement(bitarray16);
    naComputeBitArrayOnesComplement(bitarray32);
    naComputeBitArrayOnesComplement(bitarray64);
    naComputeBitArrayOnesComplement(bitarrayn);
    break;
  case COMPUTE_TWOS_COMPLEMENT:
    stringconversion = [[NSBundle mainBundle] localizedStringForKey:@"BitFiddleConversionTwosComp" value:nil table:nil];
    naComputeBitArrayTwosComplement(bitarray8);
    naComputeBitArrayTwosComplement(bitarray16);
    naComputeBitArrayTwosComplement(bitarray32);
    naComputeBitArrayTwosComplement(bitarray64);
    naComputeBitArrayTwosComplement(bitarrayn);
    break;
  }
  
  
  if(isMini){
    if(stringbyteswap){
      [[self window] setTitle:[NSString stringWithFormat:@"BitFiddle     %@     %@", stringconversion, stringbyteswap]];
    }else{
      [[self window] setTitle:[NSString stringWithFormat:@"BitFiddle     %@", stringconversion]];
    }
  }
  

  if(conversiontype == COMPUTE_ONES_COMPLEMENT){
    [outdec8  fillWithString:NULL withDecSign:NA_FALSE];
    [outdec16 fillWithString:NULL withDecSign:NA_FALSE];
    [outdec32 fillWithString:NULL withDecSign:NA_FALSE];
    [outdec64 fillWithString:NULL withDecSign:NA_FALSE];
    [outdecn  fillWithString:NULL withDecSign:NA_FALSE];
  }else if(conversiontype == COMPUTE_TWOS_COMPLEMENT){
    [outdec8  fillWithBitArray:bitarray8  withDecSign:NA_TRUE];
    [outdec16 fillWithBitArray:bitarray16 withDecSign:NA_TRUE];
    [outdec32 fillWithBitArray:bitarray32 withDecSign:NA_TRUE];
    [outdec64 fillWithBitArray:bitarray64 withDecSign:NA_TRUE];
    [outdecn  fillWithBitArray:bitarrayn  withDecSign:NA_TRUE];
  }else{
    [outdec8  fillWithBitArray:bitarray8  withDecSign:NA_FALSE];
    [outdec16 fillWithBitArray:bitarray16 withDecSign:NA_FALSE];
    [outdec32 fillWithBitArray:bitarray32 withDecSign:NA_FALSE];
    [outdec64 fillWithBitArray:bitarray64 withDecSign:NA_FALSE];
    [outdecn  fillWithBitArray:bitarrayn  withDecSign:NA_FALSE];
  }

  [outhex8  fillWithBitArray:bitarray8  withDecSign:NA_FALSE];
  [outhex16 fillWithBitArray:bitarray16 withDecSign:NA_FALSE];
  [outhex32 fillWithBitArray:bitarray32 withDecSign:NA_FALSE];
  [outhex64 fillWithBitArray:bitarray64 withDecSign:NA_FALSE];
  [outhexn  fillWithBitArray:bitarrayn  withDecSign:NA_FALSE];
  
  [outbin8  fillWithBitArray:bitarray8  withDecSign:NA_FALSE];
  [outbin16 fillWithBitArray:bitarray16 withDecSign:NA_FALSE];
  [outbin32 fillWithBitArray:bitarray32 withDecSign:NA_FALSE];
  [outbin64 fillWithBitArray:bitarray64 withDecSign:NA_FALSE];
  [outbinn  fillWithBitArray:bitarrayn  withDecSign:NA_FALSE];
    
  [outasc8  fillWithBitArray:bitarray8  withDecSign:NA_FALSE];
  [outasc16 fillWithBitArray:bitarray16 withDecSign:NA_FALSE];
  [outasc32 fillWithBitArray:bitarray32 withDecSign:NA_FALSE];
  [outasc64 fillWithBitArray:bitarray64 withDecSign:NA_FALSE];
  [outascn  fillWithBitArray:bitarrayn  withDecSign:NA_FALSE];
  
  naClearBitArray(bitarray8);
  naClearBitArray(bitarray16);
  naClearBitArray(bitarray32);
  naClearBitArray(bitarray64);
  naClearBitArray(bitarrayn);
}




- (void)controlTextDidChange:(NSNotification *)note{
  NSTextField * changedField = [note object];
  if      (changedField == indec){[self valueChangeDec:changedField];}
  else if (changedField == inhex){[self valueChangeHex:changedField];}
  else if (changedField == inbin){[self valueChangeBin:changedField];}
  else if (changedField == inasc){[self valueChangeAsc:changedField];}
}



- (void)valueChangeDec:(NSControl*)sender{
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  
  naReleaseBuffer(bitarray);
  NAString instring = naMakeStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  bitarray = naCreateBitArrayWithDecString(&instring);
  naClearString(&instring);

  [self update];
}



- (void)valueChangeHex:(NSControl*)sender{
  [indec setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];

  naReleaseBuffer(bitarray);
  NAString instring = naMakeStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  bitarray = naCreateBitArrayWithHexString(&instring);
  naClearString(&instring);

  [self update];
}



- (void)valueChangeBin:(NSControl*)sender{
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inasc setStringValue:@""];

  naReleaseBuffer(bitarray);
  NAString instring = naMakeStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  bitarray = naCreateBitArrayWithBinString(&instring);
  naClearString(&instring);

  [self update];
}



- (void)valueChangeAsc:(NSControl*)sender{
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];

  naReleaseBuffer(bitarray);
  NAString instring = naMakeStringWithUTF8CStringLiteral([[sender stringValue] UTF8String]);
  bitarray = naCreateBitArrayWithAscString(&instring);
  naClearString(&instring);

  [self update];
}


- (void)resetValue{
  [indec setStringValue:@""];
  [inhex setStringValue:@""];
  [inbin setStringValue:@""];
  [inasc setStringValue:@""];
  
  naReleaseBuffer(bitarray);
  bitarray = naCreateBufferPlain();
  
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