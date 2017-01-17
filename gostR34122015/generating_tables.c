#include "generating_tables.h"

void printBas(uint8_t *a, size_t a_size)
{
    for(size_t i = 0; i < a_size; i++)
        if(a[i] > 0xf) printf("0x%X ", a[i]);
        else printf("0x0%X ", a[i]);
    printf("\n");
}

void printB256(BLOCK256 a)
{
    for(size_t i = 0; i < BLOCK256SIZE; i++)
        if(a[i] > 0xf) printf("0x%X ", a[i]);
        else printf("0x0%X ", a[i]);
    printf("\n");
}

void printB128(BLOCK128 a)
{
    for(size_t i = 0; i < BLOCK128SIZE; i++)
        if(a[i] > 0xf) printf("0x%X ", a[i]);
        else printf("0x0%X ", a[i]);
    printf("\n");
}

void printKey(gost_key *key)
{
    if(key->bits == 64)
    {
        for(size_t i = 0; i < GOST128_ROUND_KEYS_COUNT; i++)
        {
            for(size_t j = 0; j < GOST128_ROUND_KEY_SIZE; j++)
                if(key->rd_key_64[i][j] > 0xf) printf("0x%X ", key->rd_key_64[i][j]);
                else printf("0x0%X ", key->rd_key_64[i][j]);
            printf("\n");
        }
    }
    else if(key->bits == 128)
    {
        for(size_t i = 0; i < GOST64_ROUND_KEYS_COUNT; i++)
        {
            for(size_t j = 0; j < GOST64_ROUND_KEY_SIZE; j++)
                if(key->rd_key_128[i][j] > 0xf) printf("0x%X ", key->rd_key_128[i][j]);
                else printf("0x0%X ", key->rd_key_128[i][j]);
            printf("\n");
        }
    }
}

void printB64(BLOCK64 a)
{
    for(size_t i = 0; i < BLOCK64SIZE; i++)
        if(a[i] > 0xf) printf("0x%X ", a[i]);
        else printf("0x0%X ", a[i]);
    printf("\n");
}

void printB32(BLOCK32 a)
{
    for(size_t i = 0; i < BLOCK32SIZE; i++)
        if(a[i] > 0xf) printf("0x%X ", a[i]);
        else printf("0x0%X ", a[i]);
    printf("\n");
}

uint16_t multiply(uint16_t lhs, uint16_t rhs)
{

    const uint16_t linear_transform_modulus = 0x01C3;
    uint16_t result = 0, modulus = linear_transform_modulus << 7;
    for(uint16_t detecter = 0x1; detecter != 0x100; detecter <<= 1, lhs <<= 1)
        if(rhs & detecter) result ^= lhs;
    for(uint16_t detecter = 0x8000; detecter != 0x80; detecter >>= 1, modulus >>= 1)
        if(result & detecter) result ^= modulus;
    return result;
}


void generateLinearTable()
{
    const uint8_t coeffL128[] = { 0x01, 0x10, 0x20, 0xC0, 0xC2, 0x85, 0x94, 0xFB };

    FILE *f = NULL;
    errno_t error = fopen_s(&f, "subst.txt", "w");
    if(f != NULL)
    {
        fprintf_s(f, "{\n");
        for(size_t i = 0; i < sizeof(coeffL128) / sizeof(coeffL128[0]); i++)
        {
            fprintf_s(f, "{\n");
            for(uint16_t j = 0; j < 0x100; j++)
            {
                uint16_t result = multiply(j, coeffL128[i]);
                if(j == 0xff) fprintf_s(f, "0x%X", result);
                else if(result > 0xf) fprintf_s(f, "0x%X, ", result);
                else fprintf_s(f, "0x0%X, ", result);

                if((j & 0xf) == 0xf) fprintf_s(f, "\n");
            }
            if (i == sizeof(coeffL128) / sizeof(coeffL128[0]) - 1)
                fprintf_s(f, "}\n");
            else fprintf_s(f, "},\n");
        }
        fprintf_s(f, "};");

        fclose(f);
    }
    else printf("Error while openning file");

    printf("Generating linear table: done");
}

void generateCoeffPositionTable()
{
    FILE *f = NULL;
    errno_t error = fopen_s(&f, "substPos.txt", "w");
    if(f != NULL)
    {
        uint8_t matrix[256] = { 0 };
        const uint8_t coeffL128[] = { 0x01, 0x10, 0x20, 0xC0, 0xC2, 0x85, 0x94, 0xFB };
        for(size_t i = 0; i < sizeof(coeffL128) / sizeof(coeffL128[0]); i++)
            matrix[coeffL128[i]] = (uint8_t)i;

        fprintf(f, "{\n\t");
        for(size_t i = 0; i < 256; i++)
        {
            if(matrix[i] > 0xf) fprintf(f, "0x%X", matrix[i]);
            else fprintf(f, "0x0%X", matrix[i]);
            if(i != 255) fprintf(f, ", ");
            if((i & 0xf) == 0xf) fprintf(f, "\n\t");
        }
        fprintf(f, "};");
    }

}
