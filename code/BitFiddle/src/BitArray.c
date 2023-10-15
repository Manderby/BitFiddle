
#include "NAUtility/NABinaryData.h"
#include "NAUtility/NAString.h"
#include "NAStruct/NABuffer.h"
#include "BitArray.h"




// Zero fill
//
// The following function returns for a given negative bytesize, how big a
// buffer must be, to include the appended zero bytes.
//
// The allocation functions of NALib expect a positive or negative bytesize.
// If the bytesize is negative, the absolute value is used to allocate
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
// - There are as many bytes allocated such that the total bytesize of the
//   allocated block is a multiple of an address bytesize, meaning 4 or 8 Bytes
//   depending on the system (32 Bit or 64 Bit).
// - The total bytesize (desired space plus null bytes) is at minimum 2 times
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

NA_HIDEF NAInt naGetNullTerminationBytesize(NAInt bytesize){
  NAInt returnbytesize;
  #if NA_DEBUG
    if((bytesize >= NA_ZERO))
      naError("size is not negative");
  #endif
  returnbytesize = (-bytesize - NA_ONE) + (NA_ADDRESS_BYTES << 1) - ((-bytesize - NA_ONE) % NA_ADDRESS_BYTES);
  #if NA_DEBUG
    if(returnbytesize < NA_ZERO)
      naError("given negative size is too close to the minimal integer value");
  #endif
  return returnbytesize;
}


void* naBitFiddleMalloc(NAInt bytesize){
  void* retptr;
  if(bytesize < 0){
    NAInt positivebytesize = naGetNullTerminationBytesize(bytesize);
    retptr = naMalloc((size_t)positivebytesize);
    naZeron(&(((NAByte*)retptr)[positivebytesize - 8]), 8);
  }else{
    retptr = naMalloc((size_t)bytesize);
  }
  return retptr;
}



NABuffer* naCreateBitArrayWithCount(NAInt count){
  NABuffer* bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(bitArray, naMakeRangei(0, count));
  return bitArray;
}



NABuffer* naCreateBitArrayCopyWithFixedSize(NABuffer* srcarray, NAInt size){
  NABuffer* bitArray;
  NARangei range = naGetBufferRange(srcarray);
  if(size < 0){
    bitArray = naCreateBufferCopy(srcarray, range, NA_FALSE);
    naPadBitArray(bitArray, -size);
  }else{
    if(range.length < size){
      bitArray = naCreateBufferCopy(srcarray, range, NA_FALSE);
      naPadBitArray(bitArray, size);
    }else{
      bitArray = naCreateBufferCopy(srcarray, naMakeRangei(0, size), NA_FALSE);
    }
  }
  return bitArray;
}



void naPadBitArray(NABuffer* bitArray, NAInt padsize){
  if(!naIsBufferEmpty(bitArray)){
    NABufferIterator iter = naMakeBufferModifier(bitArray);
    naLocateBufferFromEnd(&iter, 0);
    while((naGetBufferRange(bitArray).length == 0) || (naGetBufferRange(bitArray).length % padsize)){
      naWriteBufferu8(&iter, 0);
    }
    naClearBufferIterator(&iter);
  }
}



