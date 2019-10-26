
#include "BitFiddle.h"
#include BIT_NALIB_PATH(NABinaryData.h)
#include BIT_NALIB_PATH(NAString.h)
#include BIT_NALIB_PATH(NABuffer.h)
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
  #ifndef NDEBUG
    if((bytesize >= NA_ZERO))
      naError("size is not negative");
  #endif
  returnbytesize = (-bytesize - NA_ONE) + (NA_SYSTEM_ADDRESS_BYTES << 1) - ((-bytesize - NA_ONE) % NA_SYSTEM_ADDRESS_BYTES);
  #ifndef NDEBUG
    if(returnbytesize < NA_ZERO)
      naError("given negative size is too close to the minimal integer value");
  #endif
  return returnbytesize;
}


void* naBitFiddleMalloc(NAInt bytesize){
  void* retptr;
  if(bytesize < 0){
    NAInt positivebytesize = naGetNullTerminationBytesize(bytesize);
    retptr = naMalloc(positivebytesize);
    naZeron(&(((NAByte*)retptr)[positivebytesize - 8]), 8);
  }else{
    retptr = naMalloc(bytesize);
  }
  return retptr;
}



NABuffer* naCreateBitArrayWithCount(NAInt count){
  NABuffer* bitarray = naNewBuffer(NA_FALSE);
  naCacheBufferRange(bitarray, naMakeRangei(0, count));
  return bitarray;
}



NABuffer* naCreateBitArrayCopyWithFixedSize(NABuffer* srcarray, NAInt size){
  NABuffer* bitarray;
  NARangei range = naGetBufferRange(srcarray);
  if(size < 0){
    bitarray = naNewBufferCopy(srcarray, range, NA_FALSE);
    naPadBitArray(bitarray, -size);
  }else{
    if(range.length < size){
      bitarray = naNewBufferCopy(srcarray, range, NA_FALSE);
      naPadBitArray(bitarray, size);
    }else{
      bitarray = naNewBufferCopy(srcarray, naMakeRangei(0, size), NA_FALSE);
    }
  }
  return bitarray;
}



void naPadBitArray(NABuffer* bitarray, NAInt padsize){
  NABufferIterator iter = naMakeBufferModifier(bitarray);
  naLocateBufferEnd(&iter);
  while((naGetBufferRange(bitarray).length == 0) || (naGetBufferRange(bitarray).length % padsize)){
    naWriteBufferu8(&iter, 0);
  }
  naClearBufferIterator(&iter);
}



//NABuffer* naInitBitArrayShiftExtension( NABuffer* dstarray, NABuffer* srcarray, NAInt shift, NAInt size){
//
//  NAInt i = 0;
//  NABit* srcptr;
//  NABit* dstptr;
//  NABit trailbit;
//  NAInt srccount;
//
//  #ifndef NDEBUG
//    if(!srcarray)
//      naError("naInitBitArrayShiftExtension", "srcarray is Null-Pointer");
//  #endif
//  if(!size){return naNewBuffer();}
//  if(size<0){/*arithmetic = NA_TRUE; */size = -size;}
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
// Looks at the given sizehint and expands or extracts the given bitarray and
// fills the padding bits with zero if necessary. Warning: expects fullstorage
// to have enough bits allocated.
//void naEnsureBitArraySizeHint(NABuffer* bitarray, NAInt sizehint){
//  NAUInt bitcount = naGetByteArrayBytesize(&(bitarray->bits));
//  NAInt arraycount = getBitArraySizeHintCount(sizehint, bitcount);
//
//  NAInt sizediff = arraycount - bitcount;
//  if(!bitcount){
//    // if the bitarray was completely empty, just fill it with zeros.
//    naZeron(naGetByteArrayMutablePointer(&(bitarray->fullstorage)), sizediff);
//    naInitByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
//  }else if(sizediff > 0){
//    // The bitarray has fewer bits than needed. Fill with zero.
//    naZeron(naGetByteArrayMutableByte(&(bitarray->bits), -1) + 1, sizediff);
//    naInitByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
//  }else if(sizediff < 0){
//    // The bit array has more bits stored than needed. Extract the relevant
//    // part.
//    naInitByteArrayExtraction(&(bitarray->bits), &(bitarray->bits), 0, arraycount);
//  }else{
//    // The Bit array already has the precise size needed. Do nothing.
//  }
//}



NABuffer* naCreateBitArrayWithBinString(NAString* string){
  NABuffer* bitarray;
  NABufferIterator iterin;
  NABufferIterator iterout;
  NABit curbit;

  bitarray = naNewBuffer(NA_FALSE);
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  iterout = naMakeBufferModifier(bitarray);
  
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
  
  return bitarray;
}



