
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NABinaryData.h"
#include "BitArray.h"
#include "NAString.h"




BitArray* naCreateBitArray(BitArray* bitarray){
  bitarray = naAllocNALibStruct(bitarray, BitArray);
  // Create two empty byte arrays
  naCreateByteArray(&(bitarray->fullstorage));
  naCreateByteArray(&(bitarray->bits));
  return bitarray;
}



BitArray* naCreateBitArrayWithCount(BitArray* bitarray, NAInt count){
  if(!count){return naCreateBitArray(bitarray);}
  bitarray = naAllocNALibStruct(bitarray, BitArray);
  if(count > 0){
    naCreateByteArrayWithSize(&(bitarray->fullstorage), count);
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, -1);
  }else{
    naCreateByteArrayWithSize(&(bitarray->fullstorage), -count);
    naCreateByteArray(&(bitarray->bits));
  }
  return bitarray;
}



BitArray* naCreateBitArrayShiftExtension( BitArray* dstarray, BitArray* srcarray, NAInt shift, NAInt size){
//  NABool arithmetic = NA_FALSE;
  NAInt i = 0;
  NABit* srcptr;
  NABit* dstptr;
  NABit trailbit;
  NAInt srccount;

  #ifndef NDEBUG
    if(!srcarray)
      naError("naCreateBitArrayShiftExtension", "srcarray is Null-Pointer");
  #endif
  if(!size){return naCreateBitArray(dstarray);}
  if(size<0){/*arithmetic = NA_TRUE; */size = -size;}
  dstarray = naCreateBitArrayWithCount(dstarray, size);
  dstptr = naGetBitArrayBit(dstarray, 0);

  srccount = naGetBitArrayCount(srcarray);
  if(srccount == 0){
    naNulln(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
    return dstarray;
  }

  if(shift < 0){
    // Shift right
    if(-shift >= srccount){
      naNulln(naGetByteArrayMutablePointer(&(dstarray->fullstorage)), size);
      return dstarray;
    }
    srcptr = naGetBitArrayBit(srcarray, -shift);
  }else{
    // Shift left
    while((i < shift) && (i < size)){
      *dstptr++ = BIT0;
      i++;
    }
    srcptr = naGetBitArrayBit(srcarray, 0);
  }

  // Set the bits of the source
  while((i < (srccount + shift)) && (i < size)){
    *dstptr++ = *srcptr++;
    i++;
  }
  // Set the trailing bits (big endian)
  trailbit = *naGetBitArrayBit(srcarray, -1);
  while(i<size){
    *dstptr++ = trailbit;
    i++;
  }

  return dstarray;
}



BitArray* naCreateBitArrayExtraction( BitArray* dstarray,
                          BitArray* srcarray,
                          NAInt offset,
                          NAInt size){
  dstarray = naAllocNALibStruct(dstarray, BitArray);
  // An extractions fullstorage corresponds to the source bits not the source
  // fullstorage. This is for safety reasons such that an extracted array can
  // not change bits of its originated array outside of the extracted area.
  naCreateByteArrayExtraction(&(dstarray->fullstorage), &(srcarray->bits), offset, size);
  // Set the bits field to the whole fullstorage.
  naCreateByteArrayExtraction(&(dstarray->bits), &(dstarray->fullstorage), 0, -1);
  return dstarray;
}


// Helper function.
// Returns the needed size of the array given the count parameter of the
// createBitArrayFromXXXString functions and the expected count to store the
// full bit array.
NAInt getBitArraySizeHintCount(NAInt sizehint, NAInt desiredcount){
  if(sizehint > 0){
    // When sizehint is positive, sizehint shall be used.
    return sizehint;
  }else if(sizehint == 0){
    // If sizehint is 0, all bites need to be stored.
    return desiredcount;
  }else{
    // When sizehint is negative, it denotes an alignment. Sufficient bits must
    // be allocated to store all bits including potential padding bits. Will
    // never return zero.
    NAInt retcount = desiredcount - (sizehint + (desiredcount % sizehint)) % sizehint;
    if(!retcount){retcount = -sizehint;}
    return retcount;
  }
}


// Helper function.
// Looks at the given sizehint and expands or extracts the given bitarray and
// fills the padding bits with zero if necessary. Warning: expects fullstorage
// to have enough bits allocated.
void naEnsureBitArraySizeHint(BitArray* bitarray, NAInt sizehint){
  NAUInt bitcount = naGetByteArraySize(&(bitarray->bits));
  NAInt arraycount = getBitArraySizeHintCount(sizehint, bitcount);

  NAInt sizediff = arraycount - bitcount;
  if(!bitcount){
    // if the bitarray was completely empty, just fill it with zeros.
    naNulln(naGetByteArrayMutablePointer(&(bitarray->fullstorage)), sizediff);
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
  }else if(sizediff > 0){
    // The bitarray has fewer bits than needed. Fill with zero.
    naNulln(naGetByteArrayMutableByte(&(bitarray->bits), -1) + 1, sizediff);
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
  }else if(sizediff < 0){
    // The bit array has more bits stored than needed. Extract the relevant
    // part.
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->bits), 0, arraycount);
  }else{
    // The Bit array already has the precise size needed. Do nothing.
  }
}