//NABuffer* naInitBitArrayShiftExtension( NABuffer* dstarray, NABuffer* srcarray, NAInt shift, NAInt size){
//
//  NAInt i = 0;
//  NABit* srcptr;
//  NABit* dstptr;
//  NABit trailbit;
//  NAInt srccount;
//
//  #if NA_DEBUG
//    if(!srcarray)
//      naError("naInitBitArrayShiftExtension", "srcarray is Null-Pointer");
//  #endif
//  if(!size){return naCreateBuffer();}
//  if(size < 0){/*arithmetic = NA_TRUE; */size = -size;}
//  dstarray = naCreateBitArrayWithCount(size);
//  dstptr = naGetBufferByteAtIndex(dstarray, 0);
//
//  srccount = naGetBitArrayCount(srcarray);
//  if(srccount == 0){
//    naZeron(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
//    return dstarray;
//  }
//
//  if(shift < 0){
//    // Shift right
//    if(-shift >= srccount){
//      naZeron(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
//      return dstarray;
//    }
//    srcptr = naGetBufferByteAtIndex(srcarray, -shift);
//  }else{
//    // Shift left
//    while((i < shift) && (i < size)){
//      *dstptr++ = BIT0;
//      i++;
//    }
//    srcptr = naGetBufferByteAtIndex(srcarray, 0);
//  }
//
//  // Set the bits of the source
//  while((i < (srccount + shift)) && (i < size)){
//    *dstptr++ = *srcptr++;
//    i++;
//  }
//  // Set the trailing bits (big endian)
//  trailbit = *naGetBufferByteAtIndex(srcarray, -1);
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
//void naEnsureBitArraySizeHint(NABuffer* bitArray, NAInt sizehint){
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



NABuffer* naCreateBitArrayWithBinString(NAString* string){
  NABuffer* bitArray;
  NABufferIterator iterin;
  NABufferIterator iterout;
  NABit curbit;

  bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(naGetStringBufferMutable(string), naGetBufferRange(naGetStringBufferMutable(string)));
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  na_PrepareBuffer(&iterin, (size_t)naGetBufferRange(naGetStringBufferMutable(string)).length);
  naLocateBufferFromEnd(&iterin, 0);
  iterout = naMakeBufferModifier(bitArray);
  
  while(naIterateBuffer(&iterin, -1)){
    NAByte curchar = naGetBufferu8(&iterin);
    switch (curchar){
    case '0':           curbit = BIT0; break;
    case '1':           curbit = BIT1; break;
    case 'o':           curbit = BIT0; break;
    case 'i':           curbit = BIT1; break;
    case 'O':           curbit = BIT0; break;
    case 'I':           curbit = BIT1; break;
    case 'L':           curbit = BIT1; break;
    default: continue;
    }
    naWriteBufferu8(&iterout, curbit);
  }
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return bitArray;
}



NABuffer* naCreateBitArrayWithDecString(NAString* string){
  NABuffer* bitArray;
  NABuffer* digit;
  NABufferIterator iterin;
  NABufferIterator iterdigit;

  bitArray = naCreateBuffer(NA_FALSE);
  
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  while(naIterateBuffer(&iterin, 1)){
    NAByte curchar = naGetBufferu8(&iterin);
    if((curchar < '0') || (curchar > '9')){continue;}
    curchar -= '0';

    digit = naCreateBuffer(NA_FALSE);
    iterdigit = naMakeBufferModifier(digit);
    naWriteBufferu8(&iterdigit, (curchar & 0x01) == 0x01);
    naWriteBufferu8(&iterdigit, (curchar & 0x02) == 0x02);
    naWriteBufferu8(&iterdigit, (curchar & 0x04) == 0x04);
    naWriteBufferu8(&iterdigit, (curchar & 0x08) == 0x08);
    naClearBufferIterator(&iterdigit);
    
    NABuffer* tentimes = naCreateBitArrayMulTen(bitArray);
    naRelease(bitArray);
    bitArray = naCreateBitArrayAddBitArray(tentimes, digit);
    naRelease(tentimes);
    naRelease(digit);
  }
  naClearBufferIterator(&iterin);
  
  return bitArray;
}



