#include "r34122015_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HIGH11	0xFFE00000

#ifndef X32
#define X32(a0, a1, out) *(uint32_t*)out = *(uint32_t*)a0 ^ *(uint32_t*)a1
#endif

void keySchedule(const uint8_t pwd[32], const uint8_t bits, gost_key *key);

void gost_r3412_128_encrypt_block(const BLOCK128 in, BLOCK128 out, const gost_key *key);
void gost_r3412_128_decrypt_block(const BLOCK128 in, BLOCK128 out, const gost_key *key);

void gost_r3412_64_encrypt_block(const BLOCK64 in, BLOCK64 out, const gost_key *key);
void gost_r3412_64_decrypt_block(const BLOCK64 in, BLOCK64 out, const gost_key *key);

void X128(const BLOCK128 a, const BLOCK128 b, BLOCK128 out);
void S128(BLOCK128 a);
void R128(BLOCK128 a);
void L128(BLOCK128 a);
void S128inv(BLOCK128 a);
void R128inv(BLOCK128);
void L128inv(BLOCK128 a);

void F128(BLOCK128 a1, BLOCK128 a2, const BLOCK128 key);

void T64(BLOCK32 a);
void g64(BLOCK32 a, const BLOCK32 round_key);
void G64(BLOCK32 a1, BLOCK32 a0, const BLOCK32 round_key);
void G64Star(BLOCK32 a1, BLOCK32 a0, const BLOCK32 round_key, BLOCK64 out);

#ifdef __cplusplus
}
#endif
