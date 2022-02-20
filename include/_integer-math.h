#include "integer.h"

#pragma once

// Functions

kk_vi_t _add_borrowed_kkvarint_to_borrowed_kkvarint_slow(kk_vi_t varint_a, kk_vi_t varint_b);
kk_vi_t _negate_borrowed_kkbigint(kk_bi_t bigint);

static inline kk_vi_t add_borrowed_kkvarint_to_borrowed_kkvarint(kk_vi_t varint_a, kk_vi_t varint_b);
static inline kk_vi_t negate_borrowed_kkvarint(kk_vi_t varint);
static inline kk_byte kkvarint_is_negative(kk_vi_t varint);
static inline kk_byte kkbigint_is_negative(kk_bi_t bigint);

// Inlines

static inline kk_vi_t add_borrowed_kkvarint_to_borrowed_kkvarint(kk_vi_t varint_a, kk_vi_t varint_b)
{
  kk_vi_t result = varint_a + varint_b;

  if (KK_SI_NOT_OVERFLOWED(result))
    return result;

  return _add_borrowed_kkvarint_to_borrowed_kkvarint_slow(varint_a, varint_b);
}

static inline kk_vi_t negate_borrowed_kkvarint(kk_vi_t varint)
{
  if (KK_VI_IS_SI(varint))
    return ~varint + 1;
  else
    return _negate_borrowed_kkbigint(kkvarint_as_kkbigint(varint));
}

static inline kk_byte kkvarint_is_negative(kk_vi_t varint)
{
  if (KK_VI_IS_SI(varint))
    return kkvarint_as_kksmallint(varint) < 0;
  else
    return kkbigint_is_negative(kkvarint_as_kkbigint(varint));
}

static inline kk_byte kkbigint_is_negative(kk_bi_t bigint)
{
  kk_bi_length_t length = KK_BI_GET_BYTEP_LENGTH(bigint);
  kk_bi_bytep_arr_t data = KK_BI_GET_BYTEP_ARRAY(bigint);
  return data[length - 1] & 0x80;
}
