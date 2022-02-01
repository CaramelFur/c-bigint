#include "integer.h"
#include "util.h"

#ifdef __GET_INLINES
static inline kk_varint_t add_borrowed_kkvarint_to_borrowed_kkvarint(kk_varint_t varint_a, kk_varint_t varint_b)
{
  kk_varint_t result = varint_a + varint_b;
  if (KK_SMALLINT_NOT_OVERFLOWED(result))
    return result;

  return add_borrowed_kkvarint_to_borrowed_kkvarint_slow(varint_a, varint_b);
}
#else
kk_varint_t add_borrowed_kkvarint_to_borrowed_kkvarint_slow(kk_varint_t varint_a, kk_varint_t varint_b)
{
  if (KK_VARINT_IS_BIGINT(varint_a | varint_b))
  {
    printf("Big become big\n");
  }
  else
  {
    printf("Small become big\n");
  }

  free_kkvarint(varint_a);
  free_kkvarint(varint_b);

  return 0;
}
#endif