BitArray* naCreateBitArrayWithBinString(BitArray* bitarray,
                                            NAString* string,
                                                NAInt sizehint){
  NAUInt stringsize;
  NAInt arraycount;
  const NAUTF8Char* curchar;
  NABit* curbit;
  NAInt bitcount;
  NAUInt i;

  bitarray = naAllocNALibStruct(bitarray, BitArray);
  // We assume that the string does not contain many garbage characters, so
  // we just take the string size as our guess how long the bin array will be.
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // todo: Add caching
  // Fill the bit array from lsb to msb.
  curchar = naGetStringChar(string, -1);
  curbit = naGetByteArrayMutablePointer(&(bitarray->fullstorage));
  bitcount = 0;
  curchar++;
  for(i = 0; i < stringsize; i++){
    curchar--;
    switch (*curchar){
    case '0':           *curbit = BIT0; break;
    case '1':           *curbit = BIT1; break;
    case 'o':           *curbit = BIT0; break;
    case 'i':           *curbit = BIT1; break;
    case 'O':           *curbit = BIT0; break;
    case 'I':           *curbit = BIT1; break;
    case 'L':           *curbit = BIT1; break;
    default: continue;
    }
    curbit++;
    bitcount++;
    if(bitcount >= arraycount){break;}  // stop if count is reached.
  }
  if(bitcount){
    // Reaching here, the bits array is empty and the clear call is somewhat
    // unneccessary. But we should be consistend anyhow. Just think about what
    // would happen if one day, the implementation of ByteArray changes...
    naClearByteArray(&(bitarray->bits));
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, bitcount);
  }
  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}



BitArray* naCreateBitArrayWithDecString(BitArray* bitarray,
                                            NAString* string,
                                                NAInt sizehint){
  NAUInt stringsize;
  NAInt arraycount;
  const NAUTF8Char* curchar;
  BitArray nibble;
  NANibble* nibbleptr;
  BitArray tenarray;
  NAUInt i;

  bitarray = naAllocNALibStruct(bitarray, BitArray);
  // We assume that the string does not contain many garbage characters, so
  // we just take the string size times 4 as our guess how long the bin array
  // will be.
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize * 4);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // todo: Add caching
  // Fill the bit array from lsb to msb by parsing the string from msb to lsb.
  curchar = naGetStringChar(string, 0);
  
  naCreateBitArrayWithCount(&nibble, 4); // todo: use existing storage.
  nibbleptr = (NANibble*)naGetBitArrayBit(&nibble, 0);
  naCreateBitArrayWithCount(&tenarray, stringsize * -4);
  
  curchar--;
  for(i = 0; i < stringsize; i++){
    curchar++;
    switch (*curchar){
    case '0': *nibbleptr = NIBBLE_0; break;
    case '1': *nibbleptr = NIBBLE_1; break;
    case '2': *nibbleptr = NIBBLE_2; break;
    case '3': *nibbleptr = NIBBLE_3; break;
    case '4': *nibbleptr = NIBBLE_4; break;
    case '5': *nibbleptr = NIBBLE_5; break;
    case '6': *nibbleptr = NIBBLE_6; break;
    case '7': *nibbleptr = NIBBLE_7; break;
    case '8': *nibbleptr = NIBBLE_8; break;
    case '9': *nibbleptr = NIBBLE_9; break;
    default: continue;
    }
    naComputeBitArrayMulTen(&tenarray, bitarray);
    naComputeBitArrayAddBitArray(bitarray, &tenarray, &nibble);
    // Note that you have to parse the whole string before having determined
    // the least significant bits.
  }
  naClearBitArray(&nibble);
  naClearBitArray(&tenarray);
  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}


