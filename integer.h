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

#define KK_SMALLINT_SIZE KK_VARINT_SIZE
#define KK_SMALLINT_BITS KK_VARINT_BITS
#define KK_SMALLINT_BITS_DATA (KK_VARINT_BITS - 1)
#define KK_SMALLINT_MAX (KK_VARINT_MAX >> 1)
#define KK_SMALLINT_MIN (-KK_SMALLINT_MAX - 1)

#define KK_VARINT_BIT_LSBF(bit) ((kk_varint_t)1 << (bit))
#define KK_VARINT_BIT_MSBF(bit) ((kk_varint_t)1 << (KK_VARINT_BITS - (bit)-1))
#define KK_VARINT_LARGE_BIT KK_VARINT_BIT_MSBF(0)
#define KK_VARINT_SIGN_BIT KK_VARINT_BIT_MSBF(1)
#define KK_VARINT_DATA_MASK (~KK_VARINT_LARGE_BIT)

#define KK_SMALLINT_SIGN_BIT KK_VARINT_LARGE_BIT

typedef void *kk_bigint_t;
typedef size_t kk_bigint_length_t;
typedef kk_smallint_t *kk_bigint_data_array_t;

// Define functions

#define KK_BIGINT_GET_DATA_LENGTH(bigint) (*(kk_bigint_length_t *)(bigint))
#define KK_BIGINT_GET_DATA_ARRAY(bigint) ((kk_bigint_data_array_t)((uint8_t *)bigint + sizeof(kk_bigint_length_t)))

#define KK_VARINT_IS_BIGINT(varint) ((varint)&KK_VARINT_LARGE_BIT)
#define KK_VARINT_IS_SMALLINT(varint) (!KK_VARINT_IS_BIGINT(varint))
#define KK_SMALLINT_IS_VALID(smallint) ((smallint) <= KK_SMALLINT_MAX && (smallint) >= KK_SMALLINT_MIN)
#define KK_BIGINT_IS_VALID(bigint) (((kk_varint_t)bigint & 1) != 0)

// Inline Functions ------------------------------------------

static inline kk_smallint_t kkvarint_as_kksmallint(kk_varint_t kkint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_VARINT_IS_BIGINT(kkint))
    assert(0 && "Trying to cast a bigint to a smallint.");
#endif

  if (kkint & KK_VARINT_SIGN_BIT)
    return kkint | KK_SMALLINT_SIGN_BIT;
  return kkint;
}

static inline kk_varint_t kksmallint_as_kkvarint(kk_smallint_t kksmallint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_SMALLINT_IS_VALID(kksmallint))
    assert(0 && "Trying to cast a bigint to a smallint.");
#endif

  return kksmallint & KK_VARINT_DATA_MASK;
}

static inline kk_bigint_t kkvarint_as_kkbigint(kk_varint_t kkint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_VARINT_IS_SMALLINT(kkint))
    assert(0 && "Trying to cast a smallint to a bigint.");
#endif

  return (kk_bigint_t)(kkint << 1);
}

static inline kk_varint_t kkbigint_as_kkvarint(kk_bigint_t kkbigint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_BIGINT_IS_VALID(kkbigint))
    assert(0 && "Trying to cast a smallint to a bigint.");
#endif

  return (kk_varint_t)kkbigint >> 1 | KK_VARINT_LARGE_BIT;
}

static inline void free_kkvarint(kk_varint_t varint)
{
  if (KK_VARINT_IS_BIGINT(varint))
    free(kkvarint_as_kkbigint(varint));
}

static inline void free_kkbigint(kk_bigint_t bigint)
{
  free(bigint);
}

// Functions -------------------------------------------------

kk_bigint_t create_kkbigint_parts(size_t parts);

kk_bigint_t create_kkbigint_bits(size_t bits);

kk_varint_t create_kkvarint_from_borrowed_hexstr(char *hexstr);

char *create_hexstr_from_borrowed_kkvarint(kk_varint_t varint);

// Util functions --------------------------------------------

void print_kkbigint_internal(kk_bigint_t bigint);