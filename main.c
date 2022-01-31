#include "integer.h"
#include "util.h"

char *malloc_string(const char *source)
{
  char *result = malloc(strlen(source) + 1);
  strcpy(result, source);
  return result;
}

int main()
{
  // char*start = "0123456789abcdeffedcba98765432100123456789abcdeffedcba987654321";

  // char *ok = malloc(128);
  // memcpy(ok, start, strlen(start));

  // kk_varint_t varint;

  // for (int i = 0; i < 100000000; i++)
  // {
  //   varint = create_kkvarint_from_borrowed_hexstr(ok);
  //   // print_kkbigint_internal(kkvarint_as_kkbigint(varint));
  //   ok = create_hexstr_from_borrowed_kkvarint(varint);
  // }

  // printf("%s\n", ok);

  // Uses 31 bits
  kk_varint_t a = 0x3fffffffffffffff;
  kk_varint_t b = create_kkvarint_from_borrowed_hexstr(malloc_string("1"));

  printf("oi\n");

  kk_varint_t c = add_borrowed_kkvarint_to_borrowed_kkvarint(a, b);

  char *d = create_hexstr_from_borrowed_kkvarint(c);

  printf("%s\n", d);

  printf("%lx\n", KK_SMALLINT_MAX);

  return 0;
}