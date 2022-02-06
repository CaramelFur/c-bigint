#include "integer.h"

#pragma once

// Functions

kk_vi_t _add_borrowed_kkvarint_to_borrowed_kkvarint_slow(kk_vi_t varint_a, kk_vi_t varint_b);

// Inlines

static inline kk_vi_t add_borrowed_kkvarint_to_borrowed_kkvarint(kk_vi_t varint_a, kk_vi_t varint_b)
{
  kk_vi_t result = varint_a + varint_b;

  if (KK_SI_NOT_OVERFLOWED(result))
    return result;

  return _add_borrowed_kkvarint_to_borrowed_kkvarint_slow(varint_a, varint_b);
}

