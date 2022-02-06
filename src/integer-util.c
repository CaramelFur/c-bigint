#include "integer.h"
#include "util.h"

void print_kkbigint_internal(kk_bi_t bigint)
{
  kk_bi_length_t data_length = KK_BIGINT_GET_DATA_LENGTH(bigint);
  kk_bi_fullpart_t dat_arr = KK_BIGINT_GET_DATA_ARRAY(bigint);

  for (size_t i = 0; i < data_length; i++)
  {
    print_bits(dat_arr + i, KK_BI_FULLPART_SIZE);
  }
  printf("\n");
}
