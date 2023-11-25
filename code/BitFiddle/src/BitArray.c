
#include "NAUtility/NABinaryData.h"
#include "NAUtility/NAString.h"
#include "NAStruct/NABuffer.h"
#include "BitArray.h"



// Zero fill
//
// The following function returns for a given negative byteSize, how big a
// buffer must be, to include the appended zero bytes.
//
// The allocation functions of NALib expect a positive or negative byteSize.
// If the byteSize is negative, the absolute value is used to allocate
// sufficient space but a certain number of bytes is appended to the memory
// block which will be initialized with binary zero but will not be visible to
// the programmer.
// 
// The following holds true:
//
// - The additional bytes are all guaranteed to be initialized with binary
//   zero.
// - There are AT LEAST as many bytes appended as one address requires.
//   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
// - There are as many bytes allocated such that the total byteSize of the
//   allocated block is a multiple of an address byteSize, meaning 4 or 8 Bytes
//   depending on the system (32 Bit or 64 Bit).
// - The total byteSize (desired space plus null bytes) is at minimum 2 times
//   the number of bytes needed for an address.
// - When using naMalloc, the part without the additional bytes might partially
//   become initialized with binary zero.
// Example for a 32bit system:
// 0 is a zero-filled byte.
// x is a desired byte,
// z is a desired byte which initially gets overwritten with zero.
// 0:  0000 0000
// 1:  z000 0000
// 2:  zz00 0000
// 3:  zzz0 0000
// 4:  zzzz 0000
// 5:  xxxx z000 0000
// 8:  xxxx zzzz 0000
// 9:  xxxx xxxx z000 0000
// 12: xxxx xxxx zzzz 0000
// 13: xxxx xxxx xxxx z000 0000

NA_HIDEF NAInt bit_GetNullTerminationBytesize(NAInt byteSize){
  NAInt returnByteSize;
  #if NA_DEBUG
    if((byteSize >= NA_ZERO))
      naError("size is not negative");
  #endif
  returnByteSize = (-byteSize - NA_ONE) + (NA_ADDRESS_BYTES << 1) - ((-byteSize - NA_ONE) % NA_ADDRESS_BYTES);
  #if NA_DEBUG
    if(returnByteSize < NA_ZERO)
      naError("given negative size is too close to the minimal integer value");
  #endif
  return returnByteSize;
}


void* bit_Malloc(NAInt byteSize){
  void* retPtr;
  if(byteSize < 0){
    NAInt positiveByteSize = bit_GetNullTerminationBytesize(byteSize);
    retPtr = naMalloc((size_t)positiveByteSize);
    naZeron(&(((NAByte*)retPtr)[positiveByteSize - 8]), 8);
  }else{
    retPtr = naMalloc((size_t)byteSize);
  }
  return retPtr;
}



// Creates a bit array with the specified count.
// - If count is positive, the bit storage has precisely the desired size.
// - If count is negative, enough space will be allocated to hold the absolute
//   number of count as bits. But the returned array will be empty to begin
//   with. If you later perform manipulations on this bitArray which need more
//   space, the previously allocated space will be used.
//   Choose the negative count appropriate to the expected needs. If for
//   example, you expect an add-operation upon the bit array, provide 1 Bit
//   more than needed.
// - If count is zero, an empty bit array is returned.
//
// The returned bit array is always unititialized.
NABuffer* bit_CreateBitArrayWithCount(NAInt count){
  NABuffer* bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(bitArray, naMakeRangei(0, count));
  return bitArray;
}



NABuffer* bitCreateBitArrayCopyWithFixedSize(NABuffer* srcArray, NAInt size){
  NABuffer* bitArray;
  NARangei range = naGetBufferRange(srcArray);
  if(size < 0){
    bitArray = naCreateBufferCopy(srcArray, range, NA_FALSE);
    bitPadBitArray(bitArray, -size);
  }else{
    if(range.length < size){
      bitArray = naCreateBufferCopy(srcArray, range, NA_FALSE);
      bitPadBitArray(bitArray, size);
    }else{
      bitArray = naCreateBufferCopy(srcArray, naMakeRangei(0, size), NA_FALSE);
    }
  }
  return bitArray;
}



void bitPadBitArray(NABuffer* bitArray, NAInt padSize){
  if(!naIsBufferEmpty(bitArray)){
    NABufferIterator iter = naMakeBufferModifier(bitArray);
    naLocateBufferFromEnd(&iter, 0);
    while((naGetBufferRange(bitArray).length == 0) || (naGetBufferRange(bitArray).length % padSize)){
      naWriteBufferu8(&iter, 0);
    }
    naClearBufferIterator(&iter);
  }
}



