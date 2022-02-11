#include "integer.h"
#include "util.h"

void print_kkvarint_internal(kk_vi_t varint)
{
  if (KK_VI_IS_SI(varint))
  {
    kk_si_t small = kkvarint_as_kksmallint(varint);
    print_bits(&small, sizeof(small));
  }
  else
  {
    print_kkbigint_internal(kkvarint_as_kkbigint(varint));
  }
}

void print_kkbigint_internal(kk_bi_t bigint)
{
  kk_bi_length_t data_length = KK_BI_GET_FULLP_LENGTH(bigint);
  kk_bi_fullp_arr_t dat_arr = KK_BI_GET_FULLP_ARRAY(bigint);

  for (size_t i = 0; i < data_length; i++)
  {
    print_bits(dat_arr + i, KK_BI_FULLPART_SIZE);
  }
  printf("\n");
}
