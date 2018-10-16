
#import <Cocoa/Cocoa.h>

#include "NAString.h"
#include "BitArray.h"


typedef enum{
  NUMBER_SYSTEM_DEC = 0,
  NUMBER_SYSTEM_HEX,
  NUMBER_SYSTEM_BIN,
  NUMBER_SYSTEM_ASC
} NumberSystem;


@interface NumberOutputField : NSTextView {
  NumberSystem numbersystem;
}

- (void)setSystem:(NumberSystem)system;

- (void)fillWithString:(NAString*) string withDecSign:(NABool)withdecsign;
- (void)fillWithBitArray:(const NABuffer*) bitarray withDecSign:(NABool)withdecsign;

@end
