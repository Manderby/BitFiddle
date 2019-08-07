
#import <Cocoa/Cocoa.h>

#include "NAString.h"
#include "BitArray.h"
#include "BitFiddleCommon.h"



@interface NumberOutputField : NSTextView {
  NumberSystem numbersystem;
}

- (void)setSystem:(NumberSystem)system;

- (void)fillWithString:(NAString*) string withDecSign:(NABool)withdecsign;
- (void)fillWithBitArray:(NABuffer*) bitarray withDecSign:(NABool)withdecsign;

@end
