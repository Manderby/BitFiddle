
#import "NumberOutputField.h"
#include "BitArray.h"


@implementation NumberOutputField


- (void)setSystem:(NumberSystem)newsystem{
  numbersystem = newsystem;
}

- (void)fillWithString:(NAString*)string withDecSign:(NABool)withdecsign{
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



- (void)fillWithBitArray:(const NABuffer*)bitarray withDecSign:(NABool)withdecsign{
  NAString outstring;
  
  switch(numbersystem){
  case NUMBER_SYSTEM_DEC:
    if(bitarray && naGetBufferRange(bitarray).length && withdecsign && naGetBufferByteAtIndex(bitarray, naGetRangeiMax(naGetBufferRange(bitarray)))){
      NABuffer* twocomp = naCreateBufferCopy(bitarray, naGetBufferRange(bitarray), NA_FALSE);
      naComputeBitArrayTwosComplement(twocomp);
      outstring = naMakeStringDecWithBitArray(twocomp);
      naReleaseBuffer(twocomp);
    }else{
      withdecsign = NA_FALSE;
      outstring = naMakeStringDecWithBitArray(bitarray);
    }
    break;
  case NUMBER_SYSTEM_HEX:
    outstring = naMakeStringHexWithBitArray(bitarray);
    break;
  case NUMBER_SYSTEM_BIN:
    outstring = naMakeStringBinWithBitArray(bitarray);
    break;
  case NUMBER_SYSTEM_ASC:
    outstring = naMakeStringAscWithBitArray(bitarray);
    break;
  }
  [self fillWithString:&outstring withDecSign:withdecsign];
  naClearString(&outstring);
}


@end
