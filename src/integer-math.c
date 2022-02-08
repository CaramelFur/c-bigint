#include "integer.h"
#include "util.h"

// Internal inlines definitions

static inline kk_vi_t add_borrowed_kkbigint_to_borrowed_kkbigint(kk_bi_t bigint_a, kk_bi_t bigint_b);
static inline kk_vi_t add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(kk_bi_t bigint_larger, kk_bi_t bigint_smaller);
static inline kk_vi_t add_borrowed_kkbigint_to_kksmallint(kk_bi_t bigint_a, kk_si_t varint_b);
static inline kk_vi_t add_kksmallint_to_kksmallint(kk_si_t varint_a, kk_si_t varint_b);

// Functions

kk_vi_t _add_borrowed_kkvarint_to_borrowed_kkvarint_slow(kk_vi_t varint_a, kk_vi_t varint_b)
{
  if (KK_VI_IS_BI(varint_a & varint_b))
    return add_borrowed_kkbigint_to_borrowed_kkbigint(kkvarint_as_kkbigint(varint_a), kkvarint_as_kkbigint(varint_b));

  if (KK_VI_IS_BI(varint_a))
    return add_borrowed_kkbigint_to_kksmallint(kkvarint_as_kkbigint(varint_a), kkvarint_as_kksmallint(varint_b));

  if (KK_VI_IS_BI(varint_b))
    return add_borrowed_kkbigint_to_kksmallint(kkvarint_as_kkbigint(varint_b), kkvarint_as_kksmallint(varint_a));

  return add_kksmallint_to_kksmallint(kkvarint_as_kksmallint(varint_a), kkvarint_as_kksmallint(varint_b));
}

// Internal inlines

static inline kk_vi_t add_borrowed_kkbigint_to_borrowed_kkbigint(kk_bi_t bigint_a, kk_bi_t bigint_b)
{
  if (KK_BI_GET_FULLP_LENGTH(bigint_a) > KK_BI_GET_FULLP_LENGTH(bigint_b))
    return add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(bigint_a, bigint_b);
  else
    return add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(bigint_b, bigint_a);
}

static inline kk_vi_t add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(kk_bi_t bigint_larger, kk_bi_t bigint_smaller)
{
  // printf("add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_optionb\n");
  kk_bi_length_t larger_length = KK_BI_GET_SMALLP_LENGTH(bigint_larger);
  kk_bi_length_t smaller_length = KK_BI_GET_SMALLP_LENGTH(bigint_smaller);
  kk_bi_smallp_arr_t data_larger = KK_BI_GET_SMALLP_ARRAY(bigint_larger);
  kk_bi_smallp_arr_t data_smaller = KK_BI_GET_SMALLP_ARRAY(bigint_smaller);

  kk_bi_fullpart_t carry = 0;

  kk_bi_smallpart_t a = 0;
  kk_bi_smallpart_t b = 0;

  for (size_t i = 0; i < smaller_length; i++)
  {
    a = data_smaller[i];
    b = data_larger[i];
    carry = (kk_bi_fullpart_t)a + (kk_bi_fullpart_t)b + carry;
    data_larger[i] = (kk_bi_smallpart_t)carry;

    carry >>= KK_BI_SMALLPART_BITS;
  }

  if (carry)
  {
    if (smaller_length == larger_length)
    {
      bigint_larger = kkbigint_resize(bigint_larger, KK_BI_SMALLP_TO_FULLP_LEN(larger_length) + 1);
      data_larger = KK_BI_GET_SMALLP_ARRAY(bigint_larger);
    }
    data_larger[smaller_length] = (kk_bi_smallpart_t)carry;
  }

  free_kkbigint(bigint_smaller);

  return kkbigint_as_kkvarint(bigint_larger);
}

static inline kk_vi_t add_borrowed_kkbigint_to_kksmallint(kk_bi_t bigint_a, kk_si_t varint_b)
{
  // printf("add_borrowed_kkbigint_to_borrowed_kkvarint\n");
  free_kkbigint(bigint_a);

  // ignore unused
  (void)varint_b;
  return 0;
}

static inline kk_vi_t add_kksmallint_to_kksmallint(kk_si_t varint_a, kk_si_t varint_b)
{
  return create_kkvarint(varint_a + varint_b);
}