BitArray* naCreateBitArrayWithHexString(BitArray* bitarray,
                                            NAString* string,
                                                NAInt sizehint){
  NAUInt stringsize;
  NAInt arraycount;
  const NAUTF8Char* curchar;
  NABit* startbit;
  uint32* curnibble;
  NAInt bitcount;
  NAUInt i;

  bitarray = naAllocNALibStruct(bitarray, BitArray);
  // We assume that the string does not contain many garbage characters, so
  // we just take the string size as our guess how long the bin array will be.
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize * 4);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // Fill the bit array from lsb to msb.
  curchar = naGetStringChar(string, -1);
  startbit = naGetByteArrayMutablePointer(&(bitarray->fullstorage));
  curnibble = (uint32*)(startbit);
  bitcount = 0;
  curchar++;
  for(i = 0; i < stringsize; i++){
    curchar--;
    switch (*curchar){
    case '0':           *curnibble++ = NIBBLE_0; break;
    case '1':           *curnibble++ = NIBBLE_1; break;
    case '2':           *curnibble++ = NIBBLE_2; break;
    case '3':           *curnibble++ = NIBBLE_3; break;
    case '4':           *curnibble++ = NIBBLE_4; break;
    case '5':           *curnibble++ = NIBBLE_5; break;
    case '6':           *curnibble++ = NIBBLE_6; break;
    case '7':           *curnibble++ = NIBBLE_7; break;
    case '8':           *curnibble++ = NIBBLE_8; break;
    case '9':           *curnibble++ = NIBBLE_9; break;
    case 'a': case 'A': *curnibble++ = NIBBLE_A; break;
    case 'b': case 'B': *curnibble++ = NIBBLE_B; break;
    case 'c': case 'C': *curnibble++ = NIBBLE_C; break;
    case 'd': case 'D': *curnibble++ = NIBBLE_D; break;
    case 'e': case 'E': *curnibble++ = NIBBLE_E; break;
    case 'f': case 'F': *curnibble++ = NIBBLE_F; break;
    default: continue;
    }
    bitcount+=4;
    if(bitcount >= arraycount){break;}
  }
  if(bitcount){
    // Reaching here, the bits array is empty and the clear call is somewhat
    // unneccessary. But we should be consistend anyhow. Just think about what
    // would happen if one day, the implementation of ByteArray changes...
    naClearByteArray(&(bitarray->bits));
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, bitcount);
  }
  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}




