#include "integer.h"
#include "util.h"

int main()
{
  char *ok = malloc(32 * 1024 + 1);
  for(int i = 0; i < 1024; i++)
    memcpy(ok + i * 32, "0123456789abcdef0123456789abcdef", 32);

  kk_varint_t varint;

  for (int i = 0; i < 100000; i++)
  {
    varint = create_kkvarint_from_borrowed_hexstr(ok);
    //print_kkbigint_internal(kkvarint_as_kkbigint(varint));
    ok = create_hexstr_from_borrowed_kkvarint(varint);
  }

  printf("%s\n", ok);


  return 0;
}