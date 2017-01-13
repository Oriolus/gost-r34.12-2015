#pragma once

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

#ifndef BLOCK32SIZE
#define BLOCK32SIZE 4
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

#define MODULO	451
#define HIGH11	0xFFE00000

#ifndef X32
#define X32(a0, a1, out) *(uint32_t*)out = *(uint32_t*)a0 ^ *(uint32_t*)a1
#endif

struct gost128_key_st
{
    uint8_t rd_key[GOST128_ROUND_KEYS_COUNT][GOST128_ROUND_KEY_SIZE];
};

struct gost64_key_st
{
    uint8_t rd_key[GOST64_ROUND_KEYS_COUNT][GOST64_ROUND_KEY_SIZE];
};

typedef struct gost128_key_st gost128_key;
typedef struct gost64_key_st gost64_key;

void X128(const BLOCK128 a, const BLOCK128 b, BLOCK128 out);
void S128(BLOCK128 a);
void R128(BLOCK128 a);
void L128(BLOCK128 a);
void S128inv(BLOCK128 a);
void R128inv(BLOCK128);
void L128inv(BLOCK128 a);

void F128(BLOCK128 a1, BLOCK128 a2, const BLOCK128 key);
void KeySchedule128(uint8_t pwd[32], gost128_key *key);

void gost_r3412_128_encrypt_block(const BLOCK128 in, BLOCK128 out, gost128_key *key);
void gost_r3415_128_decrypt_block(const BLOCK128 in, BLOCK128 out, gost128_key *key);

void T64(BLOCK32 a);
void g64(BLOCK32 a, const BLOCK32 round_key);
void G64(BLOCK32 a1, BLOCK32 a0, const BLOCK32 round_key);
void G64Star(BLOCK32 a1, BLOCK32 a0, const BLOCK32 round_key, BLOCK64 out);
void KeySchedule64(uint8_t pwd[32], gost64_key *key);

void gost_r3412_64_encrypt(const BLOCK64 in, BLOCK64 out, gost64_key *key);
void gost_r3412_64_decrypt(const BLOCK64 in, BLOCK64 out, gost64_key *key);
