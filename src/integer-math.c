#include "integer.h"
#include "util.h"

// Internal inlines definitions

static inline kk_vi_t add_borrowed_kkbigint_to_borrowed_kkbigint(kk_bi_t bigint_a, kk_bi_t bigint_b);
static inline kk_vi_t add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(
    kk_bi_t bigint_larger,
    kk_bi_t bigint_smaller,
    kk_bi_length_t larger_length,
    kk_bi_smallp_arr_t data_larger,
    kk_bi_length_t smaller_length,
    kk_bi_smallp_arr_t data_smaller);
static inline kk_vi_t add_borrowed_kkbigint_to_kksmallint(kk_bi_t bigint_a, kk_si_t varint_b);
static inline kk_vi_t add_kksmallint_to_kksmallint(kk_si_t varint_a, kk_si_t varint_b);

// Functions

kk_vi_t _negate_borrowed_kkbigint(kk_bi_t bigint)
{
  kk_bi_length_t length = KK_BI_GET_FULLP_LENGTH(bigint);
  kk_bi_fullp_arr_t data = KK_BI_GET_FULLP_ARRAY(bigint);

  for (kk_bi_length_t i = 0; i < length; i++)
    data[i] = ~data[i];

  kk_bi_fullpart_t one = 1;
  return add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(
      bigint,
      NULL,
      KK_BI_FULLP_TO_SMALLP_LEN(length),
      (kk_bi_smallp_arr_t)data,
      KK_BI_BYTEP_TO_SMALLP_LEN(KK_BI_FULLPART_SIZE),
      (kk_bi_smallp_arr_t)&one);
}

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

static inline kk_vi_t add_kksmallint_to_kksmallint(kk_si_t varint_a, kk_si_t varint_b)
{
  return create_kkvarint(varint_a + varint_b);
}

static inline kk_vi_t add_borrowed_kkbigint_to_borrowed_kkbigint(kk_bi_t bigint_a, kk_bi_t bigint_b)
{
  kk_bi_length_t a_length = KK_BI_GET_SMALLP_LENGTH(bigint_a);
  kk_bi_length_t b_length = KK_BI_GET_SMALLP_LENGTH(bigint_b);
  kk_bi_smallp_arr_t a_data = KK_BI_GET_SMALLP_ARRAY(bigint_a);
  kk_bi_smallp_arr_t b_data = KK_BI_GET_SMALLP_ARRAY(bigint_b);

  if (a_length > b_length)
    return add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(bigint_a, bigint_b, a_length, a_data, b_length, b_data);
  else
    return add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(bigint_b, bigint_a, b_length, b_data, a_length, a_data);
}

static inline kk_vi_t add_borrowed_kkbigint_to_kksmallint(kk_bi_t bigint_a, kk_si_t varint_b)
{
  kk_bi_length_t big_length = KK_BI_GET_SMALLP_LENGTH(bigint_a);
  kk_bi_smallp_arr_t big_data = KK_BI_GET_SMALLP_ARRAY(bigint_a);
  kk_bi_length_t small_length = KK_BI_BYTEP_TO_SMALLP_LEN(sizeof(kk_si_t));
  kk_bi_smallp_arr_t small_data = (kk_bi_smallp_arr_t)&varint_b;

  return add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(bigint_a, NULL, big_length, big_data, small_length, small_data);
}

static inline kk_vi_t add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_option(
    kk_bi_t bigint_larger,
    kk_bi_t bigint_smaller,
    kk_bi_length_t larger_length,
    kk_bi_smallp_arr_t data_larger,
    kk_bi_length_t smaller_length,
    kk_bi_smallp_arr_t data_smaller)
{
  // printf("add_borrowed_larger_kkbigint_to_borrowed_smaller_kkbigint_optionb\n");

  kk_byte smaller_sign = (kk_byte)(data_smaller[smaller_length - 1] >> (KK_BI_BYTEPART_BITS - 1));
  kk_byte larger_sign = (kk_byte)(data_larger[larger_length - 1] >> (KK_BI_BYTEPART_BITS - 1));

  kk_bi_smallpart_t signext = (!smaller_sign) + KK_BI_SMALLPART_MAX;
  // smaller_sign
  //     ? KK_BI_SMALLPART_MAX
  //     : 0;

  kk_bi_fullpart_t carry = 0;
  kk_bi_smallpart_t a = 0;
  kk_bi_smallpart_t b = 0;
  kk_bi_length_t i = 0;

  for (; i < smaller_length; i++)
  {
    a = data_smaller[i];
    b = data_larger[i];
    carry = (kk_bi_fullpart_t)a + (kk_bi_fullpart_t)b + carry;
    data_larger[i] = (kk_bi_smallpart_t)carry;

    carry >>= KK_BI_SMALLPART_BITS;
  }

  free_kkbigint(bigint_smaller);

  for (; i < larger_length; i++)
  {
    b = data_larger[i];
    carry = (kk_bi_fullpart_t)signext + (kk_bi_fullpart_t)b + carry;
    data_larger[i] = (kk_bi_smallpart_t)carry;

    carry >>= KK_BI_SMALLPART_BITS;
  }

  kk_byte final_sign_inv = !(data_larger[larger_length - 1] >> (KK_BI_BYTEPART_BITS - 1));

  // TODO: optimize this
  if (
      (final_sign_inv & smaller_sign & larger_sign) == 1 ||
      (final_sign_inv || smaller_sign || larger_sign) == 0)
  {
    kk_bi_length_t new_length = KK_BI_SMALLP_TO_FULLP_LEN(larger_length) + 1;

    bigint_larger = kkbigint_resize(bigint_larger, new_length);
    kk_bi_fullp_arr_t data_new = KK_BI_GET_FULLP_ARRAY(bigint_larger);

    data_new[new_length - 1] = signext + carry;
  }
#ifdef KK_INT_AUTO_SHRINK
  // If we haven't carried, we can check if we can shrink
  else
    return kkvarint_shrink(kkbigint_as_kkvarint(bigint_larger));
#endif

  return kkbigint_as_kkvarint(bigint_larger);
}
