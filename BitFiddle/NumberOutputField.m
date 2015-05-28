
#import "NumberOutputField.h"
#include "BitArray.h"


@implementation NumberOutputField


- (void)setSystem:(NumberSystem)newsystem withBitCount:(NAInt)newcount{
  numbersystem = newsystem;
  bitcount = newcount;
}

- (void)fillWithString:(NAString*) string withDecSign:(NABool)withdecsign{
  NSMutableString* mutablestring = [[self textStorage] mutableString];
  if(!string){
    [mutablestring setString:@" "];
  }else if(naIsStringEmpty(string)){
    [mutablestring setString:@"0"];
  }else{
    if(withdecsign){
      [mutablestring setString:[NSString stringWithFormat:@"-%s", naGetStringUTF8Pointer(string)]];
    }else{
      [mutablestring setString:[NSString stringWithUTF8String:naGetStringUTF8Pointer(string)]];
    }
  }
}


- (void)fillWithBitArray:(BitArray*) bitarray withDecSign:(NABool)withdecsign{
  NAString outstring;
  BitArray decsignarray;
  
//  
//  if(bitcount > naGetBitArrayCount(bitarray)){
//    naCreateBitArrayShiftExtension(&clamparray, bitarray, 0, bitcount);
//  }else{
//    naCreateBitArrayExtraction(&clamparray, bitarray, 0, bitcount);
//  }
  switch(numbersystem){
  case NUMBER_SYSTEM_DEC:
    naCreateBitArrayExtraction(&decsignarray, bitarray, 0, -1);
    if(bitarray && naGetBitArrayCount(bitarray) && withdecsign && *naGetBitArrayBit(bitarray, -1)){
      naDecoupleBitArray(&decsignarray);
      naComputeBitArrayTwosComplement(&decsignarray);
    }else{
      withdecsign = NA_FALSE;
    }
    naCreateStringDecFromBitArray(&outstring, &decsignarray);
    naClearBitArray(&decsignarray);
    break;
  case NUMBER_SYSTEM_HEX:
    naCreateStringHexFromBitArray(&outstring, bitarray);
    break;
  case NUMBER_SYSTEM_BIN:
    naCreateStringBinFromBitArray(&outstring, bitarray);
    break;
  }
  [self fillWithString:&outstring withDecSign:withdecsign];
  naClearString(&outstring);
//  naClearBitArray(&clamparray);
}


@end
