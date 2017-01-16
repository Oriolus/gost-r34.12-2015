#include "gost_2015.h"

/*
 * remake key_scheldule
 * - key size
 */

static const uint8_t coeffL128[BLOCK128SIZE] =
{
    0x01, 0x94, 0x20, 0x85, 0x10, 0xC2, 0xC0, 0x01, 0xFB, 0x01, 0xC0, 0xC2, 0x10, 0x85, 0x20, 0x94
};
static const uint8_t pi128[256] =
{
	0xFC, 0xEE, 0xDD, 0x11, 0xCF, 0x6E, 0x31, 0x16, 0xFB, 0xC4, 0xFA, 0xDA, 0x23, 0xC5, 0x04, 0x4D,
	0xE9, 0x77, 0xF0, 0xDB, 0x93, 0x2E, 0x99, 0xBA, 0x17, 0x36, 0xF1, 0xBB, 0x14, 0xCD, 0x5F, 0xC1,
	0xF9, 0x18, 0x65, 0x5A, 0xE2, 0x5C, 0xEF, 0x21, 0x81, 0x1C, 0x3C, 0x42, 0x8B, 0x01, 0x8E, 0x4F,
	0x05, 0x84, 0x02, 0xAE, 0xE3, 0x6A, 0x8F, 0xA0, 0x06, 0x0B, 0xED, 0x98, 0x7F, 0xD4, 0xD3, 0x1F,
	0xEB, 0x34, 0x2C, 0x51, 0xEA, 0xC8, 0x48, 0xAB, 0xF2, 0x2A, 0x68, 0xA2, 0xFD, 0x3A, 0xCE, 0xCC,
	0xB5, 0x70, 0x0E, 0x56, 0x08, 0x0C, 0x76, 0x12, 0xBF, 0x72, 0x13, 0x47, 0x9C, 0xB7, 0x5D, 0x87,
	0x15, 0xA1, 0x96, 0x29, 0x10, 0x7B, 0x9A, 0xC7, 0xF3, 0x91, 0x78, 0x6F, 0x9D, 0x9E, 0xB2, 0xB1,
	0x32, 0x75, 0x19, 0x3D, 0xFF, 0x35, 0x8A, 0x7E, 0x6D, 0x54, 0xC6, 0x80, 0xC3, 0xBD, 0x0D, 0x57,
	0xDF, 0xF5, 0x24, 0xA9, 0x3E, 0xA8, 0x43, 0xC9, 0xD7, 0x79, 0xD6, 0xF6, 0x7C, 0x22, 0xB9, 0x03,
	0xE0, 0x0F, 0xEC, 0xDE, 0x7A, 0x94, 0xB0, 0xBC, 0xDC, 0xE8, 0x28, 0x50, 0x4E, 0x33, 0x0A, 0x4A,
	0xA7, 0x97, 0x60, 0x73, 0x1E, 0x00, 0x62, 0x44, 0x1A, 0xB8, 0x38, 0x82, 0x64, 0x9F, 0x26, 0x41,
	0xAD, 0x45, 0x46, 0x92, 0x27, 0x5E, 0x55, 0x2F, 0x8C, 0xA3, 0xA5, 0x7D, 0x69, 0xD5, 0x95, 0x3B,
	0x07, 0x58, 0xB3, 0x40, 0x86, 0xAC, 0x1D, 0xF7, 0x30, 0x37, 0x6B, 0xE4, 0x88, 0xD9, 0xE7, 0x89,
	0xE1, 0x1B, 0x83, 0x49, 0x4C, 0x3F, 0xF8, 0xFE, 0x8D, 0x53, 0xAA, 0x90, 0xCA, 0xD8, 0x85, 0x61,
	0x20, 0x71, 0x67, 0xA4, 0x2D, 0x2B, 0x09, 0x5B, 0xCB, 0x9B, 0x25, 0xD0, 0xBE, 0xE5, 0x6C, 0x52,
	0x59, 0xA6, 0x74, 0xD2, 0xE6, 0xF4, 0xB4, 0xC0, 0xD1, 0x66, 0xAF, 0xC2, 0x39, 0x4B, 0x63, 0xB6
};
static const uint8_t invPi128[256] =
{
	0xA5, 0x2D, 0x32, 0x8F, 0x0E, 0x30, 0x38, 0xC0, 0x54, 0xE6, 0x9E, 0x39, 0x55, 0x7E, 0x52, 0x91,
	0x64, 0x03, 0x57, 0x5A, 0x1C, 0x60, 0x07, 0x18, 0x21, 0x72, 0xA8, 0xD1, 0x29, 0xC6, 0xA4, 0x3F,
	0xE0, 0x27, 0x8D, 0x0C, 0x82, 0xEA, 0xAE, 0xB4, 0x9A, 0x63, 0x49, 0xE5, 0x42, 0xE4, 0x15, 0xB7,
	0xC8, 0x06, 0x70, 0x9D, 0x41, 0x75, 0x19, 0xC9, 0xAA, 0xFC, 0x4D, 0xBF, 0x2A, 0x73, 0x84, 0xD5,
	0xC3, 0xAF, 0x2B, 0x86, 0xA7, 0xB1, 0xB2, 0x5B, 0x46, 0xD3, 0x9F, 0xFD, 0xD4, 0x0F, 0x9C, 0x2F,
	0x9B, 0x43, 0xEF, 0xD9, 0x79, 0xB6, 0x53, 0x7F, 0xC1, 0xF0, 0x23, 0xE7, 0x25, 0x5E, 0xB5, 0x1E,
	0xA2, 0xDF, 0xA6, 0xFE, 0xAC, 0x22, 0xF9, 0xE2, 0x4A, 0xBC, 0x35, 0xCA, 0xEE, 0x78, 0x05, 0x6B,
	0x51, 0xE1, 0x59, 0xA3, 0xF2, 0x71, 0x56, 0x11, 0x6A, 0x89, 0x94, 0x65, 0x8C, 0xBB, 0x77, 0x3C,
	0x7B, 0x28, 0xAB, 0xD2, 0x31, 0xDE, 0xC4, 0x5F, 0xCC, 0xCF, 0x76, 0x2C, 0xB8, 0xD8, 0x2E, 0x36,
	0xDB, 0x69, 0xB3, 0x14, 0x95, 0xBE, 0x62, 0xA1, 0x3B, 0x16, 0x66, 0xE9, 0x5C, 0x6C, 0x6D, 0xAD,
	0x37, 0x61, 0x4B, 0xB9, 0xE3, 0xBA, 0xF1, 0xA0, 0x85, 0x83, 0xDA, 0x47, 0xC5, 0xB0, 0x33, 0xFA,
	0x96, 0x6F, 0x6E, 0xC2, 0xF6, 0x50, 0xFF, 0x5D, 0xA9, 0x8E, 0x17, 0x1B, 0x97, 0x7D, 0xEC, 0x58,
	0xF7, 0x1F, 0xFB, 0x7C, 0x09, 0x0D, 0x7A, 0x67, 0x45, 0x87, 0xDC, 0xE8, 0x4F, 0x1D, 0x4E, 0x04,
	0xEB, 0xF8, 0xF3, 0x3E, 0x3D, 0xBD, 0x8A, 0x88, 0xDD, 0xCD, 0x0B, 0x13, 0x98, 0x02, 0x93, 0x80,
	0x90, 0xD0, 0x24, 0x34, 0xCB, 0xED, 0xF4, 0xCE, 0x99, 0x10, 0x44, 0x40, 0x92, 0x3A, 0x01, 0x26,
	0x12, 0x1A, 0x48, 0x68, 0xF5, 0x81, 0x8B, 0xC7, 0xD6, 0x20, 0x0A, 0x08, 0x00, 0x4C, 0xD7, 0x74
};
static const uint8_t C128[32][16] =
{
	{ 0x01, 0x94, 0x84, 0xDD, 0x10, 0xBD, 0x27, 0x5D, 0xB8, 0x7A, 0x48, 0x6C, 0x72, 0x76, 0xA2, 0x6E },
	{ 0x02, 0xEB, 0xCB, 0x79, 0x20, 0xB9, 0x4E, 0xBA, 0xB3, 0xF4, 0x90, 0xD8, 0xE4, 0xEC, 0x87, 0xDC },
	{ 0x03, 0x7F, 0x4F, 0xA4, 0x30, 0x04, 0x69, 0xE7, 0x0B, 0x8E, 0xD8, 0xB4, 0x96, 0x9A, 0x25, 0xB2 },
	{ 0x04, 0x15, 0x55, 0xF2, 0x40, 0xB1, 0x9C, 0xB7, 0xA5, 0x2B, 0xE3, 0x73, 0x0B, 0x1B, 0xCD, 0x7B },
	{ 0x05, 0x81, 0xD1, 0x2F, 0x50, 0x0C, 0xBB, 0xEA, 0x1D, 0x51, 0xAB, 0x1F, 0x79, 0x6D, 0x6F, 0x15 },
	{ 0x06, 0xFE, 0x9E, 0x8B, 0x60, 0x08, 0xD2, 0x0D, 0x16, 0xDF, 0x73, 0xAB, 0xEF, 0xF7, 0x4A, 0xA7 },
	{ 0x07, 0x6A, 0x1A, 0x56, 0x70, 0xB5, 0xF5, 0x50, 0xAE, 0xA5, 0x3B, 0xC7, 0x9D, 0x81, 0xE8, 0xC9 },
	{ 0x08, 0x2A, 0xAA, 0x27, 0x80, 0xA1, 0xFB, 0xAD, 0x89, 0x56, 0x05, 0xE6, 0x16, 0x36, 0x59, 0xF6 },
	{ 0x09, 0xBE, 0x2E, 0xFA, 0x90, 0x1C, 0xDC, 0xF0, 0x31, 0x2C, 0x4D, 0x8A, 0x64, 0x40, 0xFB, 0x98 },
	{ 0x0A, 0xC1, 0x61, 0x5E, 0xA0, 0x18, 0xB5, 0x17, 0x3A, 0xA2, 0x95, 0x3E, 0xF2, 0xDA, 0xDE, 0x2A },
	{ 0x0B, 0x55, 0xE5, 0x83, 0xB0, 0xA5, 0x92, 0x4A, 0x82, 0xD8, 0xDD, 0x52, 0x80, 0xAC, 0x7C, 0x44 },
	{ 0x0C, 0x3F, 0xFF, 0xD5, 0xC0, 0x10, 0x67, 0x1A, 0x2C, 0x7D, 0xE6, 0x95, 0x1D, 0x2D, 0x94, 0x8D },
	{ 0x0D, 0xAB, 0x7B, 0x08, 0xD0, 0xAD, 0x40, 0x47, 0x94, 0x07, 0xAE, 0xF9, 0x6F, 0x5B, 0x36, 0xE3 },
	{ 0x0E, 0xD4, 0x34, 0xAC, 0xE0, 0xA9, 0x29, 0xA0, 0x9F, 0x89, 0x76, 0x4D, 0xF9, 0xC1, 0x13, 0x51 },
	{ 0x0F, 0x40, 0xB0, 0x71, 0xF0, 0x14, 0x0E, 0xFD, 0x27, 0xF3, 0x3E, 0x21, 0x8B, 0xB7, 0xB1, 0x3F },
	{ 0x10, 0x54, 0x97, 0x4E, 0xC3, 0x81, 0x35, 0x99, 0xD1, 0xAC, 0x0A, 0x0F, 0x2C, 0x6C, 0xB2, 0x2F },
	{ 0x11, 0xC0, 0x13, 0x93, 0xD3, 0x3C, 0x12, 0xC4, 0x69, 0xD6, 0x42, 0x63, 0x5E, 0x1A, 0x10, 0x41 },
	{ 0x12, 0xBF, 0x5C, 0x37, 0xE3, 0x38, 0x7B, 0x23, 0x62, 0x58, 0x9A, 0xD7, 0xC8, 0x80, 0x35, 0xF3 },
	{ 0x13, 0x2B, 0xD8, 0xEA, 0xF3, 0x85, 0x5C, 0x7E, 0xDA, 0x22, 0xD2, 0xBB, 0xBA, 0xF6, 0x97, 0x9D },
	{ 0x14, 0x41, 0xC2, 0xBC, 0x83, 0x30, 0xA9, 0x2E, 0x74, 0x87, 0xE9, 0x7C, 0x27, 0x77, 0x7F, 0x54 },
	{ 0x15, 0xD5, 0x46, 0x61, 0x93, 0x8D, 0x8E, 0x73, 0xCC, 0xFD, 0xA1, 0x10, 0x55, 0x01, 0xDD, 0x3A },
	{ 0x16, 0xAA, 0x09, 0xC5, 0xA3, 0x89, 0xE7, 0x94, 0xC7, 0x73, 0x79, 0xA4, 0xC3, 0x9B, 0xF8, 0x88 },
	{ 0x17, 0x3E, 0x8D, 0x18, 0xB3, 0x34, 0xC0, 0xC9, 0x7F, 0x09, 0x31, 0xC8, 0xB1, 0xED, 0x5A, 0xE6 },
	{ 0x18, 0x7E, 0x3D, 0x69, 0x43, 0x20, 0xCE, 0x34, 0x58, 0xFA, 0x0F, 0xE9, 0x3A, 0x5A, 0xEB, 0xD9 },
	{ 0x19, 0xEA, 0xB9, 0xB4, 0x53, 0x9D, 0xE9, 0x69, 0xE0, 0x80, 0x47, 0x85, 0x48, 0x2C, 0x49, 0xB7 },
	{ 0x1A, 0x95, 0xF6, 0x10, 0x63, 0x99, 0x80, 0x8E, 0xEB, 0x0E, 0x9F, 0x31, 0xDE, 0xB6, 0x6C, 0x05 },
	{ 0x1B, 0x01, 0x72, 0xCD, 0x73, 0x24, 0xA7, 0xD3, 0x53, 0x74, 0xD7, 0x5D, 0xAC, 0xC0, 0xCE, 0x6B },
	{ 0x1C, 0x6B, 0x68, 0x9B, 0x03, 0x91, 0x52, 0x83, 0xFD, 0xD1, 0xEC, 0x9A, 0x31, 0x41, 0x26, 0xA2 },
	{ 0x1D, 0xFF, 0xEC, 0x46, 0x13, 0x2C, 0x75, 0xDE, 0x45, 0xAB, 0xA4, 0xF6, 0x43, 0x37, 0x84, 0xCC },
	{ 0x1E, 0x80, 0xA3, 0xE2, 0x23, 0x28, 0x1C, 0x39, 0x4E, 0x25, 0x7C, 0x42, 0xD5, 0xAD, 0xA1, 0x7E },
	{ 0x1F, 0x14, 0x27, 0x3F, 0x33, 0x95, 0x3B, 0x64, 0xF6, 0x5F, 0x34, 0x2E, 0xA7, 0xDB, 0x03, 0x10 },
	{ 0x20, 0xA8, 0xED, 0x9C, 0x45, 0xC1, 0x6A, 0xF1, 0x61, 0x9B, 0x14, 0x1E, 0x58, 0xD8, 0xA7, 0x5E }
};
static const uint8_t GM[8][256] =
{
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
        0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
        0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
    },
    {
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
        0xC3, 0xD3, 0xE3, 0xF3, 0x83, 0x93, 0xA3, 0xB3, 0x43, 0x53, 0x63, 0x73, 0x03, 0x13, 0x23, 0x33,
        0x45, 0x55, 0x65, 0x75, 0x05, 0x15, 0x25, 0x35, 0xC5, 0xD5, 0xE5, 0xF5, 0x85, 0x95, 0xA5, 0xB5,
        0x86, 0x96, 0xA6, 0xB6, 0xC6, 0xD6, 0xE6, 0xF6, 0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76,
        0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xDA, 0xEA, 0xFA, 0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x7A,
        0x49, 0x59, 0x69, 0x79, 0x09, 0x19, 0x29, 0x39, 0xC9, 0xD9, 0xE9, 0xF9, 0x89, 0x99, 0xA9, 0xB9,
        0xCF, 0xDF, 0xEF, 0xFF, 0x8F, 0x9F, 0xAF, 0xBF, 0x4F, 0x5F, 0x6F, 0x7F, 0x0F, 0x1F, 0x2F, 0x3F,
        0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C, 0x6C, 0x7C, 0x8C, 0x9C, 0xAC, 0xBC, 0xCC, 0xDC, 0xEC, 0xFC,
        0xD7, 0xC7, 0xF7, 0xE7, 0x97, 0x87, 0xB7, 0xA7, 0x57, 0x47, 0x77, 0x67, 0x17, 0x07, 0x37, 0x27,
        0x14, 0x04, 0x34, 0x24, 0x54, 0x44, 0x74, 0x64, 0x94, 0x84, 0xB4, 0xA4, 0xD4, 0xC4, 0xF4, 0xE4,
        0x92, 0x82, 0xB2, 0xA2, 0xD2, 0xC2, 0xF2, 0xE2, 0x12, 0x02, 0x32, 0x22, 0x52, 0x42, 0x72, 0x62,
        0x51, 0x41, 0x71, 0x61, 0x11, 0x01, 0x31, 0x21, 0xD1, 0xC1, 0xF1, 0xE1, 0x91, 0x81, 0xB1, 0xA1,
        0x5D, 0x4D, 0x7D, 0x6D, 0x1D, 0x0D, 0x3D, 0x2D, 0xDD, 0xCD, 0xFD, 0xED, 0x9D, 0x8D, 0xBD, 0xAD,
        0x9E, 0x8E, 0xBE, 0xAE, 0xDE, 0xCE, 0xFE, 0xEE, 0x1E, 0x0E, 0x3E, 0x2E, 0x5E, 0x4E, 0x7E, 0x6E,
        0x18, 0x08, 0x38, 0x28, 0x58, 0x48, 0x78, 0x68, 0x98, 0x88, 0xB8, 0xA8, 0xD8, 0xC8, 0xF8, 0xE8,
        0xDB, 0xCB, 0xFB, 0xEB, 0x9B, 0x8B, 0xBB, 0xAB, 0x5B, 0x4B, 0x7B, 0x6B, 0x1B, 0x0B, 0x3B, 0x2B
    },
    {
        0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0, 0xC3, 0xE3, 0x83, 0xA3, 0x43, 0x63, 0x03, 0x23,
        0x45, 0x65, 0x05, 0x25, 0xC5, 0xE5, 0x85, 0xA5, 0x86, 0xA6, 0xC6, 0xE6, 0x06, 0x26, 0x46, 0x66,
        0x8A, 0xAA, 0xCA, 0xEA, 0x0A, 0x2A, 0x4A, 0x6A, 0x49, 0x69, 0x09, 0x29, 0xC9, 0xE9, 0x89, 0xA9,
        0xCF, 0xEF, 0x8F, 0xAF, 0x4F, 0x6F, 0x0F, 0x2F, 0x0C, 0x2C, 0x4C, 0x6C, 0x8C, 0xAC, 0xCC, 0xEC,
        0xD7, 0xF7, 0x97, 0xB7, 0x57, 0x77, 0x17, 0x37, 0x14, 0x34, 0x54, 0x74, 0x94, 0xB4, 0xD4, 0xF4,
        0x92, 0xB2, 0xD2, 0xF2, 0x12, 0x32, 0x52, 0x72, 0x51, 0x71, 0x11, 0x31, 0xD1, 0xF1, 0x91, 0xB1,
        0x5D, 0x7D, 0x1D, 0x3D, 0xDD, 0xFD, 0x9D, 0xBD, 0x9E, 0xBE, 0xDE, 0xFE, 0x1E, 0x3E, 0x5E, 0x7E,
        0x18, 0x38, 0x58, 0x78, 0x98, 0xB8, 0xD8, 0xF8, 0xDB, 0xFB, 0x9B, 0xBB, 0x5B, 0x7B, 0x1B, 0x3B,
        0x6D, 0x4D, 0x2D, 0x0D, 0xED, 0xCD, 0xAD, 0x8D, 0xAE, 0x8E, 0xEE, 0xCE, 0x2E, 0x0E, 0x6E, 0x4E,
        0x28, 0x08, 0x68, 0x48, 0xA8, 0x88, 0xE8, 0xC8, 0xEB, 0xCB, 0xAB, 0x8B, 0x6B, 0x4B, 0x2B, 0x0B,
        0xE7, 0xC7, 0xA7, 0x87, 0x67, 0x47, 0x27, 0x07, 0x24, 0x04, 0x64, 0x44, 0xA4, 0x84, 0xE4, 0xC4,
        0xA2, 0x82, 0xE2, 0xC2, 0x22, 0x02, 0x62, 0x42, 0x61, 0x41, 0x21, 0x01, 0xE1, 0xC1, 0xA1, 0x81,
        0xBA, 0x9A, 0xFA, 0xDA, 0x3A, 0x1A, 0x7A, 0x5A, 0x79, 0x59, 0x39, 0x19, 0xF9, 0xD9, 0xB9, 0x99,
        0xFF, 0xDF, 0xBF, 0x9F, 0x7F, 0x5F, 0x3F, 0x1F, 0x3C, 0x1C, 0x7C, 0x5C, 0xBC, 0x9C, 0xFC, 0xDC,
        0x30, 0x10, 0x70, 0x50, 0xB0, 0x90, 0xF0, 0xD0, 0xF3, 0xD3, 0xB3, 0x93, 0x73, 0x53, 0x33, 0x13,
        0x75, 0x55, 0x35, 0x15, 0xF5, 0xD5, 0xB5, 0x95, 0xB6, 0x96, 0xF6, 0xD6, 0x36, 0x16, 0x76, 0x56
    },
    {
        0x00, 0xC0, 0x43, 0x83, 0x86, 0x46, 0xC5, 0x05, 0xCF, 0x0F, 0x8C, 0x4C, 0x49, 0x89, 0x0A, 0xCA,
        0x5D, 0x9D, 0x1E, 0xDE, 0xDB, 0x1B, 0x98, 0x58, 0x92, 0x52, 0xD1, 0x11, 0x14, 0xD4, 0x57, 0x97,
        0xBA, 0x7A, 0xF9, 0x39, 0x3C, 0xFC, 0x7F, 0xBF, 0x75, 0xB5, 0x36, 0xF6, 0xF3, 0x33, 0xB0, 0x70,
        0xE7, 0x27, 0xA4, 0x64, 0x61, 0xA1, 0x22, 0xE2, 0x28, 0xE8, 0x6B, 0xAB, 0xAE, 0x6E, 0xED, 0x2D,
        0xB7, 0x77, 0xF4, 0x34, 0x31, 0xF1, 0x72, 0xB2, 0x78, 0xB8, 0x3B, 0xFB, 0xFE, 0x3E, 0xBD, 0x7D,
        0xEA, 0x2A, 0xA9, 0x69, 0x6C, 0xAC, 0x2F, 0xEF, 0x25, 0xE5, 0x66, 0xA6, 0xA3, 0x63, 0xE0, 0x20,
        0x0D, 0xCD, 0x4E, 0x8E, 0x8B, 0x4B, 0xC8, 0x08, 0xC2, 0x02, 0x81, 0x41, 0x44, 0x84, 0x07, 0xC7,
        0x50, 0x90, 0x13, 0xD3, 0xD6, 0x16, 0x95, 0x55, 0x9F, 0x5F, 0xDC, 0x1C, 0x19, 0xD9, 0x5A, 0x9A,
        0xAD, 0x6D, 0xEE, 0x2E, 0x2B, 0xEB, 0x68, 0xA8, 0x62, 0xA2, 0x21, 0xE1, 0xE4, 0x24, 0xA7, 0x67,
        0xF0, 0x30, 0xB3, 0x73, 0x76, 0xB6, 0x35, 0xF5, 0x3F, 0xFF, 0x7C, 0xBC, 0xB9, 0x79, 0xFA, 0x3A,
        0x17, 0xD7, 0x54, 0x94, 0x91, 0x51, 0xD2, 0x12, 0xD8, 0x18, 0x9B, 0x5B, 0x5E, 0x9E, 0x1D, 0xDD,
        0x4A, 0x8A, 0x09, 0xC9, 0xCC, 0x0C, 0x8F, 0x4F, 0x85, 0x45, 0xC6, 0x06, 0x03, 0xC3, 0x40, 0x80,
        0x1A, 0xDA, 0x59, 0x99, 0x9C, 0x5C, 0xDF, 0x1F, 0xD5, 0x15, 0x96, 0x56, 0x53, 0x93, 0x10, 0xD0,
        0x47, 0x87, 0x04, 0xC4, 0xC1, 0x01, 0x82, 0x42, 0x88, 0x48, 0xCB, 0x0B, 0x0E, 0xCE, 0x4D, 0x8D,
        0xA0, 0x60, 0xE3, 0x23, 0x26, 0xE6, 0x65, 0xA5, 0x6F, 0xAF, 0x2C, 0xEC, 0xE9, 0x29, 0xAA, 0x6A,
        0xFD, 0x3D, 0xBE, 0x7E, 0x7B, 0xBB, 0x38, 0xF8, 0x32, 0xF2, 0x71, 0xB1, 0xB4, 0x74, 0xF7, 0x37
    },
    {
        0x00, 0xC2, 0x47, 0x85, 0x8E, 0x4C, 0xC9, 0x0B, 0xDF, 0x1D, 0x98, 0x5A, 0x51, 0x93, 0x16, 0xD4,
        0x7D, 0xBF, 0x3A, 0xF8, 0xF3, 0x31, 0xB4, 0x76, 0xA2, 0x60, 0xE5, 0x27, 0x2C, 0xEE, 0x6B, 0xA9,
        0xFA, 0x38, 0xBD, 0x7F, 0x74, 0xB6, 0x33, 0xF1, 0x25, 0xE7, 0x62, 0xA0, 0xAB, 0x69, 0xEC, 0x2E,
        0x87, 0x45, 0xC0, 0x02, 0x09, 0xCB, 0x4E, 0x8C, 0x58, 0x9A, 0x1F, 0xDD, 0xD6, 0x14, 0x91, 0x53,
        0x37, 0xF5, 0x70, 0xB2, 0xB9, 0x7B, 0xFE, 0x3C, 0xE8, 0x2A, 0xAF, 0x6D, 0x66, 0xA4, 0x21, 0xE3,
        0x4A, 0x88, 0x0D, 0xCF, 0xC4, 0x06, 0x83, 0x41, 0x95, 0x57, 0xD2, 0x10, 0x1B, 0xD9, 0x5C, 0x9E,
        0xCD, 0x0F, 0x8A, 0x48, 0x43, 0x81, 0x04, 0xC6, 0x12, 0xD0, 0x55, 0x97, 0x9C, 0x5E, 0xDB, 0x19,
        0xB0, 0x72, 0xF7, 0x35, 0x3E, 0xFC, 0x79, 0xBB, 0x6F, 0xAD, 0x28, 0xEA, 0xE1, 0x23, 0xA6, 0x64,
        0x6E, 0xAC, 0x29, 0xEB, 0xE0, 0x22, 0xA7, 0x65, 0xB1, 0x73, 0xF6, 0x34, 0x3F, 0xFD, 0x78, 0xBA,
        0x13, 0xD1, 0x54, 0x96, 0x9D, 0x5F, 0xDA, 0x18, 0xCC, 0x0E, 0x8B, 0x49, 0x42, 0x80, 0x05, 0xC7,
        0x94, 0x56, 0xD3, 0x11, 0x1A, 0xD8, 0x5D, 0x9F, 0x4B, 0x89, 0x0C, 0xCE, 0xC5, 0x07, 0x82, 0x40,
        0xE9, 0x2B, 0xAE, 0x6C, 0x67, 0xA5, 0x20, 0xE2, 0x36, 0xF4, 0x71, 0xB3, 0xB8, 0x7A, 0xFF, 0x3D,
        0x59, 0x9B, 0x1E, 0xDC, 0xD7, 0x15, 0x90, 0x52, 0x86, 0x44, 0xC1, 0x03, 0x08, 0xCA, 0x4F, 0x8D,
        0x24, 0xE6, 0x63, 0xA1, 0xAA, 0x68, 0xED, 0x2F, 0xFB, 0x39, 0xBC, 0x7E, 0x75, 0xB7, 0x32, 0xF0,
        0xA3, 0x61, 0xE4, 0x26, 0x2D, 0xEF, 0x6A, 0xA8, 0x7C, 0xBE, 0x3B, 0xF9, 0xF2, 0x30, 0xB5, 0x77,
        0xDE, 0x1C, 0x99, 0x5B, 0x50, 0x92, 0x17, 0xD5, 0x01, 0xC3, 0x46, 0x84, 0x8F, 0x4D, 0xC8, 0x0A
    },
    {
        0x00, 0x85, 0xC9, 0x4C, 0x51, 0xD4, 0x98, 0x1D, 0xA2, 0x27, 0x6B, 0xEE, 0xF3, 0x76, 0x3A, 0xBF,
        0x87, 0x02, 0x4E, 0xCB, 0xD6, 0x53, 0x1F, 0x9A, 0x25, 0xA0, 0xEC, 0x69, 0x74, 0xF1, 0xBD, 0x38,
        0xCD, 0x48, 0x04, 0x81, 0x9C, 0x19, 0x55, 0xD0, 0x6F, 0xEA, 0xA6, 0x23, 0x3E, 0xBB, 0xF7, 0x72,
        0x4A, 0xCF, 0x83, 0x06, 0x1B, 0x9E, 0xD2, 0x57, 0xE8, 0x6D, 0x21, 0xA4, 0xB9, 0x3C, 0x70, 0xF5,
        0x59, 0xDC, 0x90, 0x15, 0x08, 0x8D, 0xC1, 0x44, 0xFB, 0x7E, 0x32, 0xB7, 0xAA, 0x2F, 0x63, 0xE6,
        0xDE, 0x5B, 0x17, 0x92, 0x8F, 0x0A, 0x46, 0xC3, 0x7C, 0xF9, 0xB5, 0x30, 0x2D, 0xA8, 0xE4, 0x61,
        0x94, 0x11, 0x5D, 0xD8, 0xC5, 0x40, 0x0C, 0x89, 0x36, 0xB3, 0xFF, 0x7A, 0x67, 0xE2, 0xAE, 0x2B,
        0x13, 0x96, 0xDA, 0x5F, 0x42, 0xC7, 0x8B, 0x0E, 0xB1, 0x34, 0x78, 0xFD, 0xE0, 0x65, 0x29, 0xAC,
        0xB2, 0x37, 0x7B, 0xFE, 0xE3, 0x66, 0x2A, 0xAF, 0x10, 0x95, 0xD9, 0x5C, 0x41, 0xC4, 0x88, 0x0D,
        0x35, 0xB0, 0xFC, 0x79, 0x64, 0xE1, 0xAD, 0x28, 0x97, 0x12, 0x5E, 0xDB, 0xC6, 0x43, 0x0F, 0x8A,
        0x7F, 0xFA, 0xB6, 0x33, 0x2E, 0xAB, 0xE7, 0x62, 0xDD, 0x58, 0x14, 0x91, 0x8C, 0x09, 0x45, 0xC0,
        0xF8, 0x7D, 0x31, 0xB4, 0xA9, 0x2C, 0x60, 0xE5, 0x5A, 0xDF, 0x93, 0x16, 0x0B, 0x8E, 0xC2, 0x47,
        0xEB, 0x6E, 0x22, 0xA7, 0xBA, 0x3F, 0x73, 0xF6, 0x49, 0xCC, 0x80, 0x05, 0x18, 0x9D, 0xD1, 0x54,
        0x6C, 0xE9, 0xA5, 0x20, 0x3D, 0xB8, 0xF4, 0x71, 0xCE, 0x4B, 0x07, 0x82, 0x9F, 0x1A, 0x56, 0xD3,
        0x26, 0xA3, 0xEF, 0x6A, 0x77, 0xF2, 0xBE, 0x3B, 0x84, 0x01, 0x4D, 0xC8, 0xD5, 0x50, 0x1C, 0x99,
        0xA1, 0x24, 0x68, 0xED, 0xF0, 0x75, 0x39, 0xBC, 0x03, 0x86, 0xCA, 0x4F, 0x52, 0xD7, 0x9B, 0x1E
    },
    {
        0x00, 0x94, 0xEB, 0x7F, 0x15, 0x81, 0xFE, 0x6A, 0x2A, 0xBE, 0xC1, 0x55, 0x3F, 0xAB, 0xD4, 0x40,
        0x54, 0xC0, 0xBF, 0x2B, 0x41, 0xD5, 0xAA, 0x3E, 0x7E, 0xEA, 0x95, 0x01, 0x6B, 0xFF, 0x80, 0x14,
        0xA8, 0x3C, 0x43, 0xD7, 0xBD, 0x29, 0x56, 0xC2, 0x82, 0x16, 0x69, 0xFD, 0x97, 0x03, 0x7C, 0xE8,
        0xFC, 0x68, 0x17, 0x83, 0xE9, 0x7D, 0x02, 0x96, 0xD6, 0x42, 0x3D, 0xA9, 0xC3, 0x57, 0x28, 0xBC,
        0x93, 0x07, 0x78, 0xEC, 0x86, 0x12, 0x6D, 0xF9, 0xB9, 0x2D, 0x52, 0xC6, 0xAC, 0x38, 0x47, 0xD3,
        0xC7, 0x53, 0x2C, 0xB8, 0xD2, 0x46, 0x39, 0xAD, 0xED, 0x79, 0x06, 0x92, 0xF8, 0x6C, 0x13, 0x87,
        0x3B, 0xAF, 0xD0, 0x44, 0x2E, 0xBA, 0xC5, 0x51, 0x11, 0x85, 0xFA, 0x6E, 0x04, 0x90, 0xEF, 0x7B,
        0x6F, 0xFB, 0x84, 0x10, 0x7A, 0xEE, 0x91, 0x05, 0x45, 0xD1, 0xAE, 0x3A, 0x50, 0xC4, 0xBB, 0x2F,
        0xE5, 0x71, 0x0E, 0x9A, 0xF0, 0x64, 0x1B, 0x8F, 0xCF, 0x5B, 0x24, 0xB0, 0xDA, 0x4E, 0x31, 0xA5,
        0xB1, 0x25, 0x5A, 0xCE, 0xA4, 0x30, 0x4F, 0xDB, 0x9B, 0x0F, 0x70, 0xE4, 0x8E, 0x1A, 0x65, 0xF1,
        0x4D, 0xD9, 0xA6, 0x32, 0x58, 0xCC, 0xB3, 0x27, 0x67, 0xF3, 0x8C, 0x18, 0x72, 0xE6, 0x99, 0x0D,
        0x19, 0x8D, 0xF2, 0x66, 0x0C, 0x98, 0xE7, 0x73, 0x33, 0xA7, 0xD8, 0x4C, 0x26, 0xB2, 0xCD, 0x59,
        0x76, 0xE2, 0x9D, 0x09, 0x63, 0xF7, 0x88, 0x1C, 0x5C, 0xC8, 0xB7, 0x23, 0x49, 0xDD, 0xA2, 0x36,
        0x22, 0xB6, 0xC9, 0x5D, 0x37, 0xA3, 0xDC, 0x48, 0x08, 0x9C, 0xE3, 0x77, 0x1D, 0x89, 0xF6, 0x62,
        0xDE, 0x4A, 0x35, 0xA1, 0xCB, 0x5F, 0x20, 0xB4, 0xF4, 0x60, 0x1F, 0x8B, 0xE1, 0x75, 0x0A, 0x9E,
        0x8A, 0x1E, 0x61, 0xF5, 0x9F, 0x0B, 0x74, 0xE0, 0xA0, 0x34, 0x4B, 0xDF, 0xB5, 0x21, 0x5E, 0xCA
    },
    {
        0x00, 0xFB, 0x35, 0xCE, 0x6A, 0x91, 0x5F, 0xA4, 0xD4, 0x2F, 0xE1, 0x1A, 0xBE, 0x45, 0x8B, 0x70,
        0x6B, 0x90, 0x5E, 0xA5, 0x01, 0xFA, 0x34, 0xCF, 0xBF, 0x44, 0x8A, 0x71, 0xD5, 0x2E, 0xE0, 0x1B,
        0xD6, 0x2D, 0xE3, 0x18, 0xBC, 0x47, 0x89, 0x72, 0x02, 0xF9, 0x37, 0xCC, 0x68, 0x93, 0x5D, 0xA6,
        0xBD, 0x46, 0x88, 0x73, 0xD7, 0x2C, 0xE2, 0x19, 0x69, 0x92, 0x5C, 0xA7, 0x03, 0xF8, 0x36, 0xCD,
        0x6F, 0x94, 0x5A, 0xA1, 0x05, 0xFE, 0x30, 0xCB, 0xBB, 0x40, 0x8E, 0x75, 0xD1, 0x2A, 0xE4, 0x1F,
        0x04, 0xFF, 0x31, 0xCA, 0x6E, 0x95, 0x5B, 0xA0, 0xD0, 0x2B, 0xE5, 0x1E, 0xBA, 0x41, 0x8F, 0x74,
        0xB9, 0x42, 0x8C, 0x77, 0xD3, 0x28, 0xE6, 0x1D, 0x6D, 0x96, 0x58, 0xA3, 0x07, 0xFC, 0x32, 0xC9,
        0xD2, 0x29, 0xE7, 0x1C, 0xB8, 0x43, 0x8D, 0x76, 0x06, 0xFD, 0x33, 0xC8, 0x6C, 0x97, 0x59, 0xA2,
        0xDE, 0x25, 0xEB, 0x10, 0xB4, 0x4F, 0x81, 0x7A, 0x0A, 0xF1, 0x3F, 0xC4, 0x60, 0x9B, 0x55, 0xAE,
        0xB5, 0x4E, 0x80, 0x7B, 0xDF, 0x24, 0xEA, 0x11, 0x61, 0x9A, 0x54, 0xAF, 0x0B, 0xF0, 0x3E, 0xC5,
        0x08, 0xF3, 0x3D, 0xC6, 0x62, 0x99, 0x57, 0xAC, 0xDC, 0x27, 0xE9, 0x12, 0xB6, 0x4D, 0x83, 0x78,
        0x63, 0x98, 0x56, 0xAD, 0x09, 0xF2, 0x3C, 0xC7, 0xB7, 0x4C, 0x82, 0x79, 0xDD, 0x26, 0xE8, 0x13,
        0xB1, 0x4A, 0x84, 0x7F, 0xDB, 0x20, 0xEE, 0x15, 0x65, 0x9E, 0x50, 0xAB, 0x0F, 0xF4, 0x3A, 0xC1,
        0xDA, 0x21, 0xEF, 0x14, 0xB0, 0x4B, 0x85, 0x7E, 0x0E, 0xF5, 0x3B, 0xC0, 0x64, 0x9F, 0x51, 0xAA,
        0x67, 0x9C, 0x52, 0xA9, 0x0D, 0xF6, 0x38, 0xC3, 0xB3, 0x48, 0x86, 0x7D, 0xD9, 0x22, 0xEC, 0x17,
        0x0C, 0xF7, 0x39, 0xC2, 0x66, 0x9D, 0x53, 0xA8, 0xD8, 0x23, 0xED, 0x16, 0xB2, 0x49, 0x87, 0x7C
    }
};
static const uint8_t K_POS[256] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00
};
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

