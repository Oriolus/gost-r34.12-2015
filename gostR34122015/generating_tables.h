#pragma once

#include "gost_2015.h"

#include <stdint.h>
#include <stdio.h>

uint16_t multiply(uint16_t lhs, uint16_t rhs);
void generateLinearTable();
void generateCoeffPositionTable();

void printKey(gost_key *key);

void printBas(uint8_t *a, size_t a_size);
void printBanySize(uint8_t *a, const size_t a_size, const size_t symbsPerStr);

void printB256(BLOCK256 a);
void printB128(BLOCK128 a);
void printB64(BLOCK64 a);
void printB32(BLOCK32 a);
