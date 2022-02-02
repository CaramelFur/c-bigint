#include "integer.h"
#include "util.h"

// Internal ilines

static inline kk_varint_t add_borrowed_kkbigint_to_borrowed_kkbigint(kk_bigint_t bigint_a, kk_bigint_t bigint_b)
{
  printf("add_borrowed_kkbigint_to_borrowed_kkbigint\n");

  free_kkbigint(bigint_a);
  free_kkbigint(bigint_b);
  return 0;
}

static inline kk_varint_t add_borrowed_kkbigint_to_kksmallint(kk_bigint_t bigint_a, kk_smallint_t varint_b)
{
  printf("add_borrowed_kkbigint_to_borrowed_kkvarint\n");
  free_kkbigint(bigint_a);
  return 0;
}

static inline kk_varint_t add_kksmallint_to_kksmallint(kk_smallint_t varint_a, kk_smallint_t varint_b)
{
  printf("add_kksmallint_to_kksmallint\n");
  return 0;
}

// Functions

kk_varint_t _add_borrowed_kkvarint_to_borrowed_kkvarint_slow(kk_varint_t varint_a, kk_varint_t varint_b)
{
  if (KK_VARINT_IS_BIGINT(varint_a & varint_b))
    return add_borrowed_kkbigint_to_borrowed_kkbigint(kkvarint_as_kkbigint(varint_a), kkvarint_as_kkbigint(varint_b));

  if (KK_VARINT_IS_BIGINT(varint_a))
    return add_borrowed_kkbigint_to_kksmallint(kkvarint_as_kkbigint(varint_a), kkvarint_as_kksmallint(varint_b));

  if (KK_VARINT_IS_BIGINT(varint_b))
    return add_borrowed_kkbigint_to_kksmallint(kkvarint_as_kkbigint(varint_b), kkvarint_as_kksmallint(varint_a));

  return add_kksmallint_to_kksmallint(kkvarint_as_kksmallint(varint_a), kkvarint_as_kksmallint(varint_b));
}