BitArray* naCreateBitArrayWithByteArray(BitArray* bitarray, NAByteArray* bytearray, NAInt sizehint){
  NAUInt bytearraysize;
  NAInt arraycount;
  NAInt bitcount = 0;
  const NAByte* curbyteptr;
  NABit* curbit;
  NAUInt i;

  bitarray = naAllocNALibStruct(bitarray, BitArray);
  bytearraysize = naGetByteArraySize(bytearray);
  arraycount = getBitArraySizeHintCount(sizehint, bytearraysize * 8);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!bytearraysize){return bitarray;}

  // todo: caching.
  curbyteptr = naGetByteArrayConstByte(bytearray, -1);
  curbit = naGetByteArrayMutablePointer(&(bitarray->fullstorage));
  for(i = 0; i < bytearraysize; i++){
    NAUTF8Char curchar = *curbyteptr;
    *curbit++ = (curchar & 0x01) >> 0;
    *curbit++ = (curchar & 0x02) >> 1;
    *curbit++ = (curchar & 0x04) >> 2;
    *curbit++ = (curchar & 0x08) >> 3;
    *curbit++ = (curchar & 0x10) >> 4;
    *curbit++ = (curchar & 0x20) >> 5;
    *curbit++ = (curchar & 0x40) >> 6;
    *curbit++ = (curchar & 0x80) >> 7;
    curbyteptr--;
    bitcount += 8;
  }
  
  if(bitcount){
    naClearByteArray(&(bitarray->bits));
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, bitcount);
  }

  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}


void naClearBitArray(BitArray* array){
  naClearByteArray(&(array->bits));
  naClearByteArray(&(array->fullstorage));
}


void naDestroyBitArray(BitArray* array){
  naClearBitArray(array);
  free(array);
}


void naDecoupleBitArray(BitArray* bitarray){
  if(naIsByteArrayEmpty(&(bitarray->bits))){
    // This array points to an empty bit array. Make the fullstorage empty
    // as well to be consistent.
    naClearByteArray(&(bitarray->fullstorage));
    naCreateByteArray(&(bitarray->fullstorage));
    return;
  }
  naClearByteArray(&(bitarray->fullstorage));
  naCreateByteArrayExtraction(&(bitarray->fullstorage), &(bitarray->bits), 0, -1);
  naDecoupleByteArray(&(bitarray->fullstorage), NA_FALSE);
  naClearByteArray(&(bitarray->bits));
  naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, -1);
}