//NABuffer* bitInitBitArrayShiftExtension( NABuffer* dstarray, NABuffer* srcArray, NAInt shift, NAInt size){
//
//  NAInt i = 0;
//  NABit* srcptr;
//  NABit* dstptr;
//  NABit trailbit;
//  NAInt srcCount;
//
//  #if NA_DEBUG
//    if(!srcArray)
//      naError("bitInitBitArrayShiftExtension", "srcArray is Null-Pointer");
//  #endif
//  if(!size){return naCreateBuffer();}
//  if(size < 0){/*arithmetic = NA_TRUE; */size = -size;}
//  dstarray = bit_CreateBitArrayWithCount(size);
//  dstptr = naGetBufferByteAtIndex(dstarray, 0);
//
//  srcCount = naGetBitArrayCount(srcArray);
//  if(srcCount == 0){
//    naZeron(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
//    return dstarray;
//  }
//
//  if(shift < 0){
//    // Shift right
//    if(-shift >= srcCount){
//      naZeron(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
//      return dstarray;
//    }
//    srcptr = naGetBufferByteAtIndex(srcArray, -shift);
//  }else{
//    // Shift left
//    while((i < shift) && (i < size)){
//      *dstptr++ = BIT0;
//      i++;
//    }
//    srcptr = naGetBufferByteAtIndex(srcArray, 0);
//  }
//
//  // Set the bits of the source
//  while((i < (srcCount + shift)) && (i < size)){
//    *dstptr++ = *srcptr++;
//    i++;
//  }
//  // Set the trailing bits (big endian)
//  trailbit = *naGetBufferByteAtIndex(srcArray, -1);
//  while(i<size){
//    *dstptr++ = trailbit;
//    i++;
//  }
//
//  return dstarray;
//  return NA_NULL;
//}




// Helper function.
// Looks at the given sizehint and expands or extracts the given bitArray and
// fills the padding bits with zero if necessary. Warning: expects fullstorage
// to have enough bits allocated.
//void binEnsureBitArraySizeHint(NABuffer* bitArray, NAInt sizehint){
//  NAUInt bitCount = naGetByteArrayBytesize(&(bitArray->bits));
//  NAInt arraycount = getBitArraySizeHintCount(sizehint, bitCount);
//
//  NAInt sizediff = arraycount - bitCount;
//  if(!bitCount){
//    // if the bitArray was completely empty, just fill it with zeros.
//    naZeron(naGetByteArrayMutablePointer(&(bitArray->fullstorage)), sizediff);
//    naInitByteArrayExtraction(&(bitArray->bits), &(bitArray->fullstorage), 0, arraycount);
//  }else if(sizediff > 0){
//    // The bitArray has fewer bits than needed. Fill with zero.
//    naZeron(naGetByteArrayMutableByte(&(bitArray->bits), -1) + 1, sizediff);
//    naInitByteArrayExtraction(&(bitArray->bits), &(bitArray->fullstorage), 0, arraycount);
//  }else if(sizediff < 0){
//    // The bit array has more bits stored than needed. Extract the relevant
//    // part.
//    naInitByteArrayExtraction(&(bitArray->bits), &(bitArray->bits), 0, arraycount);
//  }else{
//    // The Bit array already has the precise size needed. Do nothing.
//  }
//}



NABuffer* bitCreateBitArrayWithBinString(NAString* string){
  NABuffer* bitArray;
  NABufferIterator iterIn;
  NABufferIterator iterOut;
  NABit curBit;

  bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(naGetStringBufferMutable(string), naGetBufferRange(naGetStringBufferMutable(string)));
  iterIn = naMakeBufferAccessor(naGetStringBufferMutable(string));
  na_PrepareBuffer(&iterIn, (size_t)naGetBufferRange(naGetStringBufferMutable(string)).length);
  naLocateBufferFromEnd(&iterIn, 0);
  iterOut = naMakeBufferModifier(bitArray);
  
  while(naIterateBuffer(&iterIn, -1)){
    NAByte curChar = naGetBufferu8(&iterIn);
    switch (curChar){
    case '0':           curBit = BIT0; break;
    case '1':           curBit = BIT1; break;
    case 'o':           curBit = BIT0; break;
    case 'i':           curBit = BIT1; break;
    case 'O':           curBit = BIT0; break;
    case 'I':           curBit = BIT1; break;
    case 'L':           curBit = BIT1; break;
    default: continue;
    }
    naWriteBufferu8(&iterOut, curBit);
  }
  naClearBufferIterator(&iterIn);
  naClearBufferIterator(&iterOut);
  
  return bitArray;
}