NABuffer* naCreateBitArrayWithHexString(NAString* string){
  NABuffer* bitArray;
  NABufferIterator iterin;
  NABufferIterator iterout;

  bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(naGetStringBufferMutable(string), naGetBufferRange(naGetStringBufferMutable(string)));
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  na_PrepareBuffer(&iterin, (size_t)naGetBufferRange(naGetStringBufferMutable(string)).length);
  naLocateBufferFromEnd(&iterin, 0);
  iterout = naMakeBufferModifier(bitArray);
  
  while(naIterateBuffer(&iterin, -1)){
    NAByte curchar = naGetBufferu8(&iterin);
    if((curchar >= '0') && (curchar <= '9')){
      naWriteBufferu8(&iterout, (curchar & 0x01) == 0x01);
      naWriteBufferu8(&iterout, (curchar & 0x02) == 0x02);
      naWriteBufferu8(&iterout, (curchar & 0x04) == 0x04);
      naWriteBufferu8(&iterout, (curchar & 0x08) == 0x08);
    }else if(((curchar >= 'A') && (curchar <= 'F')) || ((curchar >= 'a') && (curchar <= 'f'))){
      curchar += 9;
      naWriteBufferu8(&iterout, (curchar & 0x01) == 0x01);
      naWriteBufferu8(&iterout, (curchar & 0x02) == 0x02);
      naWriteBufferu8(&iterout, (curchar & 0x04) == 0x04);
      naWriteBufferu8(&iterout, (curchar & 0x08) == 0x08);
    }
  }
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return bitArray;
}




NABuffer* naCreateBitArrayWithAscString(NAString* string){
  NABuffer* bitArray;
  NABufferIterator iterin;
  NABufferIterator iterout;

  bitArray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(naGetStringBufferMutable(string), naGetBufferRange(naGetStringBufferMutable(string)));
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  na_PrepareBuffer(&iterin, (size_t)naGetBufferRange(naGetStringBufferMutable(string)).length);
  naLocateBufferFromEnd(&iterin, 0);
  iterout = naMakeBufferModifier(bitArray);
  
  while(naIterateBuffer(&iterin, -1)){
    NAByte curchar = naGetBufferu8(&iterin);
    naWriteBufferu8(&iterout, (curchar & 0x01) == 0x01);
    naWriteBufferu8(&iterout, (curchar & 0x02) == 0x02);
    naWriteBufferu8(&iterout, (curchar & 0x04) == 0x04);
    naWriteBufferu8(&iterout, (curchar & 0x08) == 0x08);
    naWriteBufferu8(&iterout, (curchar & 0x10) == 0x10);
    naWriteBufferu8(&iterout, (curchar & 0x20) == 0x20);
    naWriteBufferu8(&iterout, (curchar & 0x40) == 0x40);
    naWriteBufferu8(&iterout, (curchar & 0x80) == 0x80);
  }
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return bitArray;
}


void naClearBitArray(NABuffer* bitArray){
  naRelease(bitArray);
}





