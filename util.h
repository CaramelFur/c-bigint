#include <stdint.h>
#include <stdlib.h>

#pragma once

#define CEIL_DIV(a, b) (1 + ((a - 1) / b))

static inline uint8_t calculate_bitparts_needs(size_t number, uint8_t bitpartsize)
{
  uint8_t i = 0;
  while (number > 0)
  {
    i++;
    number >>= bitpartsize;
  }
  return i;
}

static inline uint8_t hex2nibble(char hex)
{
  if (hex >= '0' && hex <= '9')
    return (uint8_t)(hex - '0');
  if (hex >= 'A' && hex <= 'F')
    return (uint8_t)(hex - 'A' + 10);
  if (hex >= 'a' && hex <= 'f')
    return (uint8_t)(hex - 'a' + 10);
  return 0;
}

static inline char nibble2hex(uint8_t nibble)
{
  if (nibble < 10)
    return (char)(nibble + '0');
  return (char)(nibble - 10 + 'A');
}

static inline size_t hexs_for_n_bits(size_t amount_bits)
{
  return amount_bits / 4;
}

static inline size_t decimals_for_n_bits(size_t amount_bits)
{
  // The precise solution is `amount_bits * (log2/log10)`
  // Here `log2/log10` =~= `0.301`
  // We dont want to use floats so we'll divide instead
  // So: `amount_bits / (1 / 0.301)`
  //     `amount_bits / 3.32`
  // We still need floats, so we'll round down, 
  // this will give us too many digits, but it is close enough
  return amount_bits / 3;
}

static inline size_t bcds_for_n_bits(size_t amount_bits) {
  return CEIL_DIV(amount_bits + 4 * CEIL_DIV(amount_bits, 3), 8);
}

void print_bits(void *data, size_t length);
uint8_t hex2byte(char *hexpair);