#include "integer.h"

#pragma once

static inline kk_si_t kkvarint_as_kksmallint(kk_vi_t kkint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_VI_IS_BI(kkint))
    assert(0 && "Trying to cast a bigint to a smallint.");
#endif

  if (kkint & KK_VI_SIGN_BIT)
    return kkint | KK_VI_META_MASK;
  return kkint;
}

static inline kk_vi_t kksmallint_as_kkvarint(kk_si_t kksmallint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_SI_IS_VALID(kksmallint))
    assert(0 && "Trying to cast a bigint to a smallint.");
#endif

  return kksmallint & KK_VI_DATA_MASK;
}

static inline kk_bi_t kkvarint_as_kkbigint(kk_vi_t kkint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_VI_IS_SI(kkint))
    assert(0 && "Trying to cast a smallint to a bigint.");
#endif

  return (kk_bi_t)(kkint << _KK_SI_BITS_RESERVED);
}

static inline kk_vi_t kkbigint_as_kkvarint(kk_bi_t kkbigint)
{
#ifdef KK_INT_TEST_ON_CAST
  if (KK_BI_IS_VALID(kkbigint))
    assert(0 && "Trying to cast a smallint to a bigint.");
#endif

  return ((kk_vi_t)kkbigint >> _KK_SI_BITS_RESERVED) | KK_VI_LARGE_BIT;
}

static inline void free_kkvarint(kk_vi_t varint)
{
  if (KK_VI_IS_BI(varint))
    free(kkvarint_as_kkbigint(varint));
}

static inline void free_kkbigint(kk_bi_t bigint)
{
  free(bigint);
}
