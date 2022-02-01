#include <time.h>
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

  // test

  // Uses 31 bits

  #define COUNT 100000000

  kk_varint_t a = 0x2fffffffffffffff;

  clock_t begin, end;

  printf("Start\n");
  
  kk_varint_t c[2] = {0, 0};

  begin = clock();

  for(kk_varint_t i = 0; i < COUNT; i++)
  {
    c[i%2] += add_borrowed_kkvarint_to_borrowed_kkvarint(a, i*0xfffffffffffff);
  }

  printf("%lu\n", c[0]);

  end = clock();
  printf("Add: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);

  c[0] = 0;
  c[1] = 0;

  printf("Start\n");

  begin = clock();

  for(kk_varint_t i = 0; i < COUNT; i++)
  {
    c[i%2] += a + i*0xfffffffffffff;
  }

  printf("%lu\n", c[0]);

  end = clock();

  printf("Add: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);

  //char *d = create_hexstr_from_borrowed_kkvarint(c);

  //printf("%s\n", d);

  // printf("%lx\n", KK_SMALLINT_MAX);

  return 0;
}
