#include "r34122015_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void keySchedule(const uint8_t pwd[32], const uint8_t bits, gost_key *key);

void gost_r3412_128_encrypt_block(const BLOCK128 in, BLOCK128 out, const gost_key *key);
void gost_r3412_128_decrypt_block(const BLOCK128 in, BLOCK128 out, const gost_key *key);

void gost_r3412_64_encrypt_block(const BLOCK64 in, BLOCK64 out, const gost_key *key);
void gost_r3412_64_decrypt_block(const BLOCK64 in, BLOCK64 out, const gost_key *key);

#ifdef __cplusplus
}
#endif
