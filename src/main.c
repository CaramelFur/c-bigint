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
  char start_one[] = "0";

  char *ok_one = malloc(18);
  memcpy(ok_one, start_one, strlen(start_one));

  kk_vi_t accumulator = create_kkvarint_from_borrowed_hexstr(ok_one);
  kk_vi_t start = create_kkvarint(INT32_MAX);

  for (int i = 0; i < INT32_MAX; i++)
  {
    kk_vi_t a = kkvarint_clone(accumulator);
    kk_vi_t b = kkvarint_clone(start);
    accumulator = add_borrowed_kkvarint_to_borrowed_kkvarint(a, b);

    // if (KK_VI_IS_BI(accumulator)){
    //   print_kkbigint_internal(kkvarint_as_kkbigint(accumulator));
    // }
  }
  char *ok = create_hexstr_from_borrowed_kkvarint(kkvarint_clone(accumulator));
  printf("%s\n", ok);
}

void test5()
{
  /*
  // 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
  // S  BI I  I  I  I  I

  int8_t test_one = 0x00;
  int8_t test_n_one = 0xFF;
  int8_t test_two = 0x3F;
  int8_t test_n_two = 0xC0;

  // plus : 0b00000000 - 0b00111111;
  // minus: 0b11111111 - 0b11000000;

  // 00 = plus
  // 11 = minus
  // 01 = bigint
  // 10 = overflow

  // a + a = b

  // (b + 0b01000000) & 0b10000000 = BIGINT

  // NO
  // 00 + 01 = 01
  // 11 + 01 = 00

  // YES
  // 01 + 01 = 10
  // 10 + 01 = 11

  int8_t new_one = 0x72;

  printf("2: %d\n", test_one);
  print_bits(&test_one, sizeof(test_one));
  printf("\n-2: %d\n", test_n_one);
  print_bits(&test_n_one, sizeof(test_n_one));
  printf("\nINT64_MAX: %d\n", test_two);
  print_bits(&test_two, sizeof(test_two));
  printf("\nINT64_MIN: %d\n", test_n_two);
  print_bits(&test_n_two, sizeof(test_n_two));
  printf("\n");
  */
}

void test6()
{
  int64_t test = 1;
  printf("%ld %lx\n", test, test);
  int64_t test2 = ~test + 1;
  printf("%ld %lx\n", test2, test2);

  kk_vi_t a = kkbigint_as_kkvarint(kkbigint_resize(create_kkbigint(1), 2));
  print_kkbigint_internal(kkvarint_as_kkbigint(a));
  a = kkbigint_as_kkvarint(kkbigint_resize(kkvarint_as_kkbigint(negate_borrowed_kkvarint(a)), 4));
  print_kkbigint_internal(kkvarint_as_kkbigint(a));
  a = negate_borrowed_kkvarint(a);
  print_kkbigint_internal(kkvarint_as_kkbigint(a));
}

void test7()
{
  printf("AAAA 1\n");

  kk_vi_t pos = kkbigint_as_kkvarint(create_kkbigint(1)); // 1
  kk_vi_t neg = negate_borrowed_kkvarint(kkvarint_clone(pos)); // -1

  print_kkvarint_internal(pos);
  print_kkvarint_internal(neg);

  kk_vi_t new = add_borrowed_kkvarint_to_borrowed_kkvarint(pos, neg); // 0
  printf("pog\n");
  fflush(stdout);
  print_kkvarint_internal(new);
  char *ok = create_hexstr_from_borrowed_kkvarint(new);
  printf("%s\n", ok);

  printf("AAAA 2\n");

  uint64_t aa = 0x3fffffffffffffff;
  uint64_t bb = 0x7fffffffffffffff;
  printf("Trying: %ld + %ld = %ld\n", aa, bb, aa + bb);

  kk_vi_t a = kkbigint_as_kkvarint(create_kkbigint(aa));
  kk_vi_t b = kkbigint_as_kkvarint(create_kkbigint(bb));

  print_kkvarint_internal(a);
  print_kkvarint_internal(b);

  kk_vi_t new2 = add_borrowed_kkvarint_to_borrowed_kkvarint(a, b);
  print_kkvarint_internal(new2);
  char *ok2 = create_hexstr_from_borrowed_kkvarint(kkvarint_clone(new2));
  printf("%s\n", ok2);
}

void test8(){
  kk_vi_t one = create_kkvarint_from_borrowed_hexstr(create_string("ffffffffffffffffffffffffffffffff"));
  kk_vi_t two = kkbigint_as_kkvarint(kkbigint_resize(create_kkbigint(1), 1));
  print_kkbigint_internal(kkvarint_as_kkbigint(one));
  print_kkbigint_internal(kkvarint_as_kkbigint(two));

  kk_vi_t new = add_borrowed_kkvarint_to_borrowed_kkvarint(one, two);

  print_kkbigint_internal(kkvarint_as_kkbigint(new));
}

int main()
{

  test7();

  return 0;
}