NAString* naNewStringDecWithBitArray(const NABuffer* bitArray){
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
  
  NAUTF8Char* stringbuf = naBitFiddleMalloc(-bitCount);
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
    
    NABufferIterator workiter = naMakeBufferMutator(work);
    naLocateBufferFromStart(&workiter, j-3);
    while(j >= (naGetBufferRange(bitArray).length - bitCount) + 3){
      // walk through the remaining value
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit2 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit3 = naGetBufferu8(&workiter);
      value = (uint8)((bit3 << 3) | (bit2 << 2) | (bit1 << 1) | (bit0 << 0));
      if(value >= 10){
        // For nibbles greaterequal than the value 10, adjust the bits accordingly.
        naSetBufferu8(&workiter, BIT1); naIterateBuffer(&workiter, -1);
        naSetBufferu8(&workiter, bit2 && bit1); naIterateBuffer(&workiter, -1);
        naSetBufferu8(&workiter, !bit1); naIterateBuffer(&workiter, -1);
      }else{
        naSetBufferu8(&workiter, lead); naIterateBuffer(&workiter, -1);
        if(lead){
          // When the previous nibble was 8 or 9, adjust the bits accordingly
          naSetBufferu8(&workiter, !bit1); naIterateBuffer(&workiter, -1);
          naSetBufferu8(&workiter, !bit1); naIterateBuffer(&workiter, -1);
          lead = bit1;
        }else{
          // otherwise, just leave the bits as they are.
          naIterateBuffer(&workiter, -2);
          if(value >= 8){lead = BIT1;}
        }
      }
      naIterateBuffer(&workiter, -1);
      i++;
      j--;
    }
    
    // extract the decimal value of the remaining bits
    if(bitCount == 1){
      naLocateBufferFromStart(&workiter, naGetBufferRange(bitArray).length - 1);
      bit0 = naGetBufferu8(&workiter);
      bit1 = BIT0;
      bit2 = BIT0;
    }else if(bitCount == 2){
      naLocateBufferFromStart(&workiter, naGetBufferRange(bitArray).length - 2);
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter);
      bit2 = BIT0;
    }else{
      naLocateBufferFromStart(&workiter, j - 2);
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit2 = naGetBufferu8(&workiter);
    }
    bit3 = lead;
    value = (uint8)((bit3 << 3) | (bit2 << 2) | (bit1 << 1) | (bit0 << 0));
    if((outputLen > 0) && !(outputLen % 3)){*charPtr-- = ' '; finalStringCount++;}
    *charPtr-- = (NAUTF8Char)(value + '0');
    outputLen++;
    finalStringCount++;
    bitCount = i;
    naClearBufferIterator(&workiter);
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

  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif

  naDelete(string);
  naRelease(work);

  return retString;
}



NAString* naNewStringHexWithBitArray(NABuffer* bitArray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitCount;
  uint8 nibble;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterin = naMakeBufferAccessor(bitArray);
  iterout = naMakeBufferModifier(buffer);
  
  bitCount = 0;
  nibble = 0;
  while(naIterateBuffer(&iterin, -1)){
    NAByte byte = naGetBufferu8(&iterin);
    nibble <<= 1;
    nibble |= byte;
    bitCount++;
    if(!(bitCount % 4)){
      if(nibble < 10){
        naWriteBufferu8(&iterout, '0' + nibble);
      }else{
        naWriteBufferu8(&iterout, 'a' + nibble - 10);
      }
      nibble = 0;
      if(bitCount && !(bitCount % 8)){
        naWriteBufferu8(&iterout, ' ');
      }
    }
  }
  
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
//#if NA_STRING_ALWAYS_CACHE == 1
//    naGetStringUTF8Pointer(string);
//  #endif

  return string;
}



NAString* naNewStringBinWithBitArray(NABuffer* bitArray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitCount;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterin = naMakeBufferAccessor(bitArray);
  iterout = naMakeBufferModifier(buffer);
  
  bitCount = 0;
  while(naIterateBuffer(&iterin, -1)){
    NAByte byte = naGetBufferu8(&iterin);
    naWriteBufferu8(&iterout, '0' + byte);
    bitCount++;
    if(bitCount && !(bitCount % 8)){
      naWriteBufferu8(&iterout, ' ');
    }
  }
  
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
//  #if NA_STRING_ALWAYS_CACHE == 1
//    naGetStringUTF8Pointer(string);
//  #endif
  
  return string;
}



NAString* naNewStringAscWithBitArray(NABuffer* bitArray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitCount;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterin = naMakeBufferAccessor(bitArray);
  iterout = naMakeBufferModifier(buffer);
  
  NAUTF8Char newchar = 0;
  bitCount = 0;
  while(naIterateBuffer(&iterin, -1)){
    NAByte byte = naGetBufferu8(&iterin);
    newchar = (NAUTF8Char)(newchar << 1 | byte);

    bitCount++;
    if(bitCount && !((bitCount - 8) % 32)){
      naWriteBufferu8(&iterout, ' ');
    }
    if(!(bitCount % 8)){
      if((newchar < 32) || (newchar > 126)){newchar = '?';}
      naWriteBufferu8(&iterout, (uint8)newchar);
      newchar = 0;
    }
  }
  
  naWriteBufferu8(&iterout, ' ');

  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
//  #if NA_STRING_ALWAYS_CACHE == 1
//    naGetStringUTF8Pointer(string);
//  #endif

  return string;
}