NABuffer* bitCreateBitArrayWithDecString(NAString* string){
  NABuffer* bitArray;
  NABuffer* digit;
  NABufferIterator iterIn;
  NABufferIterator iterDigit;

  bitArray = naCreateBuffer(NA_FALSE);
  
  iterIn = naMakeBufferAccessor(naGetStringBufferMutable(string));
  while(naIterateBuffer(&iterIn, 1)){
    NAByte curChar = naGetBufferu8(&iterIn);
    if((curChar < '0') || (curChar > '9')){continue;}
    curChar -= '0';

    digit = naCreateBuffer(NA_FALSE);
    iterDigit = naMakeBufferModifier(digit);
    naWriteBufferu8(&iterDigit, (curChar & 0x01) == 0x01);
    naWriteBufferu8(&iterDigit, (curChar & 0x02) == 0x02);
    naWriteBufferu8(&iterDigit, (curChar & 0x04) == 0x04);
    naWriteBufferu8(&iterDigit, (curChar & 0x08) == 0x08);
    naClearBufferIterator(&iterDigit);
    
    NABuffer* tentimes = bitCreateBitArrayMulTen(bitArray);
    naRelease(bitArray);
    bitArray = bitCreateBitArrayAddBitArray(tentimes, digit);
    naRelease(tentimes);
    naRelease(digit);
  }
  naClearBufferIterator(&iterIn);
  
  return bitArray;
}



NABuffer* bitCreateBitArrayWithHexString(NAString* string){
  NABuffer* bitArray;
  NABufferIterator iterIn;
  NABufferIterator iterOut;

  bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(naGetStringBufferMutable(string), naGetBufferRange(naGetStringBufferMutable(string)));
  iterIn = naMakeBufferAccessor(naGetStringBufferMutable(string));
  na_PrepareBuffer(&iterIn, (size_t)naGetBufferRange(naGetStringBufferMutable(string)).length);
  naLocateBufferFromEnd(&iterIn, 0);
  iterOut = naMakeBufferModifier(bitArray);
  
  while(naIterateBuffer(&iterIn, -1)){
    NAByte curChar = naGetBufferu8(&iterIn);
    if((curChar >= '0') && (curChar <= '9')){
      naWriteBufferu8(&iterOut, (curChar & 0x01) == 0x01);
      naWriteBufferu8(&iterOut, (curChar & 0x02) == 0x02);
      naWriteBufferu8(&iterOut, (curChar & 0x04) == 0x04);
      naWriteBufferu8(&iterOut, (curChar & 0x08) == 0x08);
    }else if(((curChar >= 'A') && (curChar <= 'F')) || ((curChar >= 'a') && (curChar <= 'f'))){
      curChar += 9;
      naWriteBufferu8(&iterOut, (curChar & 0x01) == 0x01);
      naWriteBufferu8(&iterOut, (curChar & 0x02) == 0x02);
      naWriteBufferu8(&iterOut, (curChar & 0x04) == 0x04);
      naWriteBufferu8(&iterOut, (curChar & 0x08) == 0x08);
    }
  }
  naClearBufferIterator(&iterIn);
  naClearBufferIterator(&iterOut);
  
  return bitArray;
}




NABuffer* bitCreateBitArrayWithAscString(NAString* string){
  NABuffer* bitArray;
  NABufferIterator iterIn;
  NABufferIterator iterOut;

  bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(naGetStringBufferMutable(string), naGetBufferRange(naGetStringBufferMutable(string)));
  iterIn = naMakeBufferAccessor(naGetStringBufferMutable(string));
  na_PrepareBuffer(&iterIn, (size_t)naGetBufferRange(naGetStringBufferMutable(string)).length);
  naLocateBufferFromEnd(&iterIn, 0);
  iterOut = naMakeBufferModifier(bitArray);
  
  while(naIterateBuffer(&iterIn, -1)){
    NAByte curChar = naGetBufferu8(&iterIn);
    naWriteBufferu8(&iterOut, (curChar & 0x01) == 0x01);
    naWriteBufferu8(&iterOut, (curChar & 0x02) == 0x02);
    naWriteBufferu8(&iterOut, (curChar & 0x04) == 0x04);
    naWriteBufferu8(&iterOut, (curChar & 0x08) == 0x08);
    naWriteBufferu8(&iterOut, (curChar & 0x10) == 0x10);
    naWriteBufferu8(&iterOut, (curChar & 0x20) == 0x20);
    naWriteBufferu8(&iterOut, (curChar & 0x40) == 0x40);
    naWriteBufferu8(&iterOut, (curChar & 0x80) == 0x80);
  }
  naClearBufferIterator(&iterIn);
  naClearBufferIterator(&iterOut);
  
  return bitArray;
}


