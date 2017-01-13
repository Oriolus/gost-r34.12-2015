#include "r34122015.h"

static const uint8_t pi64[4][256] =
{
	{ 
		0x6C, 0x64, 0x66, 0x62, 0x6A, 0x65, 0x6B, 0x69, 0x6E, 0x68, 0x6D, 0x67, 0x60, 0x63, 0x6F, 0x61,
		0x8C, 0x84, 0x86, 0x82, 0x8A, 0x85, 0x8B, 0x89, 0x8E, 0x88, 0x8D, 0x87, 0x80, 0x83, 0x8F, 0x81,
		0x2C, 0x24, 0x26, 0x22, 0x2A, 0x25, 0x2B, 0x29, 0x2E, 0x28, 0x2D, 0x27, 0x20, 0x23, 0x2F, 0x21,
		0x3C, 0x34, 0x36, 0x32, 0x3A, 0x35, 0x3B, 0x39, 0x3E, 0x38, 0x3D, 0x37, 0x30, 0x33, 0x3F, 0x31,
		0x9C, 0x94, 0x96, 0x92, 0x9A, 0x95, 0x9B, 0x99, 0x9E, 0x98, 0x9D, 0x97, 0x90, 0x93, 0x9F, 0x91,
		0xAC, 0xA4, 0xA6, 0xA2, 0xAA, 0xA5, 0xAB, 0xA9, 0xAE, 0xA8, 0xAD, 0xA7, 0xA0, 0xA3, 0xAF, 0xA1,
		0x5C, 0x54, 0x56, 0x52, 0x5A, 0x55, 0x5B, 0x59, 0x5E, 0x58, 0x5D, 0x57, 0x50, 0x53, 0x5F, 0x51,
		0xCC, 0xC4, 0xC6, 0xC2, 0xCA, 0xC5, 0xCB, 0xC9, 0xCE, 0xC8, 0xCD, 0xC7, 0xC0, 0xC3, 0xCF, 0xC1,
		0x1C, 0x14, 0x16, 0x12, 0x1A, 0x15, 0x1B, 0x19, 0x1E, 0x18, 0x1D, 0x17, 0x10, 0x13, 0x1F, 0x11,
		0xEC, 0xE4, 0xE6, 0xE2, 0xEA, 0xE5, 0xEB, 0xE9, 0xEE, 0xE8, 0xED, 0xE7, 0xE0, 0xE3, 0xEF, 0xE1,
		0x4C, 0x44, 0x46, 0x42, 0x4A, 0x45, 0x4B, 0x49, 0x4E, 0x48, 0x4D, 0x47, 0x40, 0x43, 0x4F, 0x41,
		0x7C, 0x74, 0x76, 0x72, 0x7A, 0x75, 0x7B, 0x79, 0x7E, 0x78, 0x7D, 0x77, 0x70, 0x73, 0x7F, 0x71,
		0xBC, 0xB4, 0xB6, 0xB2, 0xBA, 0xB5, 0xBB, 0xB9, 0xBE, 0xB8, 0xBD, 0xB7, 0xB0, 0xB3, 0xBF, 0xB1,
		0xDC, 0xD4, 0xD6, 0xD2, 0xDA, 0xD5, 0xDB, 0xD9, 0xDE, 0xD8, 0xDD, 0xD7, 0xD0, 0xD3, 0xDF, 0xD1,
		0x0C, 0x04, 0x06, 0x02, 0x0A, 0x05, 0x0B, 0x09, 0x0E, 0x08, 0x0D, 0x07, 0x00, 0x03, 0x0F, 0x01,
		0xFC, 0xF4, 0xF6, 0xF2, 0xFA, 0xF5, 0xFB, 0xF9, 0xFE, 0xF8, 0xFD, 0xF7, 0xF0, 0xF3, 0xFF, 0xF1
	},
	{
		0xCB, 0xC3, 0xC5, 0xC8, 0xC2, 0xCF, 0xCA, 0xCD, 0xCE, 0xC1, 0xC7, 0xC4, 0xCC, 0xC9, 0xC6, 0xC0,
		0x8B, 0x83, 0x85, 0x88, 0x82, 0x8F, 0x8A, 0x8D, 0x8E, 0x81, 0x87, 0x84, 0x8C, 0x89, 0x86, 0x80,
		0x2B, 0x23, 0x25, 0x28, 0x22, 0x2F, 0x2A, 0x2D, 0x2E, 0x21, 0x27, 0x24, 0x2C, 0x29, 0x26, 0x20,
		0x1B, 0x13, 0x15, 0x18, 0x12, 0x1F, 0x1A, 0x1D, 0x1E, 0x11, 0x17, 0x14, 0x1C, 0x19, 0x16, 0x10,
		0xDB, 0xD3, 0xD5, 0xD8, 0xD2, 0xDF, 0xDA, 0xDD, 0xDE, 0xD1, 0xD7, 0xD4, 0xDC, 0xD9, 0xD6, 0xD0,
		0x4B, 0x43, 0x45, 0x48, 0x42, 0x4F, 0x4A, 0x4D, 0x4E, 0x41, 0x47, 0x44, 0x4C, 0x49, 0x46, 0x40,
		0xFB, 0xF3, 0xF5, 0xF8, 0xF2, 0xFF, 0xFA, 0xFD, 0xFE, 0xF1, 0xF7, 0xF4, 0xFC, 0xF9, 0xF6, 0xF0,
		0x6B, 0x63, 0x65, 0x68, 0x62, 0x6F, 0x6A, 0x6D, 0x6E, 0x61, 0x67, 0x64, 0x6C, 0x69, 0x66, 0x60,
		0x7B, 0x73, 0x75, 0x78, 0x72, 0x7F, 0x7A, 0x7D, 0x7E, 0x71, 0x77, 0x74, 0x7C, 0x79, 0x76, 0x70,
		0x0B, 0x03, 0x05, 0x08, 0x02, 0x0F, 0x0A, 0x0D, 0x0E, 0x01, 0x07, 0x04, 0x0C, 0x09, 0x06, 0x00,
		0xAB, 0xA3, 0xA5, 0xA8, 0xA2, 0xAF, 0xAA, 0xAD, 0xAE, 0xA1, 0xA7, 0xA4, 0xAC, 0xA9, 0xA6, 0xA0,
		0x5B, 0x53, 0x55, 0x58, 0x52, 0x5F, 0x5A, 0x5D, 0x5E, 0x51, 0x57, 0x54, 0x5C, 0x59, 0x56, 0x50,
		0x3B, 0x33, 0x35, 0x38, 0x32, 0x3F, 0x3A, 0x3D, 0x3E, 0x31, 0x37, 0x34, 0x3C, 0x39, 0x36, 0x30,
		0xEB, 0xE3, 0xE5, 0xE8, 0xE2, 0xEF, 0xEA, 0xED, 0xEE, 0xE1, 0xE7, 0xE4, 0xEC, 0xE9, 0xE6, 0xE0,
		0x9B, 0x93, 0x95, 0x98, 0x92, 0x9F, 0x9A, 0x9D, 0x9E, 0x91, 0x97, 0x94, 0x9C, 0x99, 0x96, 0x90,
		0xBB, 0xB3, 0xB5, 0xB8, 0xB2, 0xBF, 0xBA, 0xBD, 0xBE, 0xB1, 0xB7, 0xB4, 0xBC, 0xB9, 0xB6, 0xB0
	},
	{
		0x57, 0x5F, 0x55, 0x5A, 0x58, 0x51, 0x56, 0x5D, 0x50, 0x59, 0x53, 0x5E, 0x5B, 0x54, 0x52, 0x5C,
		0xD7, 0xDF, 0xD5, 0xDA, 0xD8, 0xD1, 0xD6, 0xDD, 0xD0, 0xD9, 0xD3, 0xDE, 0xDB, 0xD4, 0xD2, 0xDC,
		0xF7, 0xFF, 0xF5, 0xFA, 0xF8, 0xF1, 0xF6, 0xFD, 0xF0, 0xF9, 0xF3, 0xFE, 0xFB, 0xF4, 0xF2, 0xFC,
		0x67, 0x6F, 0x65, 0x6A, 0x68, 0x61, 0x66, 0x6D, 0x60, 0x69, 0x63, 0x6E, 0x6B, 0x64, 0x62, 0x6C,
		0x97, 0x9F, 0x95, 0x9A, 0x98, 0x91, 0x96, 0x9D, 0x90, 0x99, 0x93, 0x9E, 0x9B, 0x94, 0x92, 0x9C,
		0x27, 0x2F, 0x25, 0x2A, 0x28, 0x21, 0x26, 0x2D, 0x20, 0x29, 0x23, 0x2E, 0x2B, 0x24, 0x22, 0x2C,
		0xC7, 0xCF, 0xC5, 0xCA, 0xC8, 0xC1, 0xC6, 0xCD, 0xC0, 0xC9, 0xC3, 0xCE, 0xCB, 0xC4, 0xC2, 0xCC,
		0xA7, 0xAF, 0xA5, 0xAA, 0xA8, 0xA1, 0xA6, 0xAD, 0xA0, 0xA9, 0xA3, 0xAE, 0xAB, 0xA4, 0xA2, 0xAC,
		0xB7, 0xBF, 0xB5, 0xBA, 0xB8, 0xB1, 0xB6, 0xBD, 0xB0, 0xB9, 0xB3, 0xBE, 0xBB, 0xB4, 0xB2, 0xBC,
		0x77, 0x7F, 0x75, 0x7A, 0x78, 0x71, 0x76, 0x7D, 0x70, 0x79, 0x73, 0x7E, 0x7B, 0x74, 0x72, 0x7C,
		0x87, 0x8F, 0x85, 0x8A, 0x88, 0x81, 0x86, 0x8D, 0x80, 0x89, 0x83, 0x8E, 0x8B, 0x84, 0x82, 0x8C,
		0x17, 0x1F, 0x15, 0x1A, 0x18, 0x11, 0x16, 0x1D, 0x10, 0x19, 0x13, 0x1E, 0x1B, 0x14, 0x12, 0x1C,
		0x47, 0x4F, 0x45, 0x4A, 0x48, 0x41, 0x46, 0x4D, 0x40, 0x49, 0x43, 0x4E, 0x4B, 0x44, 0x42, 0x4C,
		0x37, 0x3F, 0x35, 0x3A, 0x38, 0x31, 0x36, 0x3D, 0x30, 0x39, 0x33, 0x3E, 0x3B, 0x34, 0x32, 0x3C,
		0xE7, 0xEF, 0xE5, 0xEA, 0xE8, 0xE1, 0xE6, 0xED, 0xE0, 0xE9, 0xE3, 0xEE, 0xEB, 0xE4, 0xE2, 0xEC,
		0x07, 0x0F, 0x05, 0x0A, 0x08, 0x01, 0x06, 0x0D, 0x00, 0x09, 0x03, 0x0E, 0x0B, 0x04, 0x02, 0x0C
	},
	{
		0x18, 0x1E, 0x12, 0x15, 0x16, 0x19, 0x11, 0x1C, 0x1F, 0x14, 0x1B, 0x10, 0x1D, 0x1A, 0x13, 0x17,
		0x78, 0x7E, 0x72, 0x75, 0x76, 0x79, 0x71, 0x7C, 0x7F, 0x74, 0x7B, 0x70, 0x7D, 0x7A, 0x73, 0x77,
		0xE8, 0xEE, 0xE2, 0xE5, 0xE6, 0xE9, 0xE1, 0xEC, 0xEF, 0xE4, 0xEB, 0xE0, 0xED, 0xEA, 0xE3, 0xE7,
		0xD8, 0xDE, 0xD2, 0xD5, 0xD6, 0xD9, 0xD1, 0xDC, 0xDF, 0xD4, 0xDB, 0xD0, 0xDD, 0xDA, 0xD3, 0xD7,
		0x08, 0x0E, 0x02, 0x05, 0x06, 0x09, 0x01, 0x0C, 0x0F, 0x04, 0x0B, 0x00, 0x0D, 0x0A, 0x03, 0x07,
		0x58, 0x5E, 0x52, 0x55, 0x56, 0x59, 0x51, 0x5C, 0x5F, 0x54, 0x5B, 0x50, 0x5D, 0x5A, 0x53, 0x57,
		0x88, 0x8E, 0x82, 0x85, 0x86, 0x89, 0x81, 0x8C, 0x8F, 0x84, 0x8B, 0x80, 0x8D, 0x8A, 0x83, 0x87,
		0x38, 0x3E, 0x32, 0x35, 0x36, 0x39, 0x31, 0x3C, 0x3F, 0x34, 0x3B, 0x30, 0x3D, 0x3A, 0x33, 0x37,
		0x48, 0x4E, 0x42, 0x45, 0x46, 0x49, 0x41, 0x4C, 0x4F, 0x44, 0x4B, 0x40, 0x4D, 0x4A, 0x43, 0x47,
		0xF8, 0xFE, 0xF2, 0xF5, 0xF6, 0xF9, 0xF1, 0xFC, 0xFF, 0xF4, 0xFB, 0xF0, 0xFD, 0xFA, 0xF3, 0xF7,
		0xA8, 0xAE, 0xA2, 0xA5, 0xA6, 0xA9, 0xA1, 0xAC, 0xAF, 0xA4, 0xAB, 0xA0, 0xAD, 0xAA, 0xA3, 0xA7,
		0x68, 0x6E, 0x62, 0x65, 0x66, 0x69, 0x61, 0x6C, 0x6F, 0x64, 0x6B, 0x60, 0x6D, 0x6A, 0x63, 0x67,
		0x98, 0x9E, 0x92, 0x95, 0x96, 0x99, 0x91, 0x9C, 0x9F, 0x94, 0x9B, 0x90, 0x9D, 0x9A, 0x93, 0x97,
		0xC8, 0xCE, 0xC2, 0xC5, 0xC6, 0xC9, 0xC1, 0xCC, 0xCF, 0xC4, 0xCB, 0xC0, 0xCD, 0xCA, 0xC3, 0xC7,
		0xB8, 0xBE, 0xB2, 0xB5, 0xB6, 0xB9, 0xB1, 0xBC, 0xBF, 0xB4, 0xBB, 0xB0, 0xBD, 0xBA, 0xB3, 0xB7,
		0x28, 0x2E, 0x22, 0x25, 0x26, 0x29, 0x21, 0x2C, 0x2F, 0x24, 0x2B, 0x20, 0x2D, 0x2A, 0x23, 0x27
	}
};

