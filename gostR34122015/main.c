#include <stdio.h>
#include <stdint.h>
#include <conio.h>

#include "r34122015.h"
#include "generating_tables.h"

int main(int argc, char *argv[])
{
    //generateCoeffPositionTable();
    //generateLinearTable();

    printf("\n\n\n\nGOST R34.12 2015 128bit\n\n");

    printf("testing S:\n\n");
    uint8_t test128s[] = { 0x00, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    S128(test128s);
    printB128(test128s);
    S128(test128s);
    printB128(test128s);
    S128(test128s);
    printB128(test128s);
    S128(test128s);
    printB128(test128s);
    printf("\n");

    printf("testing R:\n\n");
    uint8_t test128r[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    R128(test128r);
    printB128(test128r);
    R128(test128r);
    printB128(test128r);
    R128(test128r);
    printB128(test128r);
    R128(test128r);
    printB128(test128r);
    printf("\n");

    printf("testing L:\n\n");
    uint8_t test128l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0xa5, 0x64 };
    L128(test128l);
    printB128(test128l);
    L128(test128l);
    printB128(test128l);
    L128(test128l);
    printB128(test128l);
    L128(test128l);
    printB128(test128l);
    printf("\n");


    gost128_key key128;
    uint8_t test128_key[] = { 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
                           0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88 };

    printf("\n\nKey: \n\n");
    KeySchedule128(test128_key, &key128);
    print128key(&key128);

    uint8_t test128[] = { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };

    printf("\n\nPlaintext:\n\n");
    printB128(test128);

    printf("\n\nEncrypted text\n\n");
    BLOCK128 out;
    gost_r3412_128_encrypt_block(test128, out, &key128);
    printB128(out);

    printf("\n\nDecrypted text\n\n");
    BLOCK128 decrypted = { 0 };
    gost_r3415_128_decrypt_block(out, decrypted, &key128);
    printB128(decrypted);

    printf("\n\n\n\nGOST R34.12 2015 64bit\n\n");

    printf("testing t:\n\n");
    BLOCK32 test64t = { 0x31, 0x75, 0xb9, 0xfd };
    T64(test64t);
    printB32(test64t);
    T64(test64t);
    printB32(test64t);
    T64(test64t);
    printB32(test64t);
    T64(test64t);
    printB32(test64t);

    printf("\n");

    printf("testing g:\n\n");
    BLOCK32 test64g = { 0x98, 0xba, 0xdc, 0xfe };
    BLOCK32 test64g_round_key = { 0x21, 0x43, 0x65, 0x87 };
    g64(test64g, test64g_round_key);
    printB32(test64g);
    g64(test64g_round_key, test64g);
    printB32(test64g_round_key);
    g64(test64g, test64g_round_key);
    printB32(test64g);
    g64(test64g_round_key, test64g);
    printB32(test64g_round_key);

    uint8_t test64_key[] = { 0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0,
                             0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

    printf("\n\nKeys: \n\n");

    gost64_key key64;
    memset(&key64, 0x00, sizeof(key64));

    KeySchedule64(test64_key, &key64);
    print64key(&key64);

    printf("\n\nPlaintext:\n\n");
    uint8_t test64[] = { 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe };
    printB64(test64);

    printf("\n\nEncrypted text\n\n");
    BLOCK64 out64;
    gost_r3412_64_encrypt_block(test64, out64, &key64);
    printB64(out64);

    printf("\n\nDecrypted text\n\n");
    BLOCK128 decrypted64 = { 0 };
    gost_r3412_64_decrypt_block(out64, decrypted64, &key64);
    printB64(decrypted64);


    printf("\n\nall done");
    getchar();
    return 0;
}
