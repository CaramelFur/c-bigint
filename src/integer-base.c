#include "integer.h"
#include "util.h"

// Base varint functions

kk_vi_t create_kkvarint(kk_si_t value)
{
  if (KK_SI_IS_VALID(value))
    return kksmallint_as_kkvarint(value);

  kk_bi_t bigint = create_kkbigint_parts(1);

  kk_bi_fullp_arr_t data = KK_BI_GET_FULLP_ARRAY(bigint);
  data[0] = (kk_bi_fullpart_t)value;

  return kkbigint_as_kkvarint(bigint);
}

kk_vi_t kkvarint_clone(kk_vi_t varint)
{
  if (KK_VI_IS_SI(varint))
    return varint;

  return kkbigint_as_kkvarint(kkbigint_clone(kkvarint_as_kkbigint(varint)));
}

// Base Bigint functions

kk_bi_t create_kkbigint_parts(kk_bi_length_t parts)
{
  kk_bi_length_t bytes = KK_BI_CALC_FULL_SIZE(parts);

  kk_bi_t bigint = aligned_alloc(_KK_SI_BITS_ALIGNMENT, bytes);
  *((kk_bi_length_t *)bigint) = parts;

  return bigint;
}

kk_bi_t create_kkbigint_bits(kk_bi_length_t bits)
{
  kk_bi_length_t bigint_part_count = (bits / KK_BI_FULLPART_BITS);
  if (bits % KK_BI_FULLPART_BITS > 0)
    bigint_part_count += 1;

  return create_kkbigint_parts(bigint_part_count);
}

kk_bi_length_t kkbigint_get_used_parts(kk_bi_t bigint)
{
  kk_bi_length_t i = KK_BI_GET_FULLP_LENGTH(bigint);
  kk_bi_fullp_arr_t data = KK_BI_GET_FULLP_ARRAY(bigint);

  do
    i--;
  while (i > 0 && data[i] == 0);

  return i + 1;
}

kk_bi_t kkbigint_shrink_to_fit(kk_bi_t bigint)
{
  kk_bi_length_t used_parts = kkbigint_get_used_parts(bigint);
  if (used_parts == KK_BI_GET_FULLP_LENGTH(bigint))
    return bigint;

  // No need to check for alignement, as we are shrinking
  // This SHOULD always keep the same address
  kk_bi_t newbigint = realloc(bigint, KK_BI_CALC_FULL_SIZE(used_parts));

  // TODO: Remove eventually
  if (newbigint != bigint)
  {
    printf("[DANGER] Shrinking realloc moved data, please report immediately: https://github.com/koka-lang/koka/issues\n");
    fflush(stdout);
    assert(0);
  }

  return newbigint;
}

kk_bi_t kkbigint_resize(kk_bi_t bigint, kk_bi_length_t new_parts)
{
  bigint = realloc(bigint, KK_BI_CALC_FULL_SIZE(new_parts));
  if (KK_BI_IS_VALID(bigint))
    return bigint;

  // Alignment failed, now we go do it the slow way
  kk_bi_length_t length = KK_BI_GET_BYTEP_LENGTH(bigint);

  kk_bi_t newbigint = create_kkbigint_parts(new_parts);
  memcpy((uint8_t *)newbigint + KK_BI_HEADER_SIZE, (uint8_t *)bigint + KK_BI_HEADER_SIZE, length);

  free_kkbigint(bigint);
  return newbigint;
}

kk_bi_t kkbigint_clone(kk_bi_t bigint)
{
  kk_bi_length_t bytes = KK_BI_CALC_FULL_SIZE(KK_BI_GET_FULLP_LENGTH(bigint));
  kk_bi_t newbigint = aligned_alloc(_KK_SI_BITS_ALIGNMENT, bytes);

  memcpy(newbigint, bigint, bytes);
  return newbigint;
}

// Parsing functions

kk_vi_t create_kkvarint_from_borrowed_hexstr(char *hexstr)
{
  size_t size_str = strlen(hexstr);
  size_t size_bits = size_str * 4;

  kk_vi_t varint;
  kk_bi_bytep_arr_t dat_arr;
  if (size_bits > KK_SI_BITS_DATA)
  {
    kk_bi_t bigint = create_kkbigint_bits(size_bits);
    varint = kkbigint_as_kkvarint(bigint);
    dat_arr = KK_BI_GET_BYTEP_ARRAY(bigint);
  }
  else
  {
    varint = 0;
    dat_arr = (kk_bi_bytep_arr_t)&varint;
  }

  uint8_t nibble = 0;
  uint8_t mask = 0;
  for (size_t i = 0; i < size_str; i++)
  {
    nibble = hex2nibble(hexstr[size_str - 1 - i]); // half a byte

    // This removes a branch, so branch prediction can do its thing
    dat_arr[i / 2] |= (uint8_t)(nibble << mask);
    mask ^= 4;
  }

  free(hexstr);

  return varint;
}

char *create_hexstr_from_borrowed_kkvarint(kk_vi_t varint)
{
  kk_bi_t bigint;
  kk_bi_length_t data_length;
  kk_bi_bytep_arr_t data_arr;

  if (KK_VI_IS_BI(varint))
  {
    bigint = kkvarint_as_kkbigint(varint);
    data_length = KK_BI_GET_BYTEP_LENGTH(bigint);
    data_arr = KK_BI_GET_BYTEP_ARRAY(bigint);
  }
  else
  {
    bigint = NULL;
    data_length = KK_SI_SIZE;
    data_arr = (kk_bi_bytep_arr_t)&varint;
  }

  size_t size_str = data_length * 2;
  char *output = malloc(size_str + 1);
  size_t output_offset = 0;

  output[0] = '0';

  uint8_t nibble = 0;
  uint8_t mask = 0;
  for (size_t i = 0; i < size_str; i++)
  {
    nibble = data_arr[data_length - 1 - (i / 2)];

    // This removes a branch, so branch prediction can do its thing
    mask ^= 4;
    nibble = (uint8_t)(nibble >> mask);

    if (!output_offset && !(nibble & 0x0F))
      continue;

    output[output_offset++] = nibble2hex(nibble);
  };

  output[output_offset ? output_offset : 1] = '\0';

  free(bigint);

  return output;
}

char *create_decstr_from_borrowed_kkvarint(kk_vi_t varint)
{
  kk_bi_t bigint;
  kk_bi_length_t data_length;
  kk_bi_bytep_arr_t data_arr;

  if (KK_VI_IS_BI(varint))
  {
    bigint = kkvarint_as_kkbigint(varint);
    data_length = KK_BI_GET_BYTEP_LENGTH(bigint);
    data_arr = KK_BI_GET_BYTEP_ARRAY(bigint);
  }
  else
  {
    bigint = NULL;
    data_length = KK_SI_SIZE;
    data_arr = (kk_bi_bytep_arr_t)&varint;
  }

  size_t size_str = data_length * 2;
  char *output = malloc(size_str + 1);
  size_t output_offset = 0;

  output[0] = '0';

  uint8_t nibble = 0;
  uint8_t mask = 0;
  for (size_t i = 0; i < size_str; i++)
  {
    nibble = data_arr[data_length - 1 - (i / 2)];

    // This removes a branch, so branch prediction can do its thing
    mask ^= 4;
    nibble = (uint8_t)(nibble >> mask);

    if (!output_offset && !(nibble & 0x0F))
      continue;

    output[output_offset++] = nibble2hex(nibble);
  };

  output[output_offset ? output_offset : 1] = '\0';

  free(bigint);

  return output;
}
