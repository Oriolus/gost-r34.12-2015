#include "gost_2015.h"
#include "r34122015_core.h"
#include "gost_2015_private.h"
#include "r34122015_core_private.h"

/*
 * realization of GOST R34.13-2015 using GOST R34.12-2015
 * fix this
 * - add iv_size check
 * - add incoming pointers check
*/

#define X64(a, b, out) (*(uint64_t*)out = *(uint64_t*)a ^ *(uint64_t*)b)

#include "generating_tables.h"

void gost_2015_ecb_encrypt(const uint8_t *in, const size_t in_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        if((in_size + 2) % key->bits == 0) *out_size = in_size + 2;
        else *out_size = ((in_size + 2) / (key->bits >> 3) + 1) * (key->bits >> 3);
        return;
    }
    assert((*out_size % (key->bits >> 3)) == 0);
    assert(in_size + 2 < *out_size);

    if(key->bits == 64)
    {
        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);

        for(; offset < max_offset; offset += BLOCK64SIZE)
        {
            gost_r3412_64_encrypt_block(in + offset, out + offset, key);
        }

        size_t left_size = in_size % BLOCK64SIZE;
        if(left_size + 2 > BLOCK64SIZE)
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);
            gost_r3412_64_encrypt_block(additional, out + offset, key);
            gost_r3412_64_encrypt_block(additional + BLOCK64SIZE, out + (offset + BLOCK64SIZE), key);
        }
        else
        {
            BLOCK64 additional = { 0 };
            size_t _out_size = BLOCK64SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);
            gost_r3412_64_encrypt_block(additional, out + offset, key);
        }
    }
    if(key->bits == 128)
    {
        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);

        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            gost_r3412_128_encrypt_block(in + offset, out + offset, key);
        }

        size_t left_size = in_size % BLOCK128SIZE;
        if(left_size + 2 > BLOCK128SIZE)
        {
            BLOCK256 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + offset, key);
            gost_r3412_128_encrypt_block(additional + BLOCK128SIZE, out + (offset + BLOCK128SIZE), key);

            memset(additional, 0x00, BLOCK256SIZE);
        }
        else
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + offset, key);

            memset(additional, 0x00, BLOCK128SIZE);
        }
    }
}
void gost_2015_ecb_decrypt(const uint8_t *in, const size_t in_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        *out_size = in_size;
        return;
    }
    if(key->bits == 64)
    {
        assert(in_size % BLOCK64SIZE == 0);
        for(size_t i = 0; i < in_size / BLOCK64SIZE; i++)
        {
            gost_r3412_64_decrypt_block(in + BLOCK64SIZE * i, out + BLOCK64SIZE * i, key);
        }
    }
    if(key->bits == 128)
    {
        assert(in_size % BLOCK128SIZE == 0);
        for(size_t i = 0; i < in_size / BLOCK128SIZE; i++)
        {
            gost_r3412_128_decrypt_block(in + BLOCK128SIZE * i, out + BLOCK128SIZE * i, key);
        }
    }
}