NABit naComputeBitArrayAddOne(NABuffer* bitArray){
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



NABuffer* naCreateBitArrayAddBitArray(NABuffer* srcarray1, NABuffer* srcarray2){
  NABufferIterator dstiter;
  NABufferIterator srciter1;
  NABufferIterator srciter2;
  NABit carry;  // Note that carry can be 0, 1, 2 or 3 during the computation
                // but will be 0 or 1 in the end.
  
  NABuffer* bitArray = naCreateBuffer(NA_FALSE);
  dstiter = naMakeBufferModifier(bitArray);
  srciter1 = naMakeBufferAccessor(srcarray1);
  srciter2 = naMakeBufferAccessor(srcarray2);
  
  NAInt srccount1 = naGetBufferRange(srcarray1).length;
  NAInt srccount2 = naGetBufferRange(srcarray2).length;

  carry = 0;
  
  // Add the two arrays as long as there exist elements in both arrays
  while(srccount1 && srccount2){
    carry += naReadBufferu8(&srciter1);
    carry += naReadBufferu8(&srciter2);
    srccount1--;
    srccount2--;
    naWriteBufferu8(&dstiter, carry & BIT1);
    carry >>= 1;
  }
  
  // Reaching here, one of the counts became 0. Whoever it was, src1 will
  // now become the one still possibly having valid bits.

  if(srccount1){
    while(srccount1){
      carry += naReadBufferu8(&srciter1);
      srccount1--;
      naWriteBufferu8(&dstiter, carry & BIT1);
      carry >>= 1;
    }
  }else{
    while(srccount2){
      carry += naReadBufferu8(&srciter2);
      srccount2--;
      naWriteBufferu8(&dstiter, carry & BIT1);
      carry >>= 1;
    }
  }

  // Add the remaining carry, if needed.
  if(carry){
    naWriteBufferu8(&dstiter, carry & BIT1);
  }

  naClearBufferIterator(&srciter1);
  naClearBufferIterator(&srciter2);
  naClearBufferIterator(&dstiter);

  return bitArray;
}



NABuffer* naCreateBitArrayMulTen(NABuffer* srcarray){
  if(naIsBufferEmpty(srcarray)){return naCreateBuffer(NA_FALSE);}
  
  NABuffer* bitArray = naCreateBuffer(NA_FALSE);
  NABuffer* bitArray2 = naCreateBufferExtraction(srcarray, 2, naGetBufferRange(srcarray).length - 2);
  NABuffer* addbuffer = naCreateBitArrayAddBitArray(srcarray, bitArray2);
  
  NABufferIterator iter = naMakeBufferModifier(bitArray);
  NABufferIterator iter2 = naMakeBufferAccessor(srcarray);
  
  naWriteBufferu8(&iter, 0);
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferBuffer(&iter, addbuffer, naGetBufferRange(addbuffer));
  naRelease(addbuffer);
  naRelease(bitArray2);
  
  naClearBufferIterator(&iter2);
  naClearBufferIterator(&iter);
  

  return bitArray;
}



void naComputeBitArrayOnesComplement(NABuffer* bitArray){
  NABufferIterator iter = naMakeBufferMutator(bitArray);
  while(naIterateBuffer(&iter, 1)){
    naSetBufferu8(&iter, 1 - naGetBufferu8(&iter));
  }
  naClearBufferIterator(&iter);
}



void naComputeBitArrayTwosComplement(NABuffer* bitArray){
  naComputeBitArrayOnesComplement(bitArray);
  naComputeBitArrayAddOne(bitArray);
}



void naComputeBitArraySwapBytes(NABuffer* bitArray){
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
