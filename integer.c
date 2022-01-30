#include "integer.h"
#include "util.h"

kk_bigint_t create_kkbigint_parts(size_t parts)
{
  size_t bytes = sizeof(parts) + (parts * KK_SMALLINT_SIZE);

  kk_bigint_t bigint = aligned_alloc(2, bytes);
  *((kk_bigint_length_t *)bigint) = parts;

  return bigint;
}

kk_bigint_t create_kkbigint_bits(size_t bits)
{
  size_t bigint_part_count = (bits / KK_VARINT_BITS);
  if (bits % KK_VARINT_BITS > 0)
    bigint_part_count += 1;

  return create_kkbigint_parts(bigint_part_count);
}

kk_varint_t create_kkvarint_from_borrowed_hexstr(char *hexstr)
{
  size_t size_str = strlen(hexstr);
  size_t size_bits = size_str * 4;

  // This could be more precise, but this is so much faster
  if (size_bits <= KK_SMALLINT_BITS_DATA)
    return (kk_varint_t)strtoll(hexstr, NULL, 16);

  kk_bigint_t bigint = create_kkbigint_bits(size_bits);
  kk_bigint_byte_array_t dat_arr = KK_BIGINT_GET_BYTE_ARRAY(bigint);

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

  return kkbigint_as_kkvarint(bigint);
}

char *create_hexstr_from_borrowed_kkvarint(kk_varint_t varint)
{
  if (KK_VARINT_IS_SMALLINT(varint))
  {
    // This could be more precise, but this is faster
    char *output = malloc(hexs_for_n_bits(KK_SMALLINT_BITS) + 1);
    sprintf(output, "%lx", varint);
    return output;
  }

  kk_bigint_t bigint = kkvarint_as_kkbigint(varint);
  kk_bigint_length_t data_length = KK_BIGINT_GET_BYTE_LENGTH(bigint);
  kk_bigint_byte_array_t dat_arr = KK_BIGINT_GET_BYTE_ARRAY(bigint);

  size_t size_str = data_length * 2;
  char *output = malloc(size_str + 1);
  size_t output_offset = 0;

  uint8_t nibble = 0;
  uint8_t mask = 0;
  for (size_t i = 0; i < size_str; i++)
  {
    nibble = dat_arr[data_length - 1 - (i / 2)];

    // This removes a branch, so branch prediction can do its thing
    mask ^= 4;
    nibble >>= mask;

    if (!output_offset && !(nibble & 0x0F))
      continue;

    output[output_offset++] = nibble2hex(nibble);
  };
  output[output_offset] = '\0';

  free(bigint);

  return output;
}

char *create_decstr_from_borrowed_kkvarint(kk_varint_t varint)
{
  if (KK_VARINT_IS_SMALLINT(varint))
  {
    // This could be more precise, but this is faster
    char *output = malloc(decimals_for_n_bits(KK_SMALLINT_BITS) + 1);
    sprintf(output, "%ld", varint);
    return output;
  }

  kk_bigint_t bigint = kkvarint_as_kkbigint(varint);
  kk_bigint_length_t data_length = KK_BIGINT_GET_DATA_LENGTH(bigint);
  kk_bigint_data_array_t dat_arr = KK_BIGINT_GET_DATA_ARRAY(bigint);

  size_t strlength = decimals_for_n_bits(data_length * KK_SMALLINT_BITS);
  char *output = malloc(strlength + 1);
  size_t output_offset = 0;

  size_t dat_arr_i = 0;
  uint8_t nibble = 0;
  for (size_t i = 0; i <= strlength; i++)
  {
    dat_arr_i = i / 2 / KK_SMALLINT_SIZE;
    nibble = (uint8_t)(dat_arr[dat_arr_i] >> (KK_VARINT_BITS - 4)) & 0x0f;
    dat_arr[dat_arr_i] <<= 4;

    if (!nibble)
      continue;

    output[output_offset++] = nibble2hex(nibble);
  };
  output[output_offset] = '\0';

  free(bigint);

  return output;
}