void bitClearBitArray(NABuffer* bitArray){
  naRelease(bitArray);
}





NAString* bitNewStringDecWithBitArray(const NABuffer* bitArray){
  NAUTF8Char* charPtr;
  NAInt outputLen;
  NAInt finalStringCount;
  NABuffer* work;
  NAInt i;
  NAInt j;
  NAString* string;
  NAString* retString;

  NAInt bitCount = naGetBufferRange(bitArray).length;
  if(!bitCount){return naNewString();}
  
  NAUTF8Char* stringbuf = bit_Malloc(-bitCount);
  charPtr = &(stringbuf[bitCount-1]);
  string = naNewStringWithMutableUTF8Buffer(stringbuf, (size_t)bitCount, (NAMutator)naFree);

  outputLen = 0;
  finalStringCount = 0;
  work = naCreateBufferCopy(bitArray, naGetBufferRange(bitArray), NA_TRUE);

  *charPtr = ' ';
  charPtr--;
  finalStringCount++;

  while(bitCount){
    // as long as a remaining value exists
    NABit lead = BIT0;
    NABit bit0;
    NABit bit1;
    NABit bit2;
    NABit bit3;
    uint8 value;
    i = 0;
    j = naGetBufferRange(bitArray).length - 1;
    
    NABufferIterator workIter = naMakeBufferMutator(work);
    naLocateBufferFromStart(&workIter, j-3);
    while(j >= (naGetBufferRange(bitArray).length - bitCount) + 3){
      // walk through the remaining value
      bit0 = naGetBufferu8(&workIter); naIterateBuffer(&workIter, 1);
      bit1 = naGetBufferu8(&workIter); naIterateBuffer(&workIter, 1);
      bit2 = naGetBufferu8(&workIter); naIterateBuffer(&workIter, 1);
      bit3 = naGetBufferu8(&workIter);
      value = (uint8)((bit3 << 3) | (bit2 << 2) | (bit1 << 1) | (bit0 << 0));
      if(value >= 10){
        // For nibbles greaterequal than the value 10, adjust the bits accordingly.
        naSetBufferu8(&workIter, BIT1); naIterateBuffer(&workIter, -1);
        naSetBufferu8(&workIter, bit2 && bit1); naIterateBuffer(&workIter, -1);
        naSetBufferu8(&workIter, !bit1); naIterateBuffer(&workIter, -1);
      }else{
        naSetBufferu8(&workIter, lead); naIterateBuffer(&workIter, -1);
        if(lead){
          // When the previous nibble was 8 or 9, adjust the bits accordingly
          naSetBufferu8(&workIter, !bit1); naIterateBuffer(&workIter, -1);
          naSetBufferu8(&workIter, !bit1); naIterateBuffer(&workIter, -1);
          lead = bit1;
        }else{
          // otherwise, just leave the bits as they are.
          naIterateBuffer(&workIter, -2);
          if(value >= 8){lead = BIT1;}
        }
      }
      naIterateBuffer(&workIter, -1);
      i++;
      j--;
    }
    
    // extract the decimal value of the remaining bits
    if(bitCount == 1){
      naLocateBufferFromStart(&workIter, naGetBufferRange(bitArray).length - 1);
      bit0 = naGetBufferu8(&workIter);
      bit1 = BIT0;
      bit2 = BIT0;
    }else if(bitCount == 2){
      naLocateBufferFromStart(&workIter, naGetBufferRange(bitArray).length - 2);
      bit0 = naGetBufferu8(&workIter); naIterateBuffer(&workIter, 1);
      bit1 = naGetBufferu8(&workIter);
      bit2 = BIT0;
    }else{
      naLocateBufferFromStart(&workIter, j - 2);
      bit0 = naGetBufferu8(&workIter); naIterateBuffer(&workIter, 1);
      bit1 = naGetBufferu8(&workIter); naIterateBuffer(&workIter, 1);
      bit2 = naGetBufferu8(&workIter);
    }
    bit3 = lead;
    value = (uint8)((bit3 << 3) | (bit2 << 2) | (bit1 << 1) | (bit0 << 0));
    if((outputLen > 0) && !(outputLen % 3)){*charPtr-- = ' '; finalStringCount++;}
    *charPtr-- = (NAUTF8Char)(value + '0');
    outputLen++;
    finalStringCount++;
    bitCount = i;
    naClearBufferIterator(&workIter);
  }
  // Remove zeros
  charPtr++;
  while((finalStringCount) && ((*charPtr == '0') || (*charPtr == ' '))){
    charPtr++;
    finalStringCount--;
  }
  
  if(finalStringCount > 1){
    retString = naNewStringExtraction(string, -finalStringCount, -1);
  }else{
    retString = naNewStringWithFormat("0 ");
  }

  naDelete(string);
  naRelease(work);

  return retString;
}



