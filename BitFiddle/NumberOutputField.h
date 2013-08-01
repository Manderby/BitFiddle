
#import <Cocoa/Cocoa.h>

#include "NAString.h"
#include "NABitArray.h"


typedef enum{
  NUMBER_SYSTEM_DEC = 0,
  NUMBER_SYSTEM_HEX,
  NUMBER_SYSTEM_BIN
} NumberSystem;


@interface NumberOutputField : NSTextView {
  NumberSystem numbersystem;
  NAInt bitcount;
}

- (void)setSystem:(NumberSystem)system withBitCount:(NAInt)count;

- (void)fillWithString:(NAString*) string withDecSign:(NABool)withdecsign;
- (void)fillWithBitArray:(NABitArray*) bitarray withDecSign:(NABool)withdecsign;

@end
