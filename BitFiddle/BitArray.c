
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NABinaryData.h"
#include "BitArray.h"
#include "NAString.h"
#include "NABuffer.h"





NABuffer* naCreateBitArrayWithCount(NAInt count){
  NABuffer* bitarray = naCreateBuffer(NA_FALSE);
  naCacheBufferRange(bitarray, naMakeRangei(0, count), NA_FALSE);
  return bitarray;
}



NABuffer* naCreateBitArrayCopyWithFixedSize(NABuffer* srcarray, NAInt size){
  NABuffer* bitarray;
  NARangei range = naGetBufferRange(srcarray);
  if(size < 0){
    bitarray = naCreateBufferCopy(srcarray, range, NA_FALSE);
    naPadBitArray(bitarray, -size);
  }else{
    if(range.length < size){
      bitarray = naCreateBufferCopy(srcarray, range, NA_FALSE);
      naPadBitArray(bitarray, size);
    }else{
      bitarray = naCreateBufferCopy(srcarray, naMakeRangei(0, size), NA_FALSE);
    }
  }
  return bitarray;
}



void naPadBitArray(NABuffer* bitarray, NAInt padsize){
  NABufferIterator iter = naMakeBufferIteratorModifier(bitarray);
  while((naGetBufferRange(bitarray).length == 0) || (naGetBufferRange(bitarray).length % padsize)){
    naWriteBufferu8(&iter, 0);
  }
  naClearBufferIterator(&iter);
}



NABuffer* naInitBitArrayShiftExtension( NABuffer* dstarray, NABuffer* srcarray, NAInt shift, NAInt size){

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
//  if(!size){return naCreateBufferPlain();}
//  if(size<0){/*arithmetic = NA_TRUE; */size = -size;}
//  dstarray = naCreateBitArrayWithCount(size);
//  dstptr = naGetBufferByteAtIndex(dstarray, 0);
//
//  srccount = naGetBitArrayCount(srcarray);
//  if(srccount == 0){
//    naNulln(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
//    return dstarray;
//  }
//
//  if(shift < 0){
//    // Shift right
//    if(-shift >= srccount){
//      naNulln(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
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
  return NA_NULL;
}




// Helper function.
// Looks at the given sizehint and expands or extracts the given bitarray and
// fills the padding bits with zero if necessary. Warning: expects fullstorage
// to have enough bits allocated.
void naEnsureBitArraySizeHint(NABuffer* bitarray, NAInt sizehint){
//  NAUInt bitcount = naGetByteArrayBytesize(&(bitarray->bits));
//  NAInt arraycount = getBitArraySizeHintCount(sizehint, bitcount);
//
//  NAInt sizediff = arraycount - bitcount;
//  if(!bitcount){
//    // if the bitarray was completely empty, just fill it with zeros.
//    naNulln(naGetByteArrayMutablePointer(&(bitarray->fullstorage)), sizediff);
//    naInitByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
//  }else if(sizediff > 0){
//    // The bitarray has fewer bits than needed. Fill with zero.
//    naNulln(naGetByteArrayMutableByte(&(bitarray->bits), -1) + 1, sizediff);
//    naInitByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
//  }else if(sizediff < 0){
//    // The bit array has more bits stored than needed. Extract the relevant
//    // part.
//    naInitByteArrayExtraction(&(bitarray->bits), &(bitarray->bits), 0, arraycount);
//  }else{
//    // The Bit array already has the precise size needed. Do nothing.
//  }
}



NABuffer* naCreateBitArrayWithBinString(NAString* string){
  NABuffer* bitarray;
  NABufferIterator iterin;
  NABufferIterator iterout;
  NABit curbit;

  bitarray = naCreateBuffer(NA_FALSE);
  iterin = naMakeBufferIteratorAccessor(naGetStringBufferConst(string));
  iterout = naMakeBufferIteratorModifier(bitarray);
  
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

  bitarray = naCreateBufferPlain();
  
  iterin = naMakeBufferIteratorAccessor(naGetStringBufferConst(string));
  while(naIterateBuffer(&iterin, 1)){
    NAByte curchar = naGetBufferu8(&iterin);
    if((curchar < '0') || (curchar > '9')){continue;}
    curchar -= '0';

    digit = naCreateBuffer(NA_FALSE);
    iterdigit = naMakeBufferIteratorModifier(digit);
    naWriteBufferu8(&iterdigit, (curchar & 0x01) == 0x01);
    naWriteBufferu8(&iterdigit, (curchar & 0x02) == 0x02);
    naWriteBufferu8(&iterdigit, (curchar & 0x04) == 0x04);
    naWriteBufferu8(&iterdigit, (curchar & 0x08) == 0x08);
    naClearBufferIterator(&iterdigit);
    
    NABuffer* tentimes = naCreateBitArrayMulTen(bitarray);
    naReleaseBuffer(bitarray);
    bitarray = naCreateBitArrayAddBitArray(tentimes, digit);
    naReleaseBuffer(tentimes);
    naReleaseBuffer(digit);
  }
  naClearBufferIterator(&iterin);
  
  return bitarray;
}



