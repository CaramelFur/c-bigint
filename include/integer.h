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

// Imporant
typedef uint8_t kk_byte;
#define KK_BYTE_SIZE sizeof(kk_byte)

#define KK_BIT_LSBF(type, bit) ((type)1 << (bit))
#define KK_BIT_MSBF(type, bit) ((type)1 << ((sizeof(type) * 8) - (bit)-1))

// Adjust to system bits
#define KK_POINTER_SIZE __SIZEOF_POINTER__

#if KK_POINTER_SIZE == 16
// 128 BIT
typedef int128_t kk_full_int_t;
typedef uint128_t kk_full_uint_t;
typedef int64_t kk_half_int_t;
typedef uint64_t kk_half_uint_t;
#define KK_FULL_INT_MAX 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#define KK_FULL_UINT_MAX 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#define KK_HALF_INT_MAX __INT64_MAX__
#define KK_HALF_UINT_MAX __UINT64_MAX__
#elif KK_POINTER_SIZE == 8
// 64 BIT
typedef int64_t kk_full_int_t;
typedef uint64_t kk_full_uint_t;
typedef int32_t kk_half_int_t;
typedef uint32_t kk_half_uint_t;
#define KK_FULL_INT_MAX __INT64_MAX__
#define KK_FULL_UINT_MAX __UINT64_MAX__
#define KK_HALF_INT_MAX __INT32_MAX__
#define KK_HALF_UINT_MAX __UINT32_MAX__
#elif KK_POINTER_SIZE == 4
// 32 BIT
typedef int32_t kk_full_int_t;
typedef uint32_t kk_full_uint_t;
typedef int16_t kk_half_int_t;
typedef uint16_t kk_half_uint_t;
#define KK_FULL_INT_MAX __INT32_MAX__
#define KK_FULL_UINT_MAX __UINT32_MAX__
#define KK_HALF_INT_MAX __INT16_MAX__
#define KK_HALF_UINT_MAX __UINT16_MAX__
#elif KK_POINTER_SIZE == 2
// 16 BIT
typedef int16_t kk_full_int_t;
typedef uint16_t kk_full_uint_t;
typedef int8_t kk_half_int_t;
typedef uint8_t kk_half_uint_t;
#define KK_FULL_INT_MAX __INT16_MAX__
#define KK_FULL_UINT_MAX __UINT16_MAX__
#define KK_HALF_INT_MAX __INT8_MAX__
#define KK_HALF_UINT_MAX __UINT8_MAX__
#else
#error "platform must be 16, 32, 64, or 128 bits."
#endif

#define KK_FULL_INT_MIN (-KK_FULL_INT_MAX - 1)
#define KK_FULL_UINT_MIN 0
#define KK_HALF_INT_MIN (-KK_HALF_INT_MAX - 1)
#define KK_HALF_UINT_MIN 0

// Varint
typedef kk_full_uint_t kk_vi_t;
#define KK_VI_SIZE (sizeof(kk_vi_t))
#define KK_VI_BITS (KK_VI_SIZE * 8)

#define KK_VI_SIGN_BIT KK_BIT_MSBF(kk_vi_t, 0)
#define KK_VI_LARGE_BIT KK_BIT_MSBF(kk_vi_t, 1)
#define KK_VI_META_MASK (KK_VI_SIGN_BIT + KK_VI_LARGE_BIT)
#define KK_VI_DATA_MASK (~KK_VI_META_MASK)

// Bigint
typedef void *kk_bi_t;
typedef kk_full_uint_t kk_bi_length_t;
typedef kk_full_uint_t kk_bi_fullpart_t;
typedef kk_half_uint_t kk_bi_smallpart_t;
typedef kk_byte kk_bi_bytepart_t;

typedef kk_bi_fullpart_t *kk_bi_fullp_arr_t;
typedef kk_bi_smallpart_t *kk_bi_smallp_arr_t;
typedef kk_bi_bytepart_t *kk_bi_bytep_arr_t;

#define KK_BI_HEADER_SIZE (sizeof(kk_bi_length_t))
#define KK_BI_FULLPART_SIZE (sizeof(kk_bi_fullpart_t))
#define KK_BI_FULLPART_BITS (KK_BI_FULLPART_SIZE * 8)
#define KK_BI_SMALLPART_SIZE (sizeof(kk_bi_smallpart_t))
#define KK_BI_SMALLPART_BITS (KK_BI_SMALLPART_SIZE * 8)
#define KK_BI_BYTEPART_SIZE (sizeof(kk_bi_bytepart_t))
#define KK_BI_BYTEPART_BITS (KK_BI_BYTEPART_SIZE * 8)

// Smallint
typedef kk_full_int_t kk_si_t;
#define KK_SI_SIZE (sizeof(kk_si_t))
#define KK_SI_BITS (KK_SI_SIZE * 8)