void keySchedule(const uint8_t pwd[], const uint8_t bits, gost_key *key)
{
    if(bits == 64)
    {
        key->bits = bits;
        for(int i = 7, pwd_i = 0; i >= 0; i--, pwd_i++)
        {
            key->rd_key_64[i][3] = key->rd_key_64[i + 8][3] = key->rd_key_64[i + 16][3] = pwd[pwd_i * 4 + 3];
            key->rd_key_64[i][2] = key->rd_key_64[i + 8][2] = key->rd_key_64[i + 16][2] = pwd[pwd_i * 4 + 2];
            key->rd_key_64[i][1] = key->rd_key_64[i + 8][1] = key->rd_key_64[i + 16][1] = pwd[pwd_i * 4 + 1];
            key->rd_key_64[i][0] = key->rd_key_64[i + 8][0] = key->rd_key_64[i + 16][0] = pwd[pwd_i * 4];
        }

        for(int i = 7; i >= 0; i--)
        {
            key->rd_key_64[i + 24][3] = key->rd_key_64[7 - i][3];
            key->rd_key_64[i + 24][2] = key->rd_key_64[7 - i][2];
            key->rd_key_64[i + 24][1] = key->rd_key_64[7 - i][1];
            key->rd_key_64[i + 24][0] = key->rd_key_64[7 - i][0];
        }
    }
    else if(bits == 128)
    {
        key->bits = bits;
        uint8_t tmp[GOST128_ROUND_KEY_SIZE] = {0};
        memcpy_s(key->rd_key_128[0], GOST128_ROUND_KEY_SIZE, pwd + GOST128_ROUND_KEY_SIZE, GOST128_ROUND_KEY_SIZE);
        memcpy_s(key->rd_key_128[1], GOST128_ROUND_KEY_SIZE, pwd, GOST128_ROUND_KEY_SIZE);

        for(size_t i = 1; i < 5; i++)
        {
            memcpy_s(key->rd_key_128[2 * i + 1], GOST128_ROUND_KEY_SIZE, key->rd_key_128[2 * i - 2], GOST128_ROUND_KEY_SIZE);
            memcpy_s(key->rd_key_128[2 * i], GOST128_ROUND_KEY_SIZE, key->rd_key_128[2 * i - 1], GOST128_ROUND_KEY_SIZE);
            for(size_t j = 0; j < 8; j++)
            {
                F128(key->rd_key_128[2 * i + 1], key->rd_key_128[2 * i], C128[8 * (i - 1) + j]);
            }
            memcpy_s(tmp, GOST128_ROUND_KEY_SIZE, key->rd_key_128[2 * i + 1], GOST128_ROUND_KEY_SIZE);
            memcpy_s(key->rd_key_128[2 * i + 1], GOST128_ROUND_KEY_SIZE, key->rd_key_128[2 * i], GOST128_ROUND_KEY_SIZE);
            memcpy_s(key->rd_key_128[2 * i], GOST128_ROUND_KEY_SIZE, tmp, GOST128_ROUND_KEY_SIZE);
        }
        memset(tmp, 0x00, GOST128_ROUND_KEY_SIZE);
    }
}

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

