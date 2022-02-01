#include "integer.h"
#include "util.h"

void print_kkbigint_internal(kk_bigint_t bigint)
{
  kk_bigint_length_t data_length = KK_BIGINT_GET_DATA_LENGTH(bigint);
  kk_bigint_data_array_t dat_arr = KK_BIGINT_GET_DATA_ARRAY(bigint);

  for (size_t i = 0; i < data_length; i++)
  {
    print_bits(dat_arr + i, KK_SMALLINT_SIZE);
  }
  printf("\n");
}