#define _KK_SI_BITS_RESERVED 2
#define _KK_SI_BITS_ALIGNMENT 4 // 2^_KK_SI_BITS_RESERVED
#define KK_SI_BITS_DATA (KK_SI_BITS - _KK_SI_BITS_RESERVED)
#define KK_SI_MAX (KK_FULL_INT_MAX >> _KK_SI_BITS_RESERVED)
#define KK_SI_MIN (-KK_SI_MAX - 1)

#define KK_SI_SIGN_BIT KK_BIT_MSBF(kk_si_t, 0)

// Define functions

// Varint
#define KK_VI_IS_BI(varint) (((varint) & KK_VI_META_MASK) == KK_VI_LARGE_BIT)
#define KK_VI_IS_SI(varint) (!KK_VI_IS_BI(varint))

// Bigint
#define _KK_BI_GET_HEADER(bigint) (*(kk_bi_length_t *)(bigint))
#define _KK_BI_GET_BODY(bigint) ((void *)((uint8_t *)bigint + KK_BI_HEADER_SIZE))

#define KK_BI_FULLP_TO_SMALLP_LEN(length) ((length) * (KK_BI_FULLPART_SIZE / KK_BI_SMALLPART_SIZE))
#define KK_BI_SMALLP_TO_FULLP_LEN(length) ((length) / (KK_BI_FULLPART_SIZE / KK_BI_SMALLPART_SIZE))
#define KK_BI_FULLP_TO_BYTEP_LEN(length) ((length) * (KK_BI_FULLPART_SIZE / KK_BI_BYTEPART_SIZE))
#define KK_BI_BYTEP_TO_FULLP_LEN(length) ((length) / (KK_BI_FULLPART_SIZE / KK_BI_BYTEPART_SIZE))
#define KK_BI_SMALLP_TO_BYTEP_LEN(length) ((length) * (KK_BI_SMALLPART_SIZE / KK_BI_BYTEPART_SIZE))
#define KK_BI_BYTEP_TO_SMALLP_LEN(length) ((length) / (KK_BI_SMALLPART_SIZE / KK_BI_BYTEPART_SIZE))

#define KK_BI_GET_FULLP_LENGTH(bigint) (_KK_BI_GET_HEADER(bigint))
#define KK_BI_GET_FULLP_ARRAY(bigint) ((kk_bi_fullp_arr_t)_KK_BI_GET_BODY(bigint))
#define KK_BI_GET_SMALLP_LENGTH(bigint) (KK_BI_FULLP_TO_SMALLP_LEN(_KK_BI_GET_HEADER(bigint)))
#define KK_BI_GET_SMALLP_ARRAY(bigint) ((kk_bi_smallp_arr_t)_KK_BI_GET_BODY(bigint))
#define KK_BI_GET_BYTEP_LENGTH(bigint) (_KK_BI_GET_HEADER(bigint) * KK_BI_FULLPART_SIZE)
#define KK_BI_GET_BYTEP_ARRAY(bigint) ((kk_bi_bytep_arr_t)_KK_BI_GET_BODY(bigint))

#define KK_BI_CALC_FULL_SIZE(length) (KK_BI_HEADER_SIZE + (length)*KK_BI_FULLPART_SIZE)
#define KK_BI_IS_VALID(bigint) (((kk_vi_t)bigint & 0x3) == 0)
#define KK_BI_NOT_VALID(bigint) (!KK_BI_IS_VALID(bigint))

// Smallint
#define KK_SI_HAS_OVERFLOWED(varint) (((kk_vi_t)(varint) + KK_VI_LARGE_BIT) & KK_VI_SIGN_BIT)
#define KK_SI_NOT_OVERFLOWED(varint) (!KK_SI_HAS_OVERFLOWED(varint))
#define KK_SI_IS_VALID(smallint) KK_SI_NOT_OVERFLOWED(smallint)
#define KK_SI_NOT_VALID(smallint) KK_SI_HAS_OVERFLOWED(smallint)

// Inline Functions ------------------------------------------

#include "_integer-inlines.h"

// Functions -------------------------------------------------

kk_vi_t create_kkvarint(kk_si_t value);
kk_vi_t kkvarint_clone(kk_vi_t value);

kk_bi_t create_kkbigint(kk_si_t value);
kk_bi_t create_kkbigint_parts(kk_bi_length_t parts);
kk_bi_t create_kkbigint_bits(kk_bi_length_t bits);
kk_vi_t create_kkvarint_from_borrowed_hexstr(char *hexstr);
char *create_hexstr_from_borrowed_kkvarint(kk_vi_t varint);
char *create_decstr_from_borrowed_kkvarint(kk_vi_t varint);

kk_bi_length_t kkbigint_get_used_parts(kk_bi_t bigint);
kk_bi_t kkbigint_shrink_to_fit(kk_bi_t bigint);
kk_bi_t kkbigint_resize(kk_bi_t bigint, kk_bi_length_t new_parts);
kk_bi_t kkbigint_clone(kk_bi_t bigint);

// Util functions --------------------------------------------

void print_kkbigint_internal(kk_bi_t bigint);

// Math ------------------------------------------------------

#include "_integer-math.h"
