#pragma once

#include "r34122015_types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define last_macB_128 0x87
#define last_macB_64  0x1B

const BLOCK128 macB_128 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, last_macB_128 };
const BLOCK64 macB_64 = {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, last_macB_64 };

void ctr_inc(BLOCK128 a);
void shl(uint8_t *full, const size_t full_size, const size_t shift_size, const uint8_t *lsb);

void supplementation_01(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);
void supplementation_02(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);
void supplementation_03(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);

void shl_1(uint8_t *a, const size_t a_size);
void shl128_1(BLOCK128 a);
void shl64_1(BLOCK64 a);

#ifdef __cplusplus
}
#endif
