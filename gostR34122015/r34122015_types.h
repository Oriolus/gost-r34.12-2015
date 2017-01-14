#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BLOCK128SIZE                16
#define GOST128_ROUNDS_COUNT        10
#define GOST128_ROUND_KEYS_COUNT    10
#define GOST128_ROUND_KEY_SIZE      16

#define BLOCK64SIZE                  8
#define GOST64_ROUNDS_COUNT         32
#define GOST64_ROUND_KEYS_COUNT     32
#define GOST64_ROUND_KEY_SIZE        4

#define BLOCK32SIZE                  4
#define BLOCK256SIZE                32

#ifndef __BLOCK256
#define __BLOCK256
typedef uint8_t BLOCK256[BLOCK256SIZE];
#endif

#ifndef __BLOCK128
#define __BLOCK128
typedef uint8_t BLOCK128[BLOCK128SIZE];
#endif

#ifndef __BLOCK64
#define __BLOCK64
typedef uint8_t BLOCK64[BLOCK64SIZE];
#endif

#ifndef __BLOCK32
#define __BLOCK32
typedef uint8_t BLOCK32[BLOCK32SIZE];
#endif

struct gost_key_st
{
    uint8_t rd_key_128[GOST128_ROUND_KEYS_COUNT][GOST128_ROUND_KEY_SIZE];
    uint8_t rd_key_64[GOST64_ROUND_KEYS_COUNT][GOST64_ROUND_KEY_SIZE];
    uint8_t bits;
};

typedef struct gost_key_st gost_key;

#ifdef __cplusplus
}
#endif
