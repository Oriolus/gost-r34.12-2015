#pragma once

#include <assert.h>
#include "r34122015_types.h"
#include "r34122015_core.h"

#ifdef __cplusplus
extern "C" {
#endif

void gost_2015_set_key(const uint8_t password[], const uint8_t bits, gost_key *key);

void gost_2015_ecb_encrypt(const uint8_t *in, const size_t in_size, uint8_t *out, size_t *out_size, const gost_key *key);
void gost_2015_ecb_decrypt(const uint8_t *in, const size_t in_size, uint8_t *out, size_t *out_size, const gost_key *key);

void supplementation_01(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);
void supplementation_02(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);
void supplementation_03(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size);

#ifdef __cplusplus
}
#endif
