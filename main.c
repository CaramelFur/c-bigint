#include "integer.h"

int main()
{
  char *ok = malloc(256);
  strcpy(ok, "0123456789abcdeffedcba9876543210");

  kk_varint_t varint;

  for (int i = 0; i < 10000000; i++)
  {
    varint = create_kkvarint_from_borrowed_hexstr(ok);
    ok = create_hexstr_from_borrowed_kkvarint(varint);
  }

  printf("%s\n", ok);

  return 0;
}