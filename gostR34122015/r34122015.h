#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef BLOCK128SIZE
#define BLOCK128SIZE 16
#endif

#ifndef BLOCK64SIZE
#define BLOCK64SIZE 8
#endif

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

void X128(const BLOCK128 a, const BLOCK128 b, BLOCK128 out);
void S128(BLOCK128 a);
void R128(BLOCK128 a0);
void L128(BLOCK128 a0);
void F128(BLOCK128 a1, BLOCK128 a2, const BLOCK128 k);
void S128inv(BLOCK128 a);
void R128inv(BLOCK128);
void L128inv(BLOCK128 a0);
void KeySchedule128(uint8_t pwd[32]);
void E128(BLOCK128 a);
void D128(BLOCK128 a);

uint8_t pi128[256];
uint8_t invPi128[256];
uint8_t coeffL128[BLOCK128SIZE];
uint8_t C128[32][16];
uint8_t K128[10][BLOCK128SIZE];
uint8_t GM[8][256];
uint8_t K_POS[256];

void T64(BLOCK32 a0);
void g64(BLOCK32 a0, const BLOCK32 ki);
void G64(BLOCK32 a1, BLOCK32 a0, const BLOCK32 ki);
void G64Star(BLOCK32 a1, BLOCK32 a0, const BLOCK32 ki, BLOCK64 out);
void KeySchedule64(uint8_t pwd[32]);
void E64(BLOCK64 a);
void D64(BLOCK64 a);

uint8_t pi64[4][256];
uint8_t K64[32][4];