NABuffer* naCreateBitArrayWithDecString(NAString* string){
  NABuffer* bitarray;
  NABuffer* digit;
  NABufferIterator iterin;
  NABufferIterator iterdigit;

  bitarray = naNewBuffer(NA_FALSE);
  
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  while(naIterateBuffer(&iterin, 1)){
    NAByte curchar = naGetBufferu8(&iterin);
    if((curchar < '0') || (curchar > '9')){continue;}
    curchar -= '0';

    digit = naNewBuffer(NA_FALSE);
    iterdigit = naMakeBufferModifier(digit);
    naWriteBufferu8(&iterdigit, (curchar & 0x01) == 0x01);
    naWriteBufferu8(&iterdigit, (curchar & 0x02) == 0x02);
    naWriteBufferu8(&iterdigit, (curchar & 0x04) == 0x04);
    naWriteBufferu8(&iterdigit, (curchar & 0x08) == 0x08);
    naClearBufferIterator(&iterdigit);
    
    NABuffer* tentimes = naCreateBitArrayMulTen(bitarray);
    naRelease(bitarray);
    bitarray = naCreateBitArrayAddBitArray(tentimes, digit);
    naRelease(tentimes);
    naRelease(digit);
  }
  naClearBufferIterator(&iterin);
  
  return bitarray;
}



NABuffer* naCreateBitArrayWithHexString(NAString* string){
  NABuffer* bitarray;
  NABufferIterator iterin;
  NABufferIterator iterout;

  bitarray = naNewBuffer(NA_FALSE);
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  iterout = naMakeBufferModifier(bitarray);
  
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
  
  return bitarray;
}




NABuffer* naCreateBitArrayWithAscString(NAString* string){
  NABuffer* bitarray;
  NABufferIterator iterin;
  NABufferIterator iterout;

  bitarray = naNewBuffer(NA_FALSE);
  iterin = naMakeBufferAccessor(naGetStringBufferMutable(string));
  iterout = naMakeBufferModifier(bitarray);
  
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
  
  return bitarray;
}


void naClearBitArray(NABuffer* bitarray){
  naRelease(bitarray);
}