void T64(BLOCK32 a)
{
	for(size_t i = 0; i < BLOCK32SIZE; i++)
	{
        a[i] = pi64[i][a[i]];
	}
}

void g64(BLOCK32 a, const BLOCK32 round_key)
{
    uint32_t _a = *(uint32_t*)a;
    uint32_t _round_key = *(uint32_t*)round_key;
    uint32_t result = _a + _round_key;
	T64((uint8_t*)&result);
	uint32_t high11 = (result & HIGH11) >> 21;
	result <<= 11;
	result |= high11;
    *(uint32_t*)a = result;
    _a = _round_key = result = 0x00;
}

void KeySchedule64(uint8_t pwd[32], gost64_key *key)
{
    for(int i = 7, pwd_i = 0; i >= 0; i--, pwd_i++)
	{
        key->rd_key[i][3] = key->rd_key[i + 8][3] = key->rd_key[i + 16][3] = pwd[pwd_i * 4 + 3];
        key->rd_key[i][2] = key->rd_key[i + 8][2] = key->rd_key[i + 16][2] = pwd[pwd_i * 4 + 2];
        key->rd_key[i][1] = key->rd_key[i + 8][1] = key->rd_key[i + 16][1] = pwd[pwd_i * 4 + 1];
        key->rd_key[i][0] = key->rd_key[i + 8][0] = key->rd_key[i + 16][0] = pwd[pwd_i * 4];
	}

    for(int i = 7; i >= 0; i--)
	{
        key->rd_key[i + 24][3] = key->rd_key[7 - i][3];
        key->rd_key[i + 24][2] = key->rd_key[7 - i][2];
        key->rd_key[i + 24][1] = key->rd_key[7 - i][1];
        key->rd_key[i + 24][0] = key->rd_key[7 - i][0];
	}
}