NABit* naGetBitArrayBit(BitArray* bitarray, NAInt indx){
  #ifndef NDEBUG
    if(!bitarray){
      naCrash("naGetBitArrayBit", "bitarray is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteArrayMutableByte(&(bitarray->bits), indx);
}

NAUInt naGetBitArrayCount(BitArray* bitarray){
  #ifndef NDEBUG
    if(!bitarray){
      naCrash("naGetBitArrayCount", "bitarray is Null-Pointer.");
      return 0;
    }
  #endif
  return naGetByteArraySize(&(bitarray->bits));
}











NAString* naNewStringDecFromBitArray(BitArray* bitarray){
  NAUTF8Char* charptr;
  NAInt outputlen;
  NAInt finalstringcount;
  BitArray* work;
  NAUInt i;
  NAUInt j;
  NAString* string;
  NAString* retstring;

  NAUInt bitcount = naGetBitArrayCount(bitarray);
  if(!bitcount){return naNewString();}
  
  NAUTF8Char* stringbuf = naAllocate(-bitcount);
  charptr = &(stringbuf[bitcount-1]);
  string = naNewStringWithMutableUTF8Buffer(stringbuf, -bitcount, NA_TRUE);

  outputlen = 0;
  finalstringcount = 0;
  work = naCreateBitArrayExtraction(NULL, bitarray, 0, -1);
  naDecoupleBitArray(work);

  while(bitcount){
    // as long as a remaining value exists
    NABit lead = BIT0;
    NABit bit0;
    NABit bit1;
    NABit bit2;
    NABit bit3;
    uint32 value;
    i = 0;
    j = naGetBitArrayCount(bitarray) - 1;
    while(j >= (bitarray->bits.size - bitcount) + 3){
      // walk through the remaining value
      bit0 = naGetByteArrayConstPointer(&(work->bits))[j-3];
      bit1 = naGetByteArrayConstPointer(&(work->bits))[j-2];
      bit2 = naGetByteArrayConstPointer(&(work->bits))[j-1];
      bit3 = naGetByteArrayConstPointer(&(work->bits))[j-0];
      value = (bit3<<24) | (bit2<<16) | (bit1<<8) | (bit0<<0);
      if(value >= NIBBLE_A){
        // For nibbles greaterequal than the value 10, adjust the bits accordingly.
        naGetByteArrayMutablePointer(&(work->bits))[j-0] = BIT1;  // division result
        naGetByteArrayMutablePointer(&(work->bits))[j-1] = bit2 && bit1;
        naGetByteArrayMutablePointer(&(work->bits))[j-2] = !bit1;
      }else{
        naGetByteArrayMutablePointer(&(work->bits))[j-0] = lead;
        if(lead){
          // When the previous nibble was 8 or 9, adjust the bits accordingly
          naGetByteArrayMutablePointer(&(work->bits))[j-1] = !bit1;
          naGetByteArrayMutablePointer(&(work->bits))[j-2] = !bit1;
          lead = bit1;
        }else{
          // otherwise, just leave the bits as they are.
          if(value >= NIBBLE_8){lead = BIT1;}
        }
      }
      i++;
      j--;
    }
    // extract the decimal value of the remaining bits
    if(bitcount==1){
      bit0 = naGetByteArrayConstPointer(&(work->bits))[bitarray->bits.size - 1];
      bit1 = BIT0;
      bit2 = BIT0;
    }else if(bitcount==2){
      bit0 = naGetByteArrayConstPointer(&(work->bits))[bitarray->bits.size - 2];
      bit1 = naGetByteArrayConstPointer(&(work->bits))[bitarray->bits.size - 1];
      bit2 = BIT0;
    }else{
      bit0 = naGetByteArrayConstPointer(&(work->bits))[j-2];
      bit1 = naGetByteArrayConstPointer(&(work->bits))[j-1];
      bit2 = naGetByteArrayConstPointer(&(work->bits))[j-0];
    }
    bit3 = lead;
    value = (bit3<<3) | (bit2<<2) | (bit1<<1) | (bit0<<0);
    if((outputlen > 0) && !(outputlen%3)){*charptr-- = ' '; finalstringcount++;}
    *charptr-- = value + '0';
    outputlen++;
    finalstringcount++;
    bitcount = i;
  }
  // Remove zeros
  charptr++;
  while((finalstringcount) && ((*charptr == '0') || (*charptr == ' '))){
    charptr++;
    finalstringcount--;
  }
  
  if(finalstringcount){
    naDestroyBitArray(work);
    retstring = naNewStringExtraction(string, -finalstringcount, -1);
    
  }else{
    retstring = naNewStringWithUTF8CStringLiteral("0");
  }
  naDelete(string);
  return retstring;
}



NAString* naNewStringHexFromBitArray(BitArray* bitarray){
  NAUTF8Char* charptr;
  NABit* bitptr;
  uint8 nibble;

  NAUInt bitcount = naGetBitArrayCount(bitarray);
  NAInt nibblecount = (bitcount % 4)?(bitcount / 4 + 1):(bitcount / 4);
  NAInt delimiters = (nibblecount - 1) / 2;
  if(!nibblecount){return naNewString();}

  NAUTF8Char* stringbuf = naAllocate(-(nibblecount + delimiters));
  charptr = stringbuf;

  bitptr = naGetBitArrayBit(bitarray, -1);
  nibble = 0;
  while(bitcount){
    nibble <<= 1;
    nibble |= *bitptr;
    bitptr--;
    bitcount--;
    if(!(bitcount % 4)){
      if(nibble < 10){
        *charptr++ = '0' + nibble;
      }else{
        *charptr++ = 'a' + nibble - 10;
      }
      nibble = 0;
      if(bitcount && !(bitcount % 8)){
        *charptr++ = ' ';
      }
    }
  }
  return naNewStringWithMutableUTF8Buffer(stringbuf, -(nibblecount + delimiters), NA_TRUE);
}



NAString* naNewStringBinFromBitArray(BitArray* bitarray){
  NAUTF8Char* charptr;
  NABit* bitptr;

  NAUInt bitcount = naGetBitArrayCount(bitarray);
  NAInt delimiters = (bitcount - 1) / 8;
  if(!bitcount){return naNewString();}

  NAUTF8Char* stringbuf = naAllocate(-(bitcount + delimiters));
  charptr = stringbuf;

  bitptr = naGetBitArrayBit(bitarray, -1);
  while(bitcount){
    *charptr++ = '0' + *bitptr--;
    bitcount--;
    if(bitcount && !(bitcount % 8)){
      *charptr++ = ' ';
    }
  }
  return naNewStringWithMutableUTF8Buffer(stringbuf, -(bitcount + delimiters), NA_TRUE);
}



NAByteArray* naCreateByteArrayFromBitArray(NAByteArray* bytearray,
                                            BitArray* bitarray){
  NAByte* curbyte;
  NABit* curbit;
  int b;
  NAUInt bytecount = naGetBitArrayCount(bitarray) / 8;
  bytearray = naCreateByteArrayWithSize(bytearray, -bytecount);
  if(!bytecount){return bytearray;}
  
  curbyte = naGetByteArrayMutablePointer(bytearray);
  curbit = naGetBitArrayBit(bitarray, (bytecount - 1) * 8) + 7;

  while(bytecount){
    *curbyte = 0;
    
    for(b=0; b<8; b++){
      *curbyte <<= 1;
      *curbyte |= *curbit;
      curbit--;
    }
    
    curbyte++;
    bytecount--;
  }

  return bytearray;
}




NABit naComputeBitArrayAddOne(BitArray* array){
  NABit carry;
  NABit* curbit;

  NAUInt bitcount = naGetBitArrayCount(array);
  if(!bitcount){return BIT1;}

  curbit = naGetByteArrayMutablePointer(&(array->bits));

  carry = BIT1;
  while(carry && bitcount){
    carry = *curbit;
    *curbit ^= BIT1;
    bitcount--;
    curbit++;
  }
  return carry;
}



NABit naComputeBitArrayAddBitArray( BitArray* dstarray,
                                    BitArray* srcarray1,
                                    BitArray* srcarray2){
  NABit* curbitd;
  const NABit* curbit1 = NA_NULL;
  const NABit* curbit2 = NA_NULL;
  NABit carry;  // Note that carry can be 0, 1, 2 or 3 during the computation
                // but will be 0 or 1 in the end.

  NAUInt dstcount = naGetByteArraySize(&(dstarray->fullstorage));
  NAUInt srccount1 = naGetByteArraySize(&(srcarray1->bits));
  NAUInt srccount2 = naGetByteArraySize(&(srcarray2->bits));

  if(!dstcount){return BIT0;}

  // The destination bit pointer will point to a bit with lesser significance
  // than the bits of src1 and src2. Therefore, it is save to say that both
  // src1 and src2 can be the same as dst and might therefore be overwritten
  // during this computation without alienating the result.
  curbitd = naGetByteArrayMutablePointer(&(dstarray->fullstorage));
  if(srccount1){curbit1 = naGetByteArrayConstPointer(&(srcarray1->bits));}
  if(srccount2){curbit2 = naGetByteArrayConstPointer(&(srcarray2->bits));}

  carry = 0;
  
  // Add the two arrays as long as there exist elements in both arrays
  while(dstcount && srccount1 && srccount2){
    carry += *curbit1++;
    carry += *curbit2++;
    srccount1--;
    srccount2--;
    *curbitd++ = carry & BIT1;
    carry >>= 1;
    dstcount--;
  }

  // Reaching here, one of the counts became 0. Whoever it was, src1 will
  // now become the one still possibly having valid bits.
  if(srccount2){srccount1 = srccount2; curbit1 = curbit2;}
  
  // Now, add up the remaining bits.
  while(dstcount && srccount1){
    carry += *curbit1++;
    srccount1--;
    *curbitd++ = carry & BIT1;
    carry >>= 1;
    dstcount--;
  }
  
  // Add the remaining carry, if needed.
  if(dstcount && carry){
    *curbitd = carry & BIT1;
    carry >>= 1;
    dstcount--;
  }
  
  naCreateByteArrayExtraction(&(dstarray->bits), &(dstarray->fullstorage), 0, -1 - dstcount);
  return carry;
}



void naComputeBitArrayMulTen(BitArray* dstarray, BitArray* srcarray){
  NABit* curbitd;
  const NABit* curbits;
  NABit carry;
  NABit bit0 = BIT0;
  NABit bit1 = BIT0;
  NABit bit2 = BIT0;

  // todo: only correct if src != dst.
  NAUInt dstcount = naGetByteArraySize(&(dstarray->fullstorage));
  NAUInt srccount = naGetByteArraySize(&(srcarray->bits));

  if(!dstcount){return;}
  if(!srccount){
    naClearByteArray(&(dstarray->bits));
    naCreateByteArray(&(dstarray->bits));
    return;
  }

  curbitd = naGetByteArrayMutablePointer(&(dstarray->fullstorage));
  curbits = naGetByteArrayConstPointer(&(srcarray->bits));

  // Set the first three bits
  if(dstcount){*curbitd++ = BIT0;  dstcount--;}

  if(srccount){bit0 = *curbits++; srccount--;}
  if(dstcount){*curbitd++ = bit0;     dstcount--;}

  if(srccount){bit1 = bit0; bit0 = *curbits++; srccount--;}
  if(dstcount){*curbitd++ = bit0;     dstcount--;}

  // Now go for the main part
  carry = BIT0;
  while(dstcount && srccount){
    bit2 = bit1;
    bit1 = bit0;
    bit0 = *curbits++;
    srccount--;
    
    carry += bit0 + bit2;
    *curbitd++ = carry & BIT1;
    carry >>= 1;
    dstcount--;
  }

  // Add up the remaining bits.
  if(dstcount){
    bit2 = bit1;
    bit1 = bit0;
    bit0 = BIT0;
    
    carry += bit0 + bit2;
    *curbitd++ = carry & BIT1;
    carry >>= 1;
    dstcount--;
  }

  if(dstcount){
    bit2 = bit1;
//    bit1 = bit0;
    bit0 = BIT0;
    
    carry += bit0 + bit2;
    *curbitd++ = carry & BIT1;
    carry >>= 1;
    dstcount--;
  }

  // Add the remaining carry, if needed.
  if(dstcount && carry){
    *curbitd = carry & BIT1;
    dstcount--;
  }

  naCreateByteArrayExtraction(&(dstarray->bits), &(dstarray->fullstorage), 0, -1 - dstcount);
}



void naComputeBitArrayOnesComplement(BitArray* array){
  NABit* curbit;
  NAUInt bitcount = naGetBitArrayCount(array);
  
  if(!bitcount){return;}

  curbit = naGetByteArrayMutablePointer(&(array->bits));
  while(bitcount){
    *curbit ^= BIT1;
    bitcount--;
    curbit++;
  }
}



void naComputeBitArrayTwosComplement(BitArray* array){
  naComputeBitArrayOnesComplement(array);
  naComputeBitArrayAddOne(array);
}


void naComputeBitArraySwapBytes(BitArray* array){
  NABit* srcptr;
  NABit* dstptr;
  #ifndef NDEBUG
    if(!array){
      naCrash("naComputeBitArraySwapBytes", "array is Null-Pointer.");
      return;
    }else{
      if(naGetByteArraySize(&(array->bits)) % 8)
        naError("naComputeBitArraySwapBytes", "size of bitarray can not be divided by 8.");
    }
  #endif

  if(!naGetBitArrayCount(array)){return;}
  srcptr = naGetBitArrayBit(array, -8);
  dstptr = naGetBitArrayBit(array, 0);
  while(srcptr > dstptr){
    naSwap64(srcptr, dstptr);
    srcptr -= 8;
    dstptr += 8;
  }
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
