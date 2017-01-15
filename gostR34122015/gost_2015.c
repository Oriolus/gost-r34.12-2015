#include "gost_2015.h"

/*
 * realization of GOST R34.13-2015 using GOST R34.12-2015
*/

#include "generating_tables.h"

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
    memcpy_s(full, shift_size, lsb, shift_size);
}

void gost_2015_set_key(const uint8_t password[], const uint8_t bits, gost_key *key)
{
    keySchedule(password, bits, key);
}

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
        size_t i = 0;
        for(; i < in_size / BLOCK64SIZE; i++)
        {
            gost_r3412_128_encrypt_block(in + BLOCK64SIZE * i, out + BLOCK64SIZE * i, key);
        }
        size_t left_size = in_size % BLOCK64SIZE;
        if(left_size + 2 > BLOCK64SIZE)
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + i * BLOCK64SIZE, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK64SIZE, key);
            gost_r3412_128_encrypt_block(additional + BLOCK64SIZE, out + ((i + 1) * BLOCK64SIZE), key);
        }
        else
        {
            BLOCK64 additional = { 0 };
            size_t _out_size = BLOCK64SIZE;
            supplementation_02(in + i * BLOCK64SIZE, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK64SIZE, key);
        }
    }
    if(key->bits == 128)
    {
        size_t i = 0;
        for(; i < in_size / BLOCK128SIZE; i++)
        {
            gost_r3412_128_encrypt_block(in + BLOCK128SIZE * i, out + BLOCK128SIZE * i, key);
        }
        size_t left_size = in_size % BLOCK128SIZE;
        if(left_size + 2 > BLOCK128SIZE)
        {
            BLOCK256 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + i * BLOCK128SIZE, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK128SIZE, key);
            gost_r3412_128_encrypt_block(additional + BLOCK128SIZE, out + ((i + 1) * BLOCK128SIZE), key);

            memset(additional, 0x00, BLOCK256SIZE);
        }
        else
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + i * BLOCK128SIZE, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK128SIZE, key);

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
            gost_r3412_128_decrypt_block(in + BLOCK64SIZE * i, out + BLOCK64SIZE * i, key);
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

    }
    if(key->bits == 128)
    {
        uint8_t *_iv = (uint8_t*)malloc(iv_size);
        memcpy_s(_iv, iv_size, iv, iv_size);
        uint8_t * const iv_msd = _iv + iv_size - BLOCK128SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            X128(in + offset, iv_msd, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);
            shl(_iv, iv_size, BLOCK128SIZE, out + offset);
        }
        size_t left_size = in_size - offset;
        if(left_size + 2 > BLOCK128SIZE)
        {
            BLOCK256 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);

            X128(additional, iv_msd, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);
            shl(_iv, iv_size, BLOCK128SIZE, out + offset);

            offset += BLOCK128SIZE;
            X128(additional + BLOCK128SIZE, iv_msd, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);

            memset(additional, 0x00, BLOCK256SIZE);
        }
        else
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + offset, left_size, additional, _out_size);

            X128(additional, iv_msd, out + offset);
            gost_r3412_128_encrypt_block(out + offset, out + offset, key);

            memset(additional, 0x00, BLOCK128SIZE);
        }

        memset(_iv, 0x00, iv_size);
        free(_iv);
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

    }
    if(key->bits == 128)
    {
        uint8_t *_iv = (uint8_t*)malloc(iv_size);
        memcpy_s(_iv, iv_size, iv, iv_size);
        uint8_t * const iv_msb = _iv + iv_size - BLOCK128SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            gost_r3412_128_decrypt_block(in + offset, out + offset, key);
            X128(iv_msb, out + offset, out + offset);
            shl(_iv, iv_size, BLOCK128SIZE, in + offset);
        }

        memset(_iv, 0x00, iv_size);
        free(_iv);
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
        memcpy_s(_iv + BLOCK64SIZE, BLOCK64SIZE, iv, BLOCK64SIZE);
        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);
        for(; offset < max_offset ; offset += BLOCK64SIZE, ctr_inc(_iv))
        {
            gost_r3412_64_encrypt_block(_iv, out + offset, key);
            X128(in + offset, out + offset, out + offset);
        }
        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK64 last_in = { 0 };
            BLOCK64 last_out = { 0 };
            memcpy_s(last_in, BLOCK64SIZE, in + offset, left_size);

            gost_r3412_64_encrypt_block(_iv, last_out, key);
            X128(last_in, last_out, last_out);

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
    assert(in_size == *out_size);

    if(key->bits == 64)
    {
        uint8_t *_iv = (uint8_t*)malloc(iv_size);
        memcpy_s(_iv, iv_size, iv, iv_size);
        const uint8_t *iv_msd = _iv + iv_size - BLOCK64SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK64SIZE - 1);
        for(; offset < max_offset; offset += BLOCK64SIZE)
        {
            gost_r3412_128_encrypt_block(iv_msd, out + offset, key);
            shl(_iv, iv_size, BLOCK64SIZE, out + offset);
            X128(in + offset, out + offset, out + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK64 last_out = { 0 };
            BLOCK64 last_in = { 0 };
            memcpy_s(last_in, BLOCK64SIZE, in + offset, left_size);

            gost_r3412_128_encrypt_block(iv_msd, last_out, key);
            X128(last_in, last_out, last_out);
            memcpy_s(out + offset, left_size, last_out, left_size);

            memset(last_out, 0x00, BLOCK64SIZE);
            memset(last_in, 0x00, BLOCK64SIZE);
        }
        memset(_iv, 0x00, iv_size);
        free(_iv);
    }
    if(key->bits == 128)
    {
        uint8_t *_iv = (uint8_t*)malloc(iv_size);
        memcpy_s(_iv, iv_size, iv, iv_size);
        const uint8_t *iv_msd = _iv + iv_size - BLOCK128SIZE;

        size_t offset = 0;
        const size_t max_offset = in_size & ~(BLOCK128SIZE - 1);
        for(; offset < max_offset; offset += BLOCK128SIZE)
        {
            gost_r3412_128_encrypt_block(iv_msd, out + offset, key);
            shl(_iv, iv_size, BLOCK128SIZE, out + offset);
            X128(in + offset, out + offset, out + offset);
        }

        size_t left_size = in_size - offset;
        if(left_size != 0)
        {
            BLOCK128 last_out = { 0 };
            BLOCK128 last_in = { 0 };
            memcpy_s(last_in, BLOCK128SIZE, in + offset, left_size);

            gost_r3412_128_encrypt_block(iv_msd, last_out, key);
            X128(last_in, last_out, last_out);
            memcpy_s(out + offset, left_size, last_out, left_size);

            memset(last_out, 0x00, BLOCK128SIZE);
            memset(last_in, 0x00, BLOCK128SIZE);
        }
        memset(_iv, 0x00, iv_size);
        free(_iv);
    }
}
void gost_2015_ofd_decrypt(const uint8_t *in, const size_t in_size, const uint8_t *iv, const size_t iv_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    gost_2015_ofb_encrypt(in, in_size, iv, iv_size, out, out_size, key);
}