void gost_2015_cbc_encrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        if((in_size + 2) % (key->bits >> 3) == 0) *out_size = in_size + 2;
        else *out_size = ((in_size + 2) / (key->bits >> 3) + 1) * (key->bits >> 3);
        return;
    }
    assert((*out_size % (key->bits >> 3)) == 0);
    assert(in_size + 2 <= *out_size);

    if(key->bits == 64)
    {
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb = lsr + iv_size - BLOCK64SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);
        for(; offset < max_offset; offset += BLOCK64SIZE)
        {
            X64((in + offset), lsr_msb, (out + offset));
            gost_r3412_64_encrypt_block(out + offset, out + offset, key);
            shl(lsr, iv_size, BLOCK64SIZE, out + offset);
        }
        size_t left_size = in_size - offset;
        if(left_size + 2 > BLOCK64SIZE)
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);

            X64(additional, lsr_msb, (out + offset));
            gost_r3412_64_encrypt_block(out + offset, out + offset, key);
            shl(lsr, iv_size, BLOCK64SIZE, out + offset);

            offset += BLOCK64SIZE;
            X64((additional + BLOCK64SIZE), lsr_msb, (out + offset));
            gost_r3412_64_encrypt_block(out + offset, out + offset, key);

            memset(additional, 0x00, BLOCK128SIZE);
        }
        else
        {
            BLOCK64 additional = { 0 };
            size_t _out_size = BLOCK64SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);

            X64(additional, lsr_msb, (out + offset));
            gost_r3412_64_encrypt_block(out + offset, out + offset, key);

            memset(additional, 0x00, BLOCK64SIZE);
        }

        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
    if(key->bits == 128)
    {
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb = lsr + iv_size - BLOCK128SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            X128(in + offset, lsr_msb, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);
            shl(lsr, iv_size, BLOCK128SIZE, out + offset);
        }
        size_t left_size = in_size - offset;
        if(left_size + 2 > BLOCK128SIZE)
        {
            BLOCK256 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);

            X128(additional, lsr_msb, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);
            shl(lsr, iv_size, BLOCK128SIZE, out + offset);

            offset += BLOCK128SIZE;
            X128(additional + BLOCK128SIZE, lsr_msb, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);

            memset(additional, 0x00, BLOCK256SIZE);
        }
        else
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);

            X128(additional, lsr_msb, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);

            memset(additional, 0x00, BLOCK128SIZE);
        }

        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
}
void gost_2015_cbc_decrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        *out_size = in_size;
        return;
    }

    if(key->bits == 64)
    {
        assert(in_size % BLOCK64SIZE == 0);
        uint8_t *lsr = (uint8_t*)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb = lsr + iv_size - BLOCK64SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);
        for(; offset < max_offset; offset += BLOCK64SIZE)
        {
            gost_r3412_64_decrypt_block(in + offset, out + offset, key);
            X64(lsr_msb, (out + offset), (out + offset));
            shl(lsr, iv_size, BLOCK64SIZE, in + offset);
        }

        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
    if(key->bits == 128)
    {
        assert(in_size % BLOCK128SIZE == 0);
        uint8_t *lsr = (uint8_t*)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb = lsr + iv_size - BLOCK128SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            gost_r3412_128_decrypt_block(in + offset, out + offset, key);
            X128(lsr_msb, out + offset, out + offset);
            shl(lsr, iv_size, BLOCK128SIZE, in + offset);
        }

        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
}

void gost_2015_ctr_encrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        *out_size = in_size;
        return;
    }
    assert(in_size == *out_size);

    if(key->bits == 64)
    {
        assert(iv_size >= BLOCK32SIZE);
        BLOCK64 _iv = { 0 };
        memcpy_s(_iv + BLOCK32SIZE, BLOCK32SIZE, iv, BLOCK32SIZE);
        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);
        for(; offset < max_offset ; offset += BLOCK64SIZE, ctr_inc(_iv))
        {
            gost_r3412_64_encrypt_block(_iv, out + offset, key);
            X64((in + offset), (out + offset), (out + offset));
        }
        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK64 last_in = { 0 };
            BLOCK64 last_out = { 0 };
            memcpy_s(last_in, BLOCK64SIZE, in + offset, left_size);

            gost_r3412_64_encrypt_block(_iv, last_out, key);
            X64(last_in, last_out, last_out);

            memcpy_s(out + offset, left_size, last_out, left_size);
            memset(last_in, 0x00, BLOCK64SIZE);
            memset(last_out, 0x00, BLOCK64SIZE);
        }
        memset(_iv, 0x00, BLOCK64SIZE);
    }
    if(key->bits == 128)
    {
        assert(iv_size >= BLOCK64SIZE);
        BLOCK128 _iv = { 0 };
        memcpy_s(_iv + BLOCK64SIZE, BLOCK64SIZE, iv, BLOCK64SIZE);
        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset ; offset += BLOCK128SIZE, ctr_inc(_iv))
        {
            gost_r3412_128_encrypt_block(_iv, out + offset, key);
            X128(in + offset, out + offset, out + offset);
        }
        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK128 last_in = { 0 };
            BLOCK128 last_out = { 0 };
            memcpy_s(last_in, BLOCK128SIZE, in + offset, left_size);

            gost_r3412_128_encrypt_block(_iv, last_out, key);
            X128(last_in, last_out, last_out);

            memcpy_s(out + offset, left_size, last_out, left_size);
            memset(last_in, 0x00, BLOCK128SIZE);
            memset(last_out, 0x00, BLOCK128SIZE);
        }
        memset(_iv, 0x00, BLOCK128SIZE);
    }
}
void gost_2015_ctr_decrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    gost_2015_ctr_encrypt(in, in_size, iv, iv_size, out, out_size, key);
}

