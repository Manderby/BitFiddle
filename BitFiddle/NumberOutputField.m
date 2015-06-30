
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
  NAString* outstring;
  BitArray decsignarray;
  
  switch(numbersystem){
  case NUMBER_SYSTEM_DEC:
    naInitBitArrayExtraction(&decsignarray, bitarray, 0, -1);
    if(bitarray && naGetBitArrayCount(bitarray) && withdecsign && *naGetBitArrayBit(bitarray, -1)){
      naDecoupleBitArray(&decsignarray);
      naComputeBitArrayTwosComplement(&decsignarray);
    }else{
      withdecsign = NA_FALSE;
    }
    outstring = naNewStringDecFromBitArray(&decsignarray);
    naClearBitArray(&decsignarray);
    break;
  case NUMBER_SYSTEM_HEX:
    outstring = naNewStringHexFromBitArray(bitarray);
    break;
  case NUMBER_SYSTEM_BIN:
    outstring = naNewStringBinFromBitArray(bitarray);
    break;
  }
  [self fillWithString:outstring withDecSign:withdecsign];
  naDelete(outstring);
}


@end
