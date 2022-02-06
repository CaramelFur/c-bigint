#include <time.h>
#include "integer.h"
#include "util.h"

char *malloc_string(const char *source)
{
  char *result = malloc(strlen(source) + 1);
  strcpy(result, source);
  return result;
}

void test1()
{
  char *start = "0123456789abcdeffedcba98765432100123456789abcdeffedcba987654321";

  char *ok = malloc(128);
  memcpy(ok, start, strlen(start));

  kk_vi_t varint;

  for (int i = 0; i < 10000000; i++)
  {
    varint = create_kkvarint_from_borrowed_hexstr(ok);
    // print_kkbigint_internal(kkvarint_as_kkbigint(varint));
    ok = create_hexstr_from_borrowed_kkvarint(varint);
  }

  printf("%s\n", ok);
}

void test2()
{
  kk_vi_t COUNT = 100000000;

  kk_vi_t a = 1;

  clock_t begin, end;

  printf("Start\n");

  kk_vi_t c[2] = {0, 0};

  begin = clock();

  for (kk_vi_t i = 0; i < COUNT; i++)
  {
    c[i % 2] = c[i % 2] * add_borrowed_kkvarint_to_borrowed_kkvarint(a, i * 0xffffffff) % 10;
  }

  printf("%lu\n", c[0]);

  end = clock();
  printf("Add: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);

  c[0] = 0;
  c[1] = 0;

  printf("Start\n");

  begin = clock();

  for (kk_vi_t i = 0; i < COUNT; i++)
  {
    c[i % 2] = c[i % 2] * (a + i * 0xffffffff) % 10;
  }

  printf("%lu\n", c[0]);

  end = clock();

  printf("Add: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
}

void test3()
{
  size_t ALIGN = 512;

  for (size_t i = 0; i < 100000; i++)
  {
    size_t size = (size_t)(rand() % 512) + 8;

    uint8_t *testalloc = aligned_alloc(ALIGN, size * ALIGN);

    uint8_t *newtestalloc = realloc(testalloc, size * ALIGN * 2);

    if (((uint64_t)newtestalloc % ALIGN) != 0)
    {
      printf("%p %p %lu\n", testalloc, newtestalloc, size);
    }
  }
}

void test4()
{
  // 64 bit of ones
  char *start_one = "ffffffffffffffff69";
  char *start_two = "ffffffffffffffff69";

  char *ok_one = malloc(128);
  char *ok_two = malloc(128);
  memcpy(ok_one, start_one, strlen(start_one));
  memcpy(ok_two, start_two, strlen(start_two));

  kk_vi_t a = create_kkvarint_from_borrowed_hexstr(ok_one);
  kk_vi_t b = create_kkvarint_from_borrowed_hexstr(ok_two);

  for (int i = 0; i < 10000; i++)
  {
    kk_vi_t c = kkvarint_clone(b);
    a = add_borrowed_kkvarint_to_borrowed_kkvarint(a, c);
  }

  char *ok = create_hexstr_from_borrowed_kkvarint(a);
  printf("%s\n%s\n", start_one, ok);
}

void test5()
{
  int64_t test = -1;
  printf("%lx\n", test);
}

int main()
{
  test1();

  return 0;
}