void gost_r3412_64_encrypt_block(const BLOCK64 in, BLOCK64 out, const gost_key *key)
{
    BLOCK32 a0 = {0}, a1 = {0};
    memcpy_s(a0, BLOCK32SIZE, in, BLOCK32SIZE);
    memcpy_s(a1, BLOCK32SIZE, in + BLOCK32SIZE, BLOCK32SIZE);

    for(size_t i = 0; i < GOST64_ROUNDS_COUNT - 1; i++)
    {
        G64(a1, a0, key->rd_key_64[i]);
    }
    G64Star(a1, a0, key->rd_key_64[GOST64_ROUNDS_COUNT - 1], out);

    memset(a1, 0x00, BLOCK32SIZE);
    memset(a0, 0x00, BLOCK32SIZE);
}
void gost_r3412_64_decrypt_block(const BLOCK64 in, BLOCK64 out, const gost_key *key)
{
    BLOCK32 a0 = {0}, a1 = {0};
    memcpy_s(a0, BLOCK32SIZE, in, BLOCK32SIZE);
    memcpy_s(a1, BLOCK32SIZE, in + BLOCK32SIZE, BLOCK32SIZE);

    for(size_t i = GOST64_ROUNDS_COUNT - 1; i >= 1; i--)
    {
        G64(a1, a0, key->rd_key_64[i]);
    }
    G64Star(a1, a0, key->rd_key_64[0], out);

    memset(a1, 0x00, BLOCK32SIZE);
    memset(a0, 0x00, BLOCK32SIZE);
}