NAString* naNewStringDecWithBitArray(const NABuffer* bitarray){
  NAUTF8Char* charptr;
  NAInt outputlen;
  NAInt finalstringcount;
  NABuffer* work;
  NAInt i;
  NAInt j;
  NAString* string;
  NAString* retstring;

  NAInt bitcount = naGetBufferRange(bitarray).length;
  if(!bitcount){return naNewString();}
  
  NAUTF8Char* stringbuf = naBitFiddleMalloc(-bitcount);
  charptr = &(stringbuf[bitcount-1]);
  string = naNewStringWithMutableUTF8Buffer(stringbuf, bitcount, (NAMutator)naFree);

  outputlen = 0;
  finalstringcount = 0;
  work = naNewBufferCopy(bitarray, naGetBufferRange(bitarray), NA_TRUE);

  while(bitcount){
    // as long as a remaining value exists
    NABit lead = BIT0;
    NABit bit0;
    NABit bit1;
    NABit bit2;
    NABit bit3;
    uint8 value;
    i = 0;
    j = naGetBufferRange(bitarray).length - 1;
    
    NABufferIterator workiter = naMakeBufferMutator(work);
    naLocateBufferFromStart(&workiter, j-3);
    while(j >= (naGetBufferRange(bitarray).length - bitcount) + 3){
      // walk through the remaining value
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit2 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit3 = naGetBufferu8(&workiter);
      value = (uint8)((bit3<<3) | (bit2<<2) | (bit1<<1) | (bit0<<0));
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
    if(bitcount==1){
      naLocateBufferFromStart(&workiter, naGetBufferRange(bitarray).length - 1);
      bit0 = naGetBufferu8(&workiter);
      bit1 = BIT0;
      bit2 = BIT0;
    }else if(bitcount==2){
      naLocateBufferFromStart(&workiter, naGetBufferRange(bitarray).length - 2);
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
    value = (uint8)((bit3<<3) | (bit2<<2) | (bit1<<1) | (bit0<<0));
    if((outputlen > 0) && !(outputlen%3)){*charptr-- = ' '; finalstringcount++;}
    *charptr-- = (NAUTF8Char)(value + '0');
    outputlen++;
    finalstringcount++;
    bitcount = i;
    naClearBufferIterator(&workiter);
  }
  // Remove zeros
  charptr++;
  while((finalstringcount) && ((*charptr == '0') || (*charptr == ' '))){
    charptr++;
    finalstringcount--;
  }
  
  if(finalstringcount){
    retstring = naNewStringExtraction(string, -finalstringcount, -1);
  }else{
    retstring = naNewStringWithUTF8CStringLiteral("0");
  }
  naDelete(string);
  naRelease(work);
  return retstring;
}



NAString* naNewStringHexWithBitArray(NABuffer* bitarray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitcount;
  uint8 nibble;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterin = naMakeBufferAccessor(bitarray);
  iterout = naMakeBufferModifier(buffer);
  
  bitcount = naGetBufferRange(bitarray).length;
  nibble = 0;
  while(naIterateBuffer(&iterin, -1)){
    NAByte byte = naGetBufferu8(&iterin);
    nibble <<= 1;
    nibble |= byte;
    bitcount--;
    if(!(bitcount % 4)){
      if(nibble < 10){
        naWriteBufferu8(&iterout, '0' + nibble);
      }else{
        naWriteBufferu8(&iterout, 'a' + nibble - 10);
      }
      nibble = 0;
      if(bitcount && !(bitcount % 8)){
        naWriteBufferu8(&iterout, ' ');
      }
    }
  }
  
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return string;
}



NAString* naNewStringBinWithBitArray(NABuffer* bitarray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitcount;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterin = naMakeBufferAccessor(bitarray);
  iterout = naMakeBufferModifier(buffer);
  
  bitcount = 0;
  while(naIterateBuffer(&iterin, -1)){
    NAByte byte = naGetBufferu8(&iterin);
    naWriteBufferu8(&iterout, '0' + byte);
    bitcount++;
    if(bitcount && !(bitcount % 8)){
      naWriteBufferu8(&iterout, ' ');
    }
  }
  
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return string;
}



NAString* naNewStringAscWithBitArray(NABuffer* bitarray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitcount;
  
  NAString* string = naNewString();
  NABuffer* buffer = naGetStringBufferMutable(string);
  
  iterin = naMakeBufferAccessor(bitarray);
  iterout = naMakeBufferModifier(buffer);
  
  NAUTF8Char newchar = 0;
  bitcount = 0;
  while(naIterateBuffer(&iterin, -1)){
    NAByte byte = naGetBufferu8(&iterin);
    newchar = (NAUTF8Char)(newchar << 1 | byte);

    bitcount++;
    if(bitcount && !((bitcount - 8) % 32)){
      naWriteBufferu8(&iterout, ' ');
    }
    if(!(bitcount % 8)){
      if((newchar < 32) || (newchar > 126)){newchar = '?';}
      naWriteBufferu8(&iterout, (uint8)newchar);
      newchar = 0;
    }
  }
  
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return string;
}



NABit naComputeBitArrayAddOne(NABuffer* bitarray){
  NABufferIterator iter;
  NABit carry = BIT1;
  
  iter = naMakeBufferMutator(bitarray);
  
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
  
  NABuffer* bitarray = naNewBuffer(NA_FALSE);
  dstiter = naMakeBufferModifier(bitarray);
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

  return bitarray;
}



NABuffer* naCreateBitArrayMulTen(NABuffer* srcarray){
  if(naIsBufferEmpty(srcarray)){return naNewBuffer(NA_FALSE);}
  
  NABuffer* bitarray = naNewBuffer(NA_FALSE);
  NABuffer* bitarray2 = naNewBufferExtraction(srcarray, 2, naGetBufferRange(srcarray).length - 2);
  NABuffer* addbuffer = naCreateBitArrayAddBitArray(srcarray, bitarray2);
  
  NABufferIterator iter = naMakeBufferModifier(bitarray);
  NABufferIterator iter2 = naMakeBufferAccessor(srcarray);
  
  naWriteBufferu8(&iter, 0);
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferBuffer(&iter, addbuffer, naGetBufferRange(addbuffer));
  naRelease(addbuffer);
  naRelease(bitarray2);
  
  naClearBufferIterator(&iter2);
  naClearBufferIterator(&iter);
  

  return bitarray;
}



void naComputeBitArrayOnesComplement(NABuffer* bitarray){
  NABufferIterator iter = naMakeBufferMutator(bitarray);
  while(naIterateBuffer(&iter, 1)){
    naSetBufferu8(&iter, 1 - naGetBufferu8(&iter));
  }
  naClearBufferIterator(&iter);
}



void naComputeBitArrayTwosComplement(NABuffer* bitarray){
  naComputeBitArrayOnesComplement(bitarray);
  naComputeBitArrayAddOne(bitarray);
}



void naComputeBitArraySwapBytes(NABuffer* bitarray){
  NABufferIterator iter1;
  NABufferIterator iter2;
  #ifndef NDEBUG
    if(naGetBufferRange(bitarray).length % 8)
      naError("size of bitarray can not be divided by 8.");
  #endif

  naCacheBufferRange(bitarray, naGetBufferRange(bitarray));
  iter1 = naMakeBufferMutator(bitarray);
  iter2 = naMakeBufferMutator(bitarray);
  naIterateBuffer(&iter1, 1);
  naIterateBuffer(&iter2, -8);
  
  while(naGetBufferLocation(&iter1) < naGetBufferLocation(&iter2)){
    for(int i=0; i<8; i++){
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