NAString* bitNewStringHexWithBitArray(NABuffer* bitArray){
  NABufferIterator iterIn;
  NABufferIterator iterOut;
  NAInt bitCount;
  uint8 nibble;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterIn = naMakeBufferAccessor(bitArray);
  iterOut = naMakeBufferModifier(buffer);
  
  bitCount = 0;
  nibble = 0;
  while(naIterateBuffer(&iterIn, -1)){
    NAByte byte = naGetBufferu8(&iterIn);
    nibble <<= 1;
    nibble |= byte;
    bitCount++;
    if(!(bitCount % 4)){
      if(nibble < 10){
        naWriteBufferu8(&iterOut, '0' + nibble);
      }else{
        naWriteBufferu8(&iterOut, 'a' + nibble - 10);
      }
      nibble = 0;
      if(bitCount && !(bitCount % 8)){
        naWriteBufferu8(&iterOut, ' ');
      }
    }
  }
  
  naClearBufferIterator(&iterIn);
  naClearBufferIterator(&iterOut);
  
  return string;
}



NAString* bitNewStringBinWithBitArray(NABuffer* bitArray){
  NABufferIterator iterIn;
  NABufferIterator iterOut;
  NAInt bitCount;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterIn = naMakeBufferAccessor(bitArray);
  iterOut = naMakeBufferModifier(buffer);
  
  bitCount = 0;
  while(naIterateBuffer(&iterIn, -1)){
    NAByte byte = naGetBufferu8(&iterIn);
    naWriteBufferu8(&iterOut, '0' + byte);
    bitCount++;
    if(bitCount && !(bitCount % 8)){
      naWriteBufferu8(&iterOut, ' ');
    }
  }
  
  naClearBufferIterator(&iterIn);
  naClearBufferIterator(&iterOut);
  
  return string;
}



NAString* bitNewStringAscWithBitArray(NABuffer* bitArray){
  NABufferIterator iterIn;
  NABufferIterator iterOut;
  NAInt bitCount;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterIn = naMakeBufferAccessor(bitArray);
  iterOut = naMakeBufferModifier(buffer);
  
  NAUTF8Char newChar = 0;
  bitCount = 0;
  while(naIterateBuffer(&iterIn, -1)){
    NAByte byte = naGetBufferu8(&iterIn);
    newChar = (NAUTF8Char)(newChar << 1 | byte);

    bitCount++;
    if(bitCount && !((bitCount - 8) % 32)){
      naWriteBufferu8(&iterOut, ' ');
    }
    if(!(bitCount % 8)){
      if((newChar < 32) || (newChar > 126)){newChar = '?';}
      naWriteBufferu8(&iterOut, (uint8)newChar);
      newChar = 0;
    }
  }
  
  naWriteBufferu8(&iterOut, ' ');

  naClearBufferIterator(&iterIn);
  naClearBufferIterator(&iterOut);
  
  return string;
}



NABit bitComputeBitArrayAddOne(NABuffer* bitArray){
  NABufferIterator iter;
  NABit carry = BIT1;
  
  iter = naMakeBufferMutator(bitArray);
  
  // Remember, BitArray stores in Little Endian.
  while(carry && naIterateBuffer(&iter, 1)){
    carry = naGetBufferu8(&iter);
    naSetBufferu8(&iter, 1 - carry);
  }
  
  naClearBufferIterator(&iter);
  
  return carry;
}