void G64(BLOCK32 a1, BLOCK32 a0, const BLOCK32 round_key)
{
	BLOCK32 tmp = {0};
    memcpy_s(tmp, BLOCK32SIZE, a0, BLOCK32SIZE);

    g64(a0, round_key);
	X32(a0, a1, a0);

    memcpy_s(a1, BLOCK32SIZE, tmp, BLOCK32SIZE);
	memset(tmp, 0x00, BLOCK32SIZE);
}

void G64Star(BLOCK32 a1, BLOCK32 a0, const BLOCK32 round_key, BLOCK64 out)
{
	BLOCK32 tmp = {0};
    memcpy_s(tmp, BLOCK32SIZE, a0, BLOCK32SIZE);

    g64(a0, round_key);
	X32(a0, a1, a0);

    memcpy_s(out + BLOCK32SIZE, BLOCK32SIZE, a0, BLOCK32SIZE);
    memcpy_s(out, BLOCK32SIZE, tmp, BLOCK32SIZE);

	memset(tmp, 0x00, BLOCK32SIZE);
}

void gost_r3412_64_encrypt_block(const BLOCK64 in, BLOCK64 out, gost64_key *key)
{
	BLOCK32 a0 = {0}, a1 = {0};
    memcpy_s(a0, BLOCK32SIZE, in, BLOCK32SIZE);
    memcpy_s(a1, BLOCK32SIZE, in + BLOCK32SIZE, BLOCK32SIZE);

    for(size_t i = 0; i < GOST64_ROUNDS_COUNT - 1; i++)
	{
        G64(a1, a0, key->rd_key[i]);
	}
    G64Star(a1, a0, key->rd_key[GOST64_ROUNDS_COUNT - 1], out);
	
	memset(a1, 0x00, BLOCK32SIZE);
	memset(a0, 0x00, BLOCK32SIZE);
}

void gost_r3412_64_decrypt_block(const BLOCK64 in, BLOCK64 out, gost64_key *key)
{
	BLOCK32 a0 = {0}, a1 = {0};
    memcpy_s(a0, BLOCK32SIZE, in, BLOCK32SIZE);
    memcpy_s(a1, BLOCK32SIZE, in + BLOCK32SIZE, BLOCK32SIZE);

    for(size_t i = GOST64_ROUNDS_COUNT - 1; i >= 1; i--)
	{
        G64(a1, a0, key->rd_key[i]);
	}
    G64Star(a1, a0, key->rd_key[0], out);

	memset(a1, 0x00, BLOCK32SIZE);
	memset(a0, 0x00, BLOCK32SIZE);
}
