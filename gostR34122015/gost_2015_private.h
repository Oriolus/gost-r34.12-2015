#pragma once

#include "r34122015_types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ctr_inc(BLOCK128 a);
void shl(uint8_t *full, const size_t full_size, const size_t shift_size, const uint8_t *lsb);

void supplementation_01(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);
void supplementation_02(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);
void supplementation_03(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);

#ifdef __cplusplus
}
#endif