NABuffer* bitCreateBitArrayAddBitArray(NABuffer* srcArray1, NABuffer* srcArray2){
  NABufferIterator dstIter;
  NABufferIterator srcIter1;
  NABufferIterator srcIter2;
  NABit carry;  // Note that carry can be 0, 1, 2 or 3 during the computation
                // but will be 0 or 1 in the end.
  
  NABuffer* bitArray = naCreateBuffer(NA_FALSE);
  dstIter = naMakeBufferModifier(bitArray);
  srcIter1 = naMakeBufferAccessor(srcArray1);
  srcIter2 = naMakeBufferAccessor(srcArray2);
  
  NAInt srcCount1 = naGetBufferRange(srcArray1).length;
  NAInt srcCount2 = naGetBufferRange(srcArray2).length;

  carry = 0;
  
  // Add the two arrays as long as there exist elements in both arrays
  while(srcCount1 && srcCount2){
    carry += naReadBufferu8(&srcIter1);
    carry += naReadBufferu8(&srcIter2);
    srcCount1--;
    srcCount2--;
    naWriteBufferu8(&dstIter, carry & BIT1);
    carry >>= 1;
  }
  
  // Reaching here, one of the counts became 0. Whoever it was, src1 will
  // now become the one still possibly having valid bits.

  if(srcCount1){
    while(srcCount1){
      carry += naReadBufferu8(&srcIter1);
      srcCount1--;
      naWriteBufferu8(&dstIter, carry & BIT1);
      carry >>= 1;
    }
  }else{
    while(srcCount2){
      carry += naReadBufferu8(&srcIter2);
      srcCount2--;
      naWriteBufferu8(&dstIter, carry & BIT1);
      carry >>= 1;
    }
  }

  // Add the remaining carry, if needed.
  if(carry){
    naWriteBufferu8(&dstIter, carry & BIT1);
  }

  naClearBufferIterator(&srcIter1);
  naClearBufferIterator(&srcIter2);
  naClearBufferIterator(&dstIter);

  return bitArray;
}



NABuffer* bitCreateBitArrayMulTen(NABuffer* srcArray){
  if(naIsBufferEmpty(srcArray)){return naCreateBuffer(NA_FALSE);}
  
  NABuffer* bitArray = naCreateBuffer(NA_FALSE);
  NABuffer* bitArray2 = naCreateBufferExtraction(srcArray, 2, naGetBufferRange(srcArray).length - 2);
  NABuffer* addBuffer = bitCreateBitArrayAddBitArray(srcArray, bitArray2);
  
  NABufferIterator iter = naMakeBufferModifier(bitArray);
  NABufferIterator iter2 = naMakeBufferAccessor(srcArray);
  
  naWriteBufferu8(&iter, 0);
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferBuffer(&iter, addBuffer, naGetBufferRange(addBuffer));
  naRelease(addBuffer);
  naRelease(bitArray2);
  
  naClearBufferIterator(&iter2);
  naClearBufferIterator(&iter);
  

  return bitArray;
}



void bitComputeBitArrayOnesComplement(NABuffer* bitArray){
  NABufferIterator iter = naMakeBufferMutator(bitArray);
  while(naIterateBuffer(&iter, 1)){
    naSetBufferu8(&iter, 1 - naGetBufferu8(&iter));
  }
  naClearBufferIterator(&iter);
}



void bitComputeBitArrayTwosComplement(NABuffer* bitArray){
  bitComputeBitArrayOnesComplement(bitArray);
  bitComputeBitArrayAddOne(bitArray);
}



void bitComputeBitArraySwapBytes(NABuffer* bitArray){
  NABufferIterator iter1;
  NABufferIterator iter2;
  #if NA_DEBUG
    if(naGetBufferRange(bitArray).length % 8)
      naError("size of bitArray can not be divided by 8.");
  #endif

  naCacheBufferRange(bitArray, naGetBufferRange(bitArray));
  iter1 = naMakeBufferMutator(bitArray);
  iter2 = naMakeBufferMutator(bitArray);
  naIterateBuffer(&iter1, 1);
  naIterateBuffer(&iter2, -8);
  
  while(naGetBufferLocation(&iter1) < naGetBufferLocation(&iter2)){
    int i;
    for(i = 0; i < 8; i++){
      NAByte byte = naGetBufferu8(&iter1);
      naSetBufferu8(&iter1, naGetBufferu8(&iter2));
      naSetBufferu8(&iter2, byte);
      naIterateBuffer(&iter1, 1);
      naIterateBuffer(&iter2, 1);
    }
    naIterateBuffer(&iter2, -16);
  }
  
  naClearBufferIterator(&iter1);
  naClearBufferIterator(&iter2);
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
