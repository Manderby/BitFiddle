

#ifndef BIT_ARRAY_INCLUDED
#define BIT_ARRAY_INCLUDED

#include "NAUtility/NAString.h"
#include "NAStruct/NABuffer.h"


// A Bit is stored as an NAByte. Sounds strange but that's how NALib does it.
typedef NAByte NABit;
typedef uint32 NANibble;  // A nibble consists of 4 Bits. Here in NALib, they
                          // are stored little-endiane'd in an uint32

#define BIT0 0x00
#define BIT1 0x01

#if NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG
  #define NIBBLE_0 0x00000000
  #define NIBBLE_1 0x01000000
  #define NIBBLE_2 0x00010000
  #define NIBBLE_3 0x01010000
  #define NIBBLE_4 0x00000100
  #define NIBBLE_5 0x01000100
  #define NIBBLE_6 0x00010100
  #define NIBBLE_7 0x01010100
  #define NIBBLE_8 0x00000001
  #define NIBBLE_9 0x01000001
  #define NIBBLE_A 0x00010001
  #define NIBBLE_B 0x01010001
  #define NIBBLE_C 0x00000101
  #define NIBBLE_D 0x01000101
  #define NIBBLE_E 0x00010101
  #define NIBBLE_F 0x01010101
#elif NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_LITTLE
  #define NIBBLE_0 0x00000000
  #define NIBBLE_1 0x00000001
  #define NIBBLE_2 0x00000100
  #define NIBBLE_3 0x00000101
  #define NIBBLE_4 0x00010000
  #define NIBBLE_5 0x00010001
  #define NIBBLE_6 0x00010100
  #define NIBBLE_7 0x00010101
  #define NIBBLE_8 0x01000000
  #define NIBBLE_9 0x01000001
  #define NIBBLE_A 0x01000100
  #define NIBBLE_B 0x01000101
  #define NIBBLE_C 0x01010000
  #define NIBBLE_D 0x01010001
  #define NIBBLE_E 0x01010100
  #define NIBBLE_F 0x01010101
#endif




// Creates a new BitArray which contains copied contents of srcArray with
// the given size. If size is positive, the higher end bits will be filled with
// binary zero if available. If size is negative, the full size of srcArray
// will be used but it will be padded to the next higher bitalignment defined
// by -size.
NABuffer* bitCreateBitArrayCopyWithFixedSize(NABuffer* srcArray, NAInt size);


// Adds binary zero at the higher endian bits so that the total size of the
// bit array is divisible by padSize.
void bitPadBitArray(NABuffer* bitArray, NAInt padSize);


// Creates a new bit array out of a shift and/or extension of another bit array
// by copying the appropriate values
//
// The shift denotes how the bits of the source array are shifted (in natural
// bit order). 0 Means no shift, positive values mean a shift-left operation
// (multiplication by 2) and negative values mean a shift-right operation
// (division by 2). Shifting left will always add zeroes and shifting right
// will discard the least significant bits.
//
// The size denotes the final size of dstarray. If size is smaller than the
// size of srcArray, the most significant bits are discarded. If it is bigger
// than src array, the most significant bits are filled with zeros. If size
// is negative, the most significant bits are filled arithmetically (1 if the
// most significant bit of srcArray was 1, zero otherwise).
//
// Examples:
// 10110101 shift  0 size   8 ->   10110101
//          shift  0 size  10 -> 0010110101
//          shift  0 size -10 -> 1110110101
//          shift  0 size   4 ->       0101
//          shift  2 size   8 ->   11010100
//          shift -2 size   8 ->   00101101
//          shift -2 size  -8 ->   11101101
//NABuffer* bitInitBitArrayShiftExtension(NABuffer* dstarray,
//                                                   NABuffer* srcArray,
//                                                         NAInt shift,
//                                                         NAInt size);

// Creates a bit array from the given string. The string can contain binary
// values, decimal values, hexadecimal values. The Ascii-function will create
// a bit representation of the Ascii characters.
// - If sizehint is positive, it denotes the precise number of bits the
//   resulting bit array shall contain.
// - If sizehint is zero, the size of the bit array is automatically set to
//   the number of bits needed to store all bits of the given string. May
//   result in an empty bit array.
// - If sizehint is negative, the full string is stored just as if sizehint
//   was zero. But the number of bits is dividable by the absolute value of
//   sizehint. Or to say it differently, the bit array is aligned.
//   For example, if sizehint is -8, the resulting bit array contains 8, 16,
//   24, 32, ... bits, but never something in between.
// Note that strings are always expected to be in natural reading order,
// meaning, the ending of the string contains the least significant values.
NABuffer* bitCreateBitArrayWithBinString(NAString* string);
NABuffer* bitCreateBitArrayWithDecString(NAString* string);
NABuffer* bitCreateBitArrayWithHexString(NAString* string);
NABuffer* bitCreateBitArrayWithAscString(NAString* string);

void bitClearBitArray(NABuffer* array);


// Creates string representations of the given bitArray as an integer number.
//
// Creates a byte array out of the given bit array. Every 8 bits are combined
// into one single byte. The byte array is guaranteed to be null-terminated.
// Careful: Only works properly when bitCount can be divided by 8! Use
// bitInitBitArrayShiftExtension first, if this is not the case.
NAString* bitNewStringDecWithBitArray(const NABuffer* bitArray);
NAString* bitNewStringHexWithBitArray(NABuffer* bitArray);
NAString* bitNewStringBinWithBitArray(NABuffer* bitArray);
NAString* bitNewStringAscWithBitArray(NABuffer* bitArray);


// //////////////////////////////////
// Arithmetic functions
//
// The following functions take the bits of one or more source arrays and will
// perform arithmetic operations with them and finally store the result in
// a destination bit array.
//
// - These are not creation functions! All pointers, including the destination
//   array must point to an initialized BitArray!
// - Some array pointers may be equal, but not all!
// - The number of bits considered for the computation is limited by the number
//   of bits allocated in the fullstorage of the dest array. The result may have
//   less bits, but never more.
// - All operations will be performed as UNSIGNED integers. Be careful: This can
//   lead to false computations when trying to add signed integers of different
//   bit sizes! Extend the smaller arrays using bitInitBitArrayShiftExtension first.


// Adds two bit arrays and puts it into the storage of dstarray. All pointers
// can be equal.
NABuffer* bitCreateBitArrayAddBitArray(
  NABuffer* srcArray1,
  NABuffer* srcArray2);

// Multiplies the integer value of srcArray by 10 (Ten). The src pointer MUST
// be different than the dst pointer!
NABuffer* bitCreateBitArrayMulTen(NABuffer* srcArray);



// //////////////////////////////////
// Bit-Fiddling functions
// The following functions alter the bits on an existing array while leaving
// its size and memory position unchanged.

// Adds the integer value 1 to the bit array. A Wrap-around may occur.
// Returns the carry.
NABit bitComputeBitArrayAddOne(NABuffer* array);

// Computes the one's and two's complement of the given array.
void bitComputeBitArrayOnesComplement(NABuffer* array);
void bitComputeBitArrayTwosComplement(NABuffer* array);

// Swaps the byte order. Careful: Only works properly when bitCount can be
// divided by 8! Use bitInitBitArrayShiftExtension first, if this is not the case.
void bitComputeBitArraySwapBytes(NABuffer* array);





#endif // BIT_ARRAY_INCLUDED



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