NABuffer* naCreateBitArrayWithHexString(NAString* string){
  NABuffer* bitarray;
  NABufferIterator iterin;
  NABufferIterator iterout;

  bitarray = naCreateBuffer(NA_FALSE);
  iterin = naMakeBufferIteratorAccessor(naGetStringBufferConst(string));
  iterout = naMakeBufferIteratorModifier(bitarray);
  
  while(naIterateBuffer(&iterin, -1)){
    NAByte curchar = naGetBufferu8(&iterin);
    naWriteBufferu8(&iterout, (curchar & 0x01) == 0x01);
    naWriteBufferu8(&iterout, (curchar & 0x02) == 0x02);
    naWriteBufferu8(&iterout, (curchar & 0x04) == 0x04);
    naWriteBufferu8(&iterout, (curchar & 0x08) == 0x08);
  }
  naClearBufferIterator(&iterin);
  naClearBufferIterator(&iterout);
  
  return bitarray;
}




NABuffer* naCreateBitArrayWithAscString(NAString* string){
  NABuffer* bitarray;
  NABufferIterator iterin;
  NABufferIterator iterout;

  bitarray = naCreateBuffer(NA_FALSE);
  iterin = naMakeBufferIteratorAccessor(naGetStringBufferConst(string));
  iterout = naMakeBufferIteratorModifier(bitarray);
  
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
  naReleaseBuffer(bitarray);
}





NAString naMakeStringDecWithBitArray(const NABuffer* bitarray){
  NAUTF8Char* charptr;
  NAInt outputlen;
  NAInt finalstringcount;
  NABuffer* work;
  NAUInt i;
  NAUInt j;
  NAString string;
  NAString retstring;

  NAUInt bitcount = naGetBufferRange(bitarray).length;
  if(!bitcount){return naMakeString();}
  
  NAUTF8Char* stringbuf = naMalloc(-bitcount);
  charptr = &(stringbuf[bitcount-1]);
  string = naMakeStringWithMutableUTF8Buffer(stringbuf, -bitcount, NA_MEMORY_CLEANUP_FREE);

  outputlen = 0;
  finalstringcount = 0;
  work = naCreateBufferCopy(bitarray, naGetBufferRange(bitarray), NA_TRUE);

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
    
    NABufferIterator workiter = naMakeBufferIteratorMutator(work);
    naSeekBufferFromStart(&workiter, j-3);
    while(j >= (naGetBufferRange(bitarray).length - bitcount) + 3){
      // walk through the remaining value
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit2 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit3 = naGetBufferu8(&workiter);
      value = (bit3<<3) | (bit2<<2) | (bit1<<1) | (bit0<<0);
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
      naSeekBufferFromStart(&workiter, naGetBufferRange(bitarray).length - 1);
      bit0 = naGetBufferu8(&workiter);
      bit1 = BIT0;
      bit2 = BIT0;
    }else if(bitcount==2){
      naSeekBufferFromStart(&workiter, naGetBufferRange(bitarray).length - 2);
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter);
      bit2 = BIT0;
    }else{
      naSeekBufferFromStart(&workiter, j - 2);
      bit0 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit1 = naGetBufferu8(&workiter); naIterateBuffer(&workiter, 1);
      bit2 = naGetBufferu8(&workiter);
    }
    bit3 = lead;
    value = (bit3<<3) | (bit2<<2) | (bit1<<1) | (bit0<<0);
    if((outputlen > 0) && !(outputlen%3)){*charptr-- = ' '; finalstringcount++;}
    *charptr-- = value + '0';
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
    retstring = naMakeStringExtraction(&string, -finalstringcount, -1);
  }else{
    retstring = naMakeStringWithUTF8CStringLiteral("0");
  }
  naClearString(&string);
  naReleaseBuffer(work);
  return retstring;
}



