#include "integer.h"

#pragma once

// Functions

kk_varint_t add_borrowed_kkvarint_to_borrowed_kkvarint_slow(kk_varint_t varint_a, kk_varint_t varint_b);

// Inlines

static inline kk_varint_t add_borrowed_kkvarint_to_borrowed_kkvarint(kk_varint_t varint_a, kk_varint_t varint_b)
{
  kk_varint_t result = varint_a + varint_b;
  if (KK_SMALLINT_NOT_OVERFLOWED(result))
    return result;

  return add_borrowed_kkvarint_to_borrowed_kkvarint_slow(varint_a, varint_b);
}

