#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "r34122015_types.h"
#include "gost_2015.h"
//#include "r34122015_private.h"
#include "generating_tables.h"

#define arraysize(a) (sizeof(a) / sizeof(a[0]))

int main(int argc, char *argv[])
{
    //generateCoeffPositionTable();
    //generateLinearTable();

    /*
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

    gost_key key128;
    uint8_t test128_key[] = { 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
                           0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88 };

    printf("\n\nKey: \n\n");
    keySchedule(test128_key, 128, &key128);
    printKey(&key128);

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
    */

    /*
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

    gost_key key64;
    memset(&key64, 0x00, sizeof(key64));


    keySchedule(test64_key, 64, &key64);
    printKey(&key64);

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
    */

    /*
    uint8_t suppl[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    BLOCK64 suppl01;
    supplementation_01(suppl, sizeof(suppl), suppl01, BLOCK64SIZE);
    printB64(suppl01);

    BLOCK128 suppl02;
    size_t out_size = BLOCK128SIZE;
    supplementation_02(suppl, sizeof(suppl), suppl02, BLOCK128SIZE);
    printB128(suppl02);

    BLOCK64 suppl03;
    supplementation_03(suppl, 1, suppl03, BLOCK64SIZE);
    printB64(suppl03);
    */

    uint8_t key_arr[] =
    {
        0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
        0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88
    };
    gost_key key;
    gost_2015_set_key(key_arr, 128, &key);

    uint8_t pt[] =
    {
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
        0x0a, 0xff, 0xee, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
        0x00, 0x0a, 0xff, 0xee, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
        0x11, 0x00, 0x0a, 0xff, 0xee, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22
    };


    /*
    size_t ecb_ct_size = 0;
    gost_2015_ecb_encrypt(pt, arraysize(pt), NULL, &ecb_ct_size, &key);
    printf("ecb_ct SIZE: %zd\n", ecb_ct_size);
    uint8_t *ecb_ct = (uint8_t*)malloc(ecb_ct_size);
    gost_2015_ecb_encrypt(pt, arraysize(pt), ecb_ct, &ecb_ct_size, &key);
    printBas(ecb_ct, ecb_ct_size);
    printf("\n\n");

    size_t ecb_pt_size = 0;
    gost_2015_ecb_decrypt(ecb_ct, ecb_ct_size, NULL, &ecb_pt_size, &key);
    printf("ecb_pt SIZE: %zd\n", ecb_pt_size);
    uint8_t *ecb_pt_01 = (uint8_t*)malloc(ecb_pt_size);
    gost_2015_ecb_decrypt(ecb_ct, ecb_ct_size, ecb_pt_01, &ecb_pt_size, &key);
    printBas(ecb_pt_01, ecb_pt_size);

    free(ecb_ct);
    free(ecb_pt_01);
    */

    /*
    BLOCK64 iv_128_ctr = { 0xf0, 0xce, 0xab, 0x90, 0x78, 0x56, 0x34, 0x12 };
    size_t ctr_ct_size = 0;
    gost_2015_ctr_encrypt(pt, arraysize(pt), iv_128_ctr, arraysize(iv_128_ctr), NULL, &ctr_ct_size, &key);
    printf("CTR ciphertext size: %zd\n", ctr_ct_size);
    uint8_t *ctr_ct = (uint8_t*)malloc(ctr_ct_size);
    gost_2015_ctr_encrypt(pt, arraysize(pt), iv_128_ctr, arraysize(iv_128_ctr), ctr_ct, &ctr_ct_size, &key);
    printBas(ctr_ct, ctr_ct_size);

    size_t ctr_pt_size = 0;
    gost_2015_ctr_decrypt(ctr_ct, ctr_ct_size, iv_128_ctr, arraysize(iv_128_ctr), NULL, &ctr_pt_size, &key);
    printf("CTR plaintext size: %zd\n", ctr_pt_size);
    uint8_t *ctr_pt_01 = (uint8_t*)malloc(ctr_pt_size);
    gost_2015_ctr_decrypt(ctr_ct, ctr_ct_size, iv_128_ctr, arraysize(iv_128_ctr), ctr_pt_01, &ctr_pt_size, &key);
    printBas(ctr_pt_01, ctr_pt_size);

    free(ctr_pt_01);
    free(ctr_ct);
    */

    /*
    uint8_t iv_128_ofb[] = { 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x90, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23,
                             0x12, 0x01, 0xf0, 0xe5, 0xd4, 0xc3, 0xb2, 0xa1, 0xf0, 0xce, 0xab, 0x90, 0x78, 0x56, 0x34, 0x12 };

    size_t ofb_ct_size = 0;
    gost_2015_ofb_encrypt(pt, arraysize(pt), iv_128_ofb, arraysize(iv_128_ofb), NULL, &ofb_ct_size, &key);
    printf("OFB ciphertext size: %zd\n", ofb_ct_size);
    uint8_t *ofb_ct = (uint8_t*)malloc(ofb_ct_size);
    gost_2015_ofb_encrypt(pt, arraysize(pt), iv_128_ofb, arraysize(iv_128_ofb), ofb_ct, &ofb_ct_size, &key);
    printBas(ofb_ct, ofb_ct_size);

    size_t ofb_pt_size = 0;
    gost_2015_ofd_decrypt(ofb_ct, ofb_ct_size, iv_128_ofb, arraysize(iv_128_ofb), NULL, &ofb_pt_size, &key);
    printf("OFB plaintext size: %zd\n", ofb_pt_size);
    uint8_t *ofb_pt_01 = (uint8_t*)malloc(ofb_pt_size);
    gost_2015_ofd_decrypt(ofb_ct, ofb_ct_size, iv_128_ofb, arraysize(iv_128_ofb), ofb_pt_01, &ofb_pt_size, &key);
    printBas(ofb_pt_01, ofb_pt_size);

    free(ofb_pt_01);
    free(ofb_ct);
    */

    /*
    uint8_t iv_128_cbc[] = { 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x90, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23,
                             0x12, 0x01, 0xf0, 0xe5, 0xd4, 0xc3, 0xb2, 0xa1, 0xf0, 0xce, 0xab, 0x90, 0x78, 0x56, 0x34, 0x12 };

    size_t cbc_ct_size = 0;
    gost_2015_cbc_encrypt(pt, arraysize(pt), iv_128_cbc, arraysize(iv_128_cbc), NULL, &cbc_ct_size, &key);
    printf("CBC ciphertext size: %zd\n", cbc_ct_size);
    uint8_t *cbc_ct = (uint8_t*)malloc(cbc_ct_size);
    gost_2015_cbc_encrypt(pt, arraysize(pt), iv_128_cbc, arraysize(iv_128_cbc), cbc_ct, &cbc_ct_size, &key);
    printBas(cbc_ct, cbc_ct_size);

    size_t cbc_pt_size = 0;
    gost_2015_cbc_decrypt(cbc_ct, cbc_ct_size, iv_128_cbc, arraysize(iv_128_cbc), NULL, &cbc_pt_size, &key);
    printf("CBC plaintext size: %zd\n", cbc_pt_size);
    uint8_t *cbc_pt_01 = (uint8_t*)malloc(cbc_pt_size);
    gost_2015_cbc_decrypt(cbc_ct, cbc_ct_size, iv_128_cbc, arraysize(iv_128_cbc), cbc_pt_01, &cbc_pt_size, &key);
    printBas(cbc_pt_01, cbc_pt_size);

    free(cbc_pt_01);
    free(cbc_ct);
    */

    uint8_t iv_128_cfb[] = { 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x90, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23,
                             0x12, 0x01, 0xf0, 0xe5, 0xd4, 0xc3, 0xb2, 0xa1, 0xf0, 0xce, 0xab, 0x90, 0x78, 0x56, 0x34, 0x12 };

    size_t out_block_size = BLOCK128SIZE;

    size_t cfb_ct_size = 0;
    gost_2015_cfb_encrypt(pt, arraysize(pt), iv_128_cfb, arraysize(iv_128_cfb), NULL, &cfb_ct_size, &key, &out_block_size);
    printf("CFB ciphertextsize: %zd\n", cfb_ct_size);
    uint8_t *cfb_ct = (uint8_t *)malloc(cfb_ct_size);
    gost_2015_cfb_encrypt(pt, arraysize(pt), iv_128_cfb, arraysize(iv_128_cfb), cfb_ct, &cfb_ct_size, &key, &out_block_size);
    printBas(cfb_ct, cfb_ct_size);

    size_t cfb_pt_size = 0;
    gost_2015_cfb_decrypt(cfb_ct, cfb_ct_size, iv_128_cfb, arraysize(iv_128_cfb), NULL, &cfb_pt_size, &key, &out_block_size);
    printf("CFB plaintext size: %zd\n", cfb_pt_size);
    uint8_t *cfb_pt = (uint8_t*)malloc(cfb_pt_size);
    gost_2015_cfb_decrypt(cfb_ct, cfb_ct_size, iv_128_cfb, arraysize(iv_128_cfb), cfb_pt, &cfb_pt_size, &key, &out_block_size);
    printBas(cfb_pt, cfb_pt_size);

    free(cfb_pt);
    free(cfb_ct);


    printf("\n\nall done");
    getchar();
    return 0;
}