void gost_2015_ofb_encrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        *out_size = in_size;
        return;
    }
    assert(in_size <= *out_size);

    if(key->bits == (BLOCK64SIZE << 3))
    {
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb = lsr + iv_size - BLOCK64SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);
        for(; offset < max_offset; offset += BLOCK64SIZE)
        {
            gost_r3412_64_encrypt_block(lsr_msb, out + offset, key);
            shl(lsr, iv_size, BLOCK64SIZE, out + offset);
            X64((in + offset), (out + offset), (out + offset));
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK64 last_out = { 0 };
            BLOCK64 last_in = { 0 };
            memcpy_s(last_in, BLOCK64SIZE, in + offset, left_size);

            gost_r3412_64_encrypt_block(lsr_msb, last_out, key);
            X64(last_in, last_out, last_out);
            memcpy_s(out + offset, left_size, last_out, left_size);

            memset(last_out, 0x00, BLOCK64SIZE);
            memset(last_in, 0x00, BLOCK64SIZE);
        }
        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
    if(key->bits == (BLOCK128SIZE << 3))
    {
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb = lsr + iv_size - BLOCK128SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            gost_r3412_128_encrypt_block(lsr_msb, out + offset, key);
            shl(lsr, iv_size, BLOCK128SIZE, out + offset);
            X128(in + offset, out + offset, out + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK128 last_out = { 0 };
            BLOCK128 last_in = { 0 };
            memcpy_s(last_in, BLOCK128SIZE, in + offset, left_size);

            gost_r3412_128_encrypt_block(lsr_msb, last_out, key);
            X128(last_in, last_out, last_out);
            memcpy_s(out + offset, left_size, last_out, left_size);

            memset(last_out, 0x00, BLOCK128SIZE);
            memset(last_in, 0x00, BLOCK128SIZE);
        }
        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
}
void gost_2015_ofd_decrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    gost_2015_ofb_encrypt(in, in_size, iv, iv_size, out, out_size, key);
}

