#include "integer.h"

#pragma once

static inline kk_smallint_t kkvarint_as_kksmallint(kk_varint_t kkint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_VARINT_IS_BIGINT(kkint))
    assert(0 && "Trying to cast a bigint to a smallint.");
#endif

  if (kkint & KK_VARINT_SIGN_BIT)
    return kkint | KK_VARINT_META_MASK;
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

  return (kk_bigint_t)(kkint << _KK_SMALLINT_BITS_RESERVED);
}

static inline kk_varint_t kkbigint_as_kkvarint(kk_bigint_t kkbigint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_BIGINT_IS_VALID(kkbigint))
    assert(0 && "Trying to cast a smallint to a bigint.");
#endif

  return ((kk_varint_t)kkbigint >> _KK_SMALLINT_BITS_RESERVED) | KK_VARINT_LARGE_BIT;
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
