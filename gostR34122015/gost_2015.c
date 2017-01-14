#include "gost_2015.h"

/*
 * realization of GOST R34.13-2015 using GOST R34.12-2015
*/

#include "generating_tables.h"

void gost_2015_set_key(const uint8_t password[], const uint8_t bits, gost_key *key)
{
    keySchedule(password, bits, key);
}

void gost_2015_ecb_encrypt(const uint8_t *in, const size_t in_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
    assert(key->bits == 64 || key->bits == 128);
    if(out == NULL)
    {
        *out_size = in_size + 2;
        *out_size = ((in_size + 2) / (key->bits >> 3) + 1) * (key->bits >> 3);
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
            printf("here\n");
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + i * BLOCK64SIZE, left_size, additional, _out_size);
            printB128(additional);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK64SIZE, key);
            printB128(additional + BLOCK64SIZE);
            printf("\n\n");
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
            printf("here\n");
            BLOCK256 additional = { 0 };
            size_t _out_size = BLOCK256SIZE;
            supplementation_02(in + i * BLOCK128SIZE, left_size, additional, _out_size);
            printB128(additional);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK128SIZE, key);
            printB128(additional + BLOCK128SIZE);
            printf("\n\n");
            gost_r3412_128_encrypt_block(additional + BLOCK128SIZE, out + ((i + 1) * BLOCK128SIZE), key);
        }
        else
        {
            BLOCK128 additional = { 0 };
            size_t _out_size = BLOCK128SIZE;
            supplementation_02(in + i * BLOCK128SIZE, left_size, additional, _out_size);
            gost_r3412_128_encrypt_block(additional, out + i * BLOCK128SIZE, key);
        }
    }
}
void gost_2015_ecb_decrypt(const uint8_t *in, const size_t in_size, uint8_t *out, size_t *out_size, const gost_key *key)
{
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

    memcpy_s(out + (out_size - in_size), out_size, in, in_size);
    if(out_size != in_size)
    {
        out[out_size - in_size - 1] = 0x01;
        memset(out, 0x00, out_size - in_size - 1);
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