void X128(const BLOCK128 a, const BLOCK128 b, BLOCK128 out)
{
	uint64_t *_a = (uint64_t*)a;
	uint64_t *_b = (uint64_t*)b;
	uint64_t *_out = (uint64_t*)out;
	*_out++ = *_a++ ^ *_b++;
	*_out++ = *_a++ ^ *_b++;
    _out = _a = _b = NULL;
}
void S128(BLOCK128 a)
{
	for(size_t i = 0; i < BLOCK128SIZE; i++)
	{
		a[i] = pi128[a[i]];
	}
}
void S128inv(BLOCK128 a)
{
	for(size_t i = 0; i < BLOCK128SIZE; i++)
	{
		a[i] = invPi128[a[i]];
	}
}
void R128(BLOCK128 a)
{
	uint8_t tmp = 0;
	for(size_t i = 0; i < BLOCK128SIZE; i++)
	{
        tmp ^= GM[K_POS[coeffL128[i]]][a[i]];
	}
    memcpy_s(a, BLOCK128SIZE - 1, a + 1, BLOCK128SIZE - 1);
    a[BLOCK128SIZE - 1] = tmp;
    tmp = 0x00;
}
void R128inv(BLOCK128 a)
{
    uint8_t tmp = a[BLOCK128SIZE - 1];
    memmove_s(a + 1, BLOCK128SIZE - 1, a, BLOCK128SIZE - 1);
    a[0] = tmp;
	tmp = 0;
	for(size_t i = 0; i < BLOCK128SIZE; i++)
	{
        tmp ^= GM[K_POS[coeffL128[i]]][a[i]];
	}
    a[0] = tmp;
    tmp = 0x00;
}
void L128(BLOCK128 a)
{
	for(size_t i = 0; i < BLOCK128SIZE; i++)
	{
        R128(a);
	}
}
void L128inv(BLOCK128 a)
{
	for(size_t i = 0; i < BLOCK128SIZE; i++)
	{
        R128inv(a);
	}
}