void gost_2015_cfb_encrypt(const uint8_t *in, const size_t in_size,
                           const uint8_t *iv, const size_t iv_size,
                           uint8_t *out, size_t *out_size,
                           const gost_key *key, const size_t *ciphertext_block_size)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        *out_size = in_size;
        return;
    }
    assert(in_size <= *out_size);
    const size_t block_size = ciphertext_block_size == NULL ? (key->bits >> 3) : *ciphertext_block_size;

    if(key->bits == (BLOCK64SIZE << 3))
    {
        assert(0 < block_size && block_size <= BLOCK64SIZE);
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb_n = lsr + iv_size - BLOCK64SIZE;
        BLOCK64 encrypted = { 0 };
        uint8_t * const enc_msb = encrypted + BLOCK64SIZE - block_size;

        const size_t max_offset = block_size * (in_size / block_size);
        size_t offset = 0;

        for(; offset < max_offset; offset += block_size)
        {
            gost_r3412_64_encrypt_block(lsr_msb_n, encrypted, key);
            X64((in + offset), encrypted, encrypted);
            memcpy_s(out + offset, block_size, enc_msb, block_size);
            shl(lsr, iv_size, block_size, out + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK64 last_in = { 0 };
            memcpy_s(last_in + BLOCK64SIZE - left_size, left_size, in + offset, left_size);
            gost_r3412_64_encrypt_block(lsr_msb_n, encrypted, key);
            X64(last_in, encrypted, encrypted);
            memcpy_s(out + offset, left_size, encrypted + BLOCK64SIZE - left_size, left_size);

            memset(last_in, 0x00, BLOCK64SIZE);
        }

        memset(encrypted, 0x00, BLOCK64SIZE);
        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
    if(key->bits == (BLOCK128SIZE << 3))
    {
        assert(0 < block_size && block_size <= BLOCK128SIZE);
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb_n = lsr + iv_size - BLOCK128SIZE;
        BLOCK128 encrypted = { 0 };
        uint8_t * const enc_msb = encrypted + BLOCK128SIZE - block_size;

        const size_t max_offset = block_size * (in_size / block_size);
        size_t offset = 0;

        for(; offset < max_offset; offset += block_size)
        {
            gost_r3412_128_encrypt_block(lsr_msb_n, encrypted, key);
            X128(in + offset, encrypted, encrypted);
            memcpy_s(out + offset, block_size, enc_msb, block_size);
            shl(lsr, iv_size, block_size, out + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK128 last_in = { 0 };
            memcpy_s(last_in + BLOCK128SIZE - left_size, left_size, in + offset, left_size);
            gost_r3412_128_encrypt_block(lsr_msb_n, encrypted, key);
            X128(last_in, encrypted, encrypted);
            memcpy_s(out + offset, left_size, encrypted + BLOCK128SIZE - left_size, left_size);

            memset(last_in, 0x00, BLOCK128SIZE);
        }

        memset(encrypted, 0x00, BLOCK128SIZE);
        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
}

void gost_2015_cfb_decrypt(const uint8_t *in, const size_t in_size,
                           const uint8_t *iv, const size_t iv_size,
                           uint8_t *out, size_t *out_size,
                           const gost_key *key, const size_t *ciphertext_block_size)
{
    assert(key->bits == (BLOCK64SIZE << 3) || key->bits == (BLOCK128SIZE << 3));
    if(out == NULL)
    {
        *out_size = in_size;
        return;
    }
    assert(in_size <= *out_size);
    const size_t block_size = ciphertext_block_size == NULL ? (key->bits >> 3) : *ciphertext_block_size;

    if(key->bits == (BLOCK64SIZE << 3))
    {
        assert(0 < block_size && block_size <= BLOCK64SIZE);
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb_n = lsr + iv_size - BLOCK64SIZE;
        BLOCK64 encrypted = { 0 };
        uint8_t * const enc_msb = encrypted + BLOCK64SIZE - block_size;

        const size_t max_offset = block_size * (in_size / block_size);
        size_t offset = 0;

        for(; offset < max_offset; offset += block_size)
        {
            gost_r3412_64_encrypt_block(lsr_msb_n, encrypted, key);
            X128(in + offset, encrypted, encrypted);
            memcpy_s(out + offset, block_size, enc_msb, block_size);
            shl(lsr, iv_size, block_size, in + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK64 last_in = { 0 };
            memcpy_s(last_in + BLOCK64SIZE - left_size, left_size, in + offset, left_size);
            gost_r3412_64_encrypt_block(lsr_msb_n, encrypted, key);
            X128(last_in, encrypted, encrypted);
            memcpy_s(out + offset, left_size, encrypted + BLOCK64SIZE - left_size, left_size);

            memset(last_in, 0x00, BLOCK64SIZE);
        }

        memset(encrypted, 0x00, BLOCK64SIZE);
        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
    if(key->bits == (BLOCK128SIZE << 3))
    {
        assert(0 < block_size && block_size <= BLOCK128SIZE);
        uint8_t * const lsr = (uint8_t * const)malloc(iv_size);
        memcpy_s(lsr, iv_size, iv, iv_size);
        uint8_t * const lsr_msb_n = lsr + iv_size - BLOCK128SIZE;
        BLOCK128 encrypted = { 0 };
        uint8_t * const enc_msb = encrypted + BLOCK128SIZE - block_size;

        const size_t max_offset = block_size * (in_size / block_size);
        size_t offset = 0;

        for(; offset < max_offset; offset += block_size)
        {
            gost_r3412_128_encrypt_block(lsr_msb_n, encrypted, key);
            X128(in + offset, encrypted, encrypted);
            memcpy_s(out + offset, block_size, enc_msb, block_size);
            shl(lsr, iv_size, block_size, in + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK128 last_in = { 0 };
            memcpy_s(last_in + BLOCK128SIZE - left_size, left_size, in + offset, left_size);
            gost_r3412_128_encrypt_block(lsr_msb_n, encrypted, key);
            X128(last_in, encrypted, encrypted);
            memcpy_s(out + offset, left_size, encrypted + BLOCK128SIZE - left_size, left_size);

            memset(last_in, 0x00, BLOCK128SIZE);
        }

        memset(encrypted, 0x00, BLOCK128SIZE);
        memset(lsr, 0x00, iv_size);
        free(lsr);
    }
}

void gost_2015_mac(const uint8_t *in, const size_t in_size, uint8_t *mac, size_t *mac_size, const gost_key *key)
{
    assert(key->bits == (BLOCK128SIZE << 3) || key->bits == (BLOCK64SIZE << 3));
    if(mac == NULL && *mac_size == 0)
    {
        *mac_size = key->bits >> 3;
        return;
    }
    assert(*mac_size <= (key->bits >> 3));

    if(key->bits == (BLOCK64SIZE << 3))
    {
        size_t offset = 0;
        const size_t max_offset = in_size & (BLOCK64SIZE - 1) ? (in_size & ~(BLOCK64SIZE - 1)) : in_size - BLOCK64SIZE;
        BLOCK64 encrypted = { 0 };
        BLOCK64 K = { 0 };
        gost_r3412_64_encrypt_block(encrypted, K, key);

        if(offset < max_offset)
        {
            gost_r3412_64_encrypt_block(in, encrypted, key);
            offset = BLOCK64SIZE;
        }
        for(; offset < max_offset; offset += BLOCK64SIZE)
        {
            X64((in + offset), encrypted, encrypted);
            gost_r3412_64_encrypt_block(encrypted, encrypted, key);
        }

        uint8_t k_msb = K[BLOCK64SIZE - 1] >> 7;
        shl64_1(K);
        if(k_msb) K[0] ^= last_macB_128;
        k_msb = 0x00;

        size_t left_size = in_size - offset;
        if(left_size == BLOCK64SIZE)
        {
            X64(encrypted, (in + offset), encrypted);
            X64(encrypted, K, encrypted);
            gost_r3412_64_encrypt_block(encrypted, encrypted, key);
            memcpy_s(mac, *mac_size, encrypted + (BLOCK64SIZE - *mac_size), *mac_size);
        }
        else
        {
            uint8_t k1_msb = K[BLOCK64SIZE - 1] >> 7;
            shl64_1(K);
            if(k1_msb) K[0] ^= last_macB_128;
            k1_msb = 0x00;

            if(left_size + 2 > BLOCK64SIZE)
            {
                BLOCK128 in_last = { 0 };
                supplementation_02(in + offset, left_size, in_last, BLOCK128SIZE);

                X64(encrypted, in_last, encrypted);
                gost_r3412_128_encrypt_block(encrypted, encrypted, key);

                X64(encrypted, in_last + BLOCK64SIZE, encrypted);
                X64(encrypted, K, encrypted);
                gost_r3412_64_encrypt_block(encrypted, encrypted, key);
                memcpy_s(mac, *mac_size, encrypted + BLOCK64SIZE - *mac_size, *mac_size);

                memset(in_last, 0x00, BLOCK128SIZE);
            }
            else
            {
                BLOCK64 in_last = { 0 };
                supplementation_02(in + offset, left_size, in_last, BLOCK64SIZE);

                X64(encrypted, in_last, encrypted);
                X64(encrypted, K, encrypted);
                gost_r3412_64_encrypt_block(encrypted, encrypted, key);
                memcpy_s(mac, *mac_size, encrypted + BLOCK64SIZE - *mac_size, *mac_size);

                memset(in_last, 0x00, BLOCK64SIZE);
            }
        }

        memset(encrypted, 0x00, BLOCK64SIZE);
        memset(K, 0x00, BLOCK64SIZE);
    }
    if(key->bits == (BLOCK128SIZE << 3))
    {
        size_t offset = 0;
        const size_t max_offset = in_size & (BLOCK128SIZE - 1) ? (in_size & ~(BLOCK128SIZE - 1)) : in_size - BLOCK128SIZE;
        BLOCK128 encrypted = { 0 };
        BLOCK128 K = { 0 };
        gost_r3412_128_encrypt_block(encrypted, K, key);

        if(offset < max_offset)
        {
            gost_r3412_128_encrypt_block(in, encrypted, key);
            offset = BLOCK128SIZE;
        }
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            X128(in + offset, encrypted, encrypted);
            gost_r3412_128_encrypt_block(encrypted, encrypted, key);
        }

        uint8_t k_msb = K[BLOCK128SIZE - 1] >> 7;
        shl128_1(K);
        if(k_msb) K[0] ^= last_macB_128;
        k_msb = 0x00;

        size_t left_size = in_size - offset;
        if(left_size == BLOCK128SIZE)
        {
            X128(encrypted, in + offset, encrypted);
            X128(encrypted, K, encrypted);
            gost_r3412_128_encrypt_block(encrypted, encrypted, key);
            memcpy_s(mac, *mac_size, encrypted + (BLOCK128SIZE - *mac_size), *mac_size);
        }
        else
        {
            uint8_t k1_msb = K[BLOCK128SIZE - 1] >> 7;
            shl128_1(K);
            if(k1_msb) K[0] ^= last_macB_128;
            k1_msb = 0x00;

            if(left_size + 2 > BLOCK128SIZE)
            {
                BLOCK256 in_last = { 0 };
                supplementation_02(in + offset, left_size, in_last, BLOCK256SIZE);

                X128(encrypted, in_last, encrypted);
                gost_r3412_128_encrypt_block(encrypted, encrypted, key);

                X128(encrypted, in_last + BLOCK128SIZE, encrypted);
                X128(encrypted, K, encrypted);
                gost_r3412_128_encrypt_block(encrypted, encrypted, key);
                memcpy_s(mac, *mac_size, encrypted + BLOCK128SIZE - *mac_size, *mac_size);

                memset(in_last, 0x00, BLOCK256SIZE);
            }
            else
            {
                BLOCK128 in_last = { 0 };
                supplementation_02(in + offset, left_size, in_last, BLOCK128SIZE);

                X128(encrypted, in_last, encrypted);
                X128(encrypted, K, encrypted);
                gost_r3412_128_encrypt_block(encrypted, encrypted, key);
                memcpy_s(mac, *mac_size, encrypted + BLOCK128SIZE - *mac_size, *mac_size);

                memset(in_last, 0x00, BLOCK128SIZE);
            }
        }

        memset(encrypted, 0x00, BLOCK128SIZE);
        memset(K, 0x00, BLOCK128SIZE);
    }

}

void supplementation_01(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size)
{
    assert(in_size <= out_size);
    assert(out_size == BLOCK64SIZE || out_size == BLOCK128SIZE);
    memcpy_s(out + (out_size - in_size), out_size, in, in_size);
    memset(out, 0x00, out_size - in_size);
}
void supplementation_02(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size)
{
    assert(out_size == BLOCK64SIZE || out_size == BLOCK128SIZE || out_size == BLOCK256SIZE);

    if(out_size == BLOCK64SIZE)
        assert(in_size + 2 <= BLOCK64SIZE);
    if(out_size == BLOCK128SIZE)
        assert(in_size + 2 <= BLOCK128SIZE);
    if(out_size == BLOCK256SIZE)
        assert(in_size + 2 <= BLOCK256SIZE);

    memcpy_s(out, out_size, in, in_size);
    if(out_size != in_size)
    {
        out[in_size] = 0x01;
        memset(out + in_size + 1, 0x00, out_size - in_size - 1);
    }
}
void supplementation_03(const uint8_t *in, const size_t in_size, uint8_t *out, const size_t out_size)
{
    assert(in_size <= out_size);
    assert(out_size == BLOCK64SIZE || out_size == BLOCK128SIZE);
    memcpy_s(out + (out_size - in_size), out_size, in, in_size);
    if(in_size != out_size)
    {
        out[out_size - in_size - 1] = 0x01;
        memset(out, 0x00, out_size - in_size - 1);
    }
}

void shl_1(uint8_t *a, const size_t a_size)
{
    assert(a);
    uint16_t carry = 0;
    for(size_t i = 0; i < a_size; i++)
    {
        a[i] = (uint8_t)(carry = (a[i] << 1 | (carry >> 8)));
    }
}
void shl128_1(BLOCK128 a)
{
    assert(a);
    uint64_t *_a = (uint64_t*)a;
    uint64_t carry = *_a >> 63;
    *_a++ = *_a << 1;
    *_a = (*_a << 1) | carry;
    carry = 0;
    _a = NULL;
}
void shl64_1(BLOCK64 a)
{
    assert(a);
    uint64_t *_a = (uint64_t*)a;
    *_a = *_a << 1;
    _a = NULL;
}

void ctr_inc(BLOCK128 a)
{
    uint16_t carry = (1 << 8);
    for(size_t i = 0; i < BLOCK128SIZE; i++)
    {
        a[i] = (uint8_t)(carry = (uint16_t)a[i] + (carry >> 8));
    }
}
void shl(uint8_t *full, const size_t full_size, const size_t shift_size, const uint8_t *lsb)
{
    assert(full_size >= shift_size);
    memmove_s(full + shift_size, full_size - shift_size, full, full_size - shift_size);
    memcpy_s(full, full_size, lsb, shift_size);
}

void gost_2015_set_key(const uint8_t password[], const uint8_t bits, gost_key *key)
{
    keySchedule(password, bits, key);
}

