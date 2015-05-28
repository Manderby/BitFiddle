

#ifndef BIT_ARRAY_INCLUDED
#define BIT_ARRAY_INCLUDED


#include "NAString.h"


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


// An BitArray stores two arrays: A full bits storage and a subarray
// thereof referencing the full storage.
//
// The bits field is always a subarray of fullstorage. While the bitarray
// visible to the programmer is denoted by the bits field, fullstorage may
// have allocated more bits ready to be used when an operation needs to be
// performed.


// Opaque type. See explanation in readme.txt
typedef struct BitArray BitArray;
struct BitArray{
  struct NAByteArray fullstorage;
  struct NAByteArray bits;
};


// Creates an empty bit array.
BitArray* naCreateBitArray(BitArray* bitarray);


// Creates a bit array with the specified count.
// - If count is positive, the bit storage has precisely the desired size.
// - If count is negative, enough space will be allocated to hold the absolute
//   number of count as bits. But the returned array will be empty to begin
//   with. If you later perform manipulations on this bitarray which need more
//   space, the previously allocated space will be used.
//   Choose the negative count appropriate to the expected needs. If for
//   example, you expect an add-operation upon the bit array, provide 1 Bit
//   more than needed.
// - If count is zero, an empty bit array is returned.
//
// The returned bit array is always unititialized.
BitArray* naCreateBitArrayWithCount( BitArray* bitarray,
                                                    NAInt count);


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
// size of srcarray, the most significant bits are discarded. If it is bigger
// than src array, the most significant bits are filled with zeros. If size
// is negative, the most significant bits are filled arithmetically (1 if the
// most significant bit of srcarray was 1, zero otherwise).
//
// Examples:
// 10110101 shift  0 size   8 ->   10110101
//          shift  0 size  10 -> 0010110101
//          shift  0 size -10 -> 1110110101
//          shift  0 size   4 ->       0101
//          shift  2 size   8 ->   11010100
//          shift -2 size   8 ->   00101101
//          shift -2 size  -8 ->   11101101
BitArray * naCreateBitArrayShiftExtension(BitArray* dstarray,
                                                   BitArray* srcarray,
                                                         NAInt shift,
                                                         NAInt size);

// Will create an extraction of the given srcarray by referencing the values.
// This function will not copy any storage. Use naDecoupleBitArray for that.
// The resulting bit array will dereference the fullstorage in the given
// range and set the bits array to that very same range. Meaning: The new
// bit array will not overwrite the original values even after operations.
BitArray* naCreateBitArrayExtraction(BitArray* dstarray,
                                              BitArray* srcarray,
                                                    NAInt offset,
                                                    NAInt size);

// Creates a bit array from the given string. The string can contain binary
// values, decimal values, hexadecimal values. The ASCII-function will create
// a bit representation of the ASCII characters.
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
// meaning, the trailing of the string contains the least significant values.
BitArray* naCreateBitArrayWithBinString(  BitArray* bitarray,
                                                     NAString* string,
                                                         NAInt sizehint);
BitArray* naCreateBitArrayWithDecString(  BitArray* bitarray,
                                                     NAString* string,
                                                         NAInt sizehint);
BitArray* naCreateBitArrayWithHexString(  BitArray* bitarray,
                                                     NAString* string,
                                                         NAInt sizehint);
BitArray* naCreateBitArrayWithByteArray(  BitArray* bitarray,
                                                  NAByteArray* bytearray,
                                                         NAInt sizehint);

void naClearBitArray(BitArray* array);
void naDestroyBitArray(BitArray* array);

// Will decouple the given array from its current storage by creating its own
// storage with the precise size of the bits field. COPIES ALWAYS!
void naDecoupleBitArray(BitArray* array);



// Returns the bit with the requested index. If index is negative, it
// is searched from the end of the array. For example -1 denotes the last bit.
// Warning: returns garbage if the array is empty.
NABit* naGetBitArrayBit(BitArray* bitarray, NAInt indx);

// Returns the size of the bits array. Not the size of the full storage!
NAUInt naGetBitArrayCount(BitArray* bitarray);


// Creates string representations of the given bitarray as an integer number.
NAString* naCreateStringDecFromBitArray(NAString* string,
                                             BitArray* bitarray);
NAString* naCreateStringHexFromBitArray(NAString* string,
                                             BitArray* bitarray);
NAString* naCreateStringBinFromBitArray(NAString* string,
                                             BitArray* bitarray);

// Creates a byte array out of the given bit array. Every 8 bits are combined
// into one single byte. The byte array is guaranteed to be null-terminated.
// Careful: Only works properly when bitcount can be divided by 8! Use
// naCreateBitArrayShiftExtension first, if this is not the case.
NAByteArray* naCreateByteArrayFromBitArray(NAByteArray* bytearray,
                                                   BitArray* bitarray);


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
//   bit sizes! Extend the smaller arrays using naCreateBitArrayShiftExtension first.


// Adds two bit arrays and puts it into the storage of dstarray. All pointers
// can be equal.
NABit naComputeBitArrayAddBitArray( BitArray* dstarray,
                                           BitArray* srcarray1,
                                           BitArray* srcarray2);

// Multiplies the integer value of srcarray by 10 (Ten). The src pointer MUST
// be different than the dst pointer!
void naComputeBitArrayMulTen(BitArray* dstarray, BitArray* srcarray);



// //////////////////////////////////
// Bit-Fiddling functions
// The following functions alter the bits on an existing array while leaving
// its size and memory position unchanged.

// Adds the integer value 1 to the bit array. A Wrap-around may occur.
// Returns the carry.
NABit naComputeBitArrayAddOne(BitArray* array);

// Computes the one's and two's complement of the given array.
void naComputeBitArrayOnesComplement(BitArray* array);
void naComputeBitArrayTwosComplement(BitArray* array);

// Swaps the byte order. Careful: Only works properly when bitcount can be
// divided by 8! Use naCreateBitArrayShiftExtension first, if this is not the case.
void naComputeBitArraySwapBytes(BitArray* array);



#endif // BIT_ARRAY_INCLUDED