void F128(BLOCK128 a, BLOCK128 a1, const BLOCK128 c)
{
    BLOCK128 tmp = {0};
    memcpy_s(tmp, BLOCK128SIZE, a, BLOCK128SIZE);

    X128(a, c, a);
    S128(a);
    L128(a);
    X128(a, a1, a);
    memcpy_s(a1, BLOCK128SIZE, tmp, BLOCK128SIZE);
	memset(tmp, 0x00, BLOCK128SIZE);
}

void gost_r3412_128_encrypt_block(const BLOCK128 in, BLOCK128 out, const gost_key *key)
{
    memcpy_s(out, BLOCK128SIZE, in, BLOCK128SIZE);
    for(size_t i = 0; i < GOST128_ROUNDS_COUNT - 1; i++)
	{
        X128(out, key->rd_key_128[i], out);
        S128(out);
        L128(out);
	}
    X128(out, key->rd_key_128[GOST128_ROUNDS_COUNT - 1], out);
}
void gost_r3412_128_decrypt_block(const BLOCK128 in, BLOCK128 out, const gost_key *key)
{
    memcpy_s(out, BLOCK128SIZE, in, BLOCK128SIZE);
    for(size_t i = GOST128_ROUNDS_COUNT - 1; i > 0; i--)
	{
        X128(out, key->rd_key_128[i], out);
        L128inv(out);
        S128inv(out);
	}
    X128(out, key->rd_key_128[0], out);
}