NAString naMakeStringHexWithBitArray(const NABuffer* bitarray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitcount;
  uint8 nibble;
  
  NAString string = naMakeString();
  NABuffer* buffer = naGetStringBufferMutable(&string);
  
  iterin = naMakeBufferIteratorAccessor(bitarray);
  iterout = naMakeBufferIteratorModifier(buffer);
  
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



NAString naMakeStringBinWithBitArray(const NABuffer* bitarray){
  NABufferIterator iterin;
  NABufferIterator iterout;
  NAInt bitcount;
  
  NAString string = naMakeString();
  NABuffer* buffer = naGetStringBufferMutable(&string);
  
  iterin = naMakeBufferIteratorAccessor(bitarray);
  iterout = naMakeBufferIteratorModifier(buffer);
  
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



NAString naMakeStringAscWithBitArray(const NABuffer* bitarray){
  NAString string;
  if(naIsBufferEmpty(bitarray)){
    string = naMakeString();
  }else{
    #ifndef NDEBUG
      if(naGetBufferRange(bitarray).length % 8)
        naError("naMakeStringAscWithBitArray", "buffer length not divisible by 8");
    #endif
    NAInt arraysize = naGetBufferRange(bitarray).length / 8;
    NAUTF8Char* stringbuffer = naMalloc(arraysize); 
    NAUTF8Char* curchar = stringbuffer;
    NAInt bitcount = 0;
    NAUTF8Char newchar = 0;
    
    NABufferIterator iter = naMakeBufferIteratorAccessor(bitarray);
    while(naIterateBuffer(&iter, -1)){
      NAByte curbit = naGetBufferu8(&iter);
      newchar = newchar << 1 | curbit;
      
      bitcount++;
      if((bitcount % 8) == 0){
        if((newchar < 32) || (newchar > 126)){newchar = '?';}
        *curchar++ = newchar;
      }
    }
    naClearBufferIterator(&iter);
    string = naMakeStringWithMutableUTF8Buffer(stringbuffer, arraysize, NA_MEMORY_CLEANUP_FREE);
  }
  return string;
}



NABit naComputeBitArrayAddOne(NABuffer* bitarray){
  NABufferIterator iter;
  NABit carry = BIT1;
  
  iter = naMakeBufferIteratorMutator(bitarray);
  
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
  
  NABuffer* bitarray = naCreateBuffer(NA_FALSE);
  dstiter = naMakeBufferIteratorModifier(bitarray);
  srciter1 = naMakeBufferIteratorAccessor(srcarray1);
  srciter2 = naMakeBufferIteratorAccessor(srcarray2);
  
  NAUInt srccount1 = naGetBufferRange(srcarray1).length;
  NAUInt srccount2 = naGetBufferRange(srcarray2).length;

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
  if(naIsBufferEmpty(srcarray)){return naCreateBufferPlain();}
  
  NABuffer* bitarray = naCreateBuffer(NA_FALSE);
  NABuffer* bitarray2 = naCreateBufferExtraction(srcarray, naMakeRangei(2, naGetBufferRange(srcarray).length - 2));
  NABuffer* addbuffer = naCreateBitArrayAddBitArray(srcarray, bitarray2);
  
  NABufferIterator iter = naMakeBufferIteratorModifier(bitarray);
  NABufferIterator iter2 = naMakeBufferIteratorAccessor(srcarray);
  
  naWriteBufferu8(&iter, 0);
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferu8(&iter, naReadBufferu8(&iter2));
  naWriteBufferBuffer(&iter, addbuffer, naGetBufferRange(addbuffer));
  naReleaseBuffer(addbuffer);
  naReleaseBuffer(bitarray2);
  
  naClearBufferIterator(&iter2);
  naClearBufferIterator(&iter);
  
  return bitarray;
}



void naComputeBitArrayOnesComplement(NABuffer* bitarray){
  NABufferIterator iter = naMakeBufferIteratorMutator(bitarray);
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
      naError("naComputeBitArraySwapBytes", "size of bitarray can not be divided by 8.");
  #endif

  naCacheBufferRange(bitarray, naGetBufferRange(bitarray), NA_FALSE);
  iter1 = naMakeBufferIteratorMutator(bitarray);
  iter2 = naMakeBufferIteratorMutator(bitarray);
  naIterateBuffer(&iter1, 1);
  naIterateBuffer(&iter2, -8);
  
  while(naTellBuffer(&iter1) < naTellBuffer(&iter2)){
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



// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.