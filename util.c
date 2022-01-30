#include "util.h"

#include <stdio.h>

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

uint8_t hex2byte(char *hexpair)
{
  return (uint8_t)(hex2nibble(hexpair[0]) << 4) | hex2nibble(hexpair[1]);
}

