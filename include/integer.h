#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#pragma once

// Settings --------------------------------------------------

#define KK_INT_TEST_ON_CAST
// #define KK_INT_CONVERT_ON_CAST

// Info ------------------------------------------------------

/*
  BIGINT -> pointer to {
    length: size_t
    data: smallint[]
  }

  SMALLINT -> normal int of pointer size

  VARINT -> int of pointer size
    but bit 61 instead of 63 is used for sign
        bit 62 is used to indicate if it is a bigint
        bit 63 is kept empty to prevent overflows
        (This makes some checking faster)


  If VARINT is a bigint, the lower 62 bits are shifted to the highest 62.
  It now represent the BIGINT pointer.
  For this to work every bigint data must be 4*n aligned

  If VARINT is a smallint, bit 63 and 62 are replaced with the value of bit 61
  It now represent a normal integer
*/

// -----------------------------------------------------------

#ifdef KK_INT_CONVERT_ON_CAST
#define KK_INT_TEST_ON_CAST
#endif

#define KK_POINTER_SIZE __SIZEOF_POINTER__

#define KK_VARINT_SIZE KK_POINTER_SIZE
#define KK_VARINT_BITS (KK_VARINT_SIZE * 8)

#if KK_VARINT_SIZE == 16
typedef int128_t kk_varint_t;
typedef uint128_t kk_varint_data_t;
#define KK_VARINT_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#elif KK_VARINT_SIZE == 8
typedef int64_t kk_varint_t;
typedef uint64_t kk_varint_data_t;
#define KK_VARINT_MAX __INT64_MAX__
#elif KK_VARINT_SIZE == 4
typedef int32_t kk_varint_t;
typedef uint32_t kk_varint_data_t;
#define KK_VARINT_MAX __INT32_MAX__
#elif KK_VARINT_SIZE == 2
typedef int16_t kk_varint_t;
typedef uint16_t kk_varint_data_t;
#define KK_VARINT_MAX __INT16_MAX__
#else
#error "platform must be 16, 32, 64, or 128 bits."
#endif

typedef kk_varint_t kk_smallint_t;

typedef void *kk_bigint_t;
typedef size_t kk_bigint_length_t;
typedef kk_smallint_t *kk_bigint_data_array_t;
typedef uint8_t *kk_bigint_byte_array_t;

#define _KK_SMALLINT_BITS_RESERVED 2
#define _KK_SMALLINT_BITS_ALIGNMENT 4 // 2^_KK_SMALLINT_BITS_RESERVED

#define KK_SMALLINT_SIZE KK_VARINT_SIZE
#define KK_SMALLINT_BITS KK_VARINT_BITS
#define KK_SMALLINT_BITS_DATA (KK_VARINT_BITS - _KK_SMALLINT_BITS_RESERVED)
#define KK_SMALLINT_MAX (KK_VARINT_MAX >> _KK_SMALLINT_BITS_RESERVED)
#define KK_SMALLINT_MIN (-KK_SMALLINT_MAX - 1)

#define KK_VARINT_BIT_LSBF(bit) ((kk_varint_t)1 << (bit))
#define KK_VARINT_BIT_MSBF(bit) ((kk_varint_t)1 << (KK_VARINT_BITS - (bit)-1))
#define KK_VARINT_OVERFLOW_BIT KK_VARINT_BIT_MSBF(0)
#define KK_VARINT_LARGE_BIT KK_VARINT_BIT_MSBF(1)
#define KK_VARINT_SIGN_BIT KK_VARINT_BIT_MSBF(2)
#define KK_VARINT_META_MASK (KK_VARINT_OVERFLOW_BIT + KK_VARINT_LARGE_BIT)
#define KK_VARINT_DATA_MASK (~KK_VARINT_META_MASK)

#define KK_SMALLINT_SIGN_BIT KK_VARINT_BIT_MSBF(0)

#define KK_BIGINT_HEADER_SIZE (sizeof(kk_bigint_length_t))

// Define functions

#define KK_BIGINT_GET_DATA_LENGTH(bigint) (*(kk_bigint_length_t *)(bigint))
#define KK_BIGINT_GET_DATA_ARRAY(bigint) ((kk_bigint_data_array_t)((uint8_t *)bigint + KK_BIGINT_HEADER_SIZE))
#define KK_BIGINT_GET_DATA_LENGTH_BYTES(bigint) (KK_BIGINT_GET_DATA_LENGTH(bigint) * KK_SMALLINT_SIZE)
#define KK_BIGINT_GET_DATA_ARRAY_BYTES(bigint) ((kk_bigint_byte_array_t)((uint8_t *)bigint + KK_BIGINT_HEADER_SIZE))

#define KK_BIGINT_CALC_FULL_SIZE(length) (KK_BIGINT_HEADER_SIZE + (length)*KK_SMALLINT_SIZE)

#define KK_VARINT_IS_BIGINT(varint) ((varint)&KK_VARINT_LARGE_BIT)
#define KK_VARINT_IS_SMALLINT(varint) (!KK_VARINT_IS_BIGINT(varint))
#define KK_SMALLINT_HAS_OVERFLOWED(varint) ((varint) & (KK_VARINT_LARGE_BIT + KK_VARINT_OVERFLOW_BIT))
#define KK_SMALLINT_NOT_OVERFLOWED(varint) (!KK_SMALLINT_HAS_OVERFLOWED(varint))
#define KK_SMALLINT_IS_VALID(smallint) ((smallint) <= KK_SMALLINT_MAX && (smallint) >= KK_SMALLINT_MIN)
#define KK_BIGINT_IS_VALID(bigint) (((kk_varint_data_t)bigint & 0x3) != 0)

// Inline Functions ------------------------------------------

#include "_integer-inlines.h"

// Functions -------------------------------------------------

kk_bigint_t create_kkbigint_parts(size_t parts);
kk_bigint_t create_kkbigint_bits(size_t bits);
kk_varint_t create_kkvarint_from_borrowed_hexstr(char *hexstr);
char *create_hexstr_from_borrowed_kkvarint(kk_varint_t varint);

kk_bigint_length_t kkbigint_get_used_parts(kk_bigint_t bigint);
kk_bigint_t kkbigint_shrink_to_fit(kk_bigint_t bigint);
kk_bigint_t kkbigint_resize(kk_bigint_t bigint, kk_bigint_length_t new_parts);

// Util functions --------------------------------------------

void print_kkbigint_internal(kk_bigint_t bigint);

// Math ------------------------------------------------------

#include "_integer-math.h"
