#include "stdio.h"

void print_bits(void *data, size_t length)
{
  unsigned char *p = data;

  p += length - 1;

  for (size_t i = 0; i < length; i++)
  {
    for (char j = 7; j >= 0; j--)
    {
      printf("%d", !!((*p) & (1 << j)));
    }
    p--;
  }
}

int main()
{
  int a = 1;
  int b = -1;
  long aa = 1;
  long bb = -1;
  long long aaa = 1;
  long long bbb = -1;

  char test = 256 / 2 + 1;
  print_bits(&aaa, sizeof(aaa));

  return 0;
}