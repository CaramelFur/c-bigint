#include "integer.h"
#include "util.h"

kk_bigint_t create_kkbigint_parts(size_t parts)
{
  size_t bytes = sizeof(parts) + (parts * KK_SMALLINT_SIZE);

  kk_bigint_t bigint = aligned_alloc(_KK_SMALLINT_BITS_ALIGNMENT, bytes);
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

  kk_varint_t varint;
  kk_bigint_byte_array_t dat_arr;
  if (size_bits > KK_SMALLINT_BITS_DATA)
  {
    kk_bigint_t bigint = create_kkbigint_bits(size_bits);
    varint = kkbigint_as_kkvarint(bigint);
    dat_arr = KK_BIGINT_GET_BYTE_ARRAY(bigint);
  }
  else
  {
    varint = 0;
    dat_arr = (kk_bigint_byte_array_t)&varint;
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

char *create_hexstr_from_borrowed_kkvarint(kk_varint_t varint)
{
  kk_bigint_t bigint;
  kk_bigint_length_t data_length;
  kk_bigint_byte_array_t data_arr;

  if (KK_VARINT_IS_BIGINT(varint))
  {
    bigint = kkvarint_as_kkbigint(varint);
    data_length = KK_BIGINT_GET_BYTE_LENGTH(bigint);
    data_arr = KK_BIGINT_GET_BYTE_ARRAY(bigint);
  }
  else
  {
    bigint = NULL;
    data_length = sizeof(kk_smallint_t);
    data_arr = (kk_bigint_byte_array_t)&varint;
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
