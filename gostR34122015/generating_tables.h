#pragma once

#include "r34122015.h"

#include <stdint.h>
#include <stdio.h>

uint16_t multiply(uint16_t lhs, uint16_t rhs);
void generateLinearTable();
void generateCoeffPositionTable();

void printB128(BLOCK128 a);
void print128key(gost128_key *key);

void printB64(BLOCK64 a);
void printB32(BLOCK32 a);
void print64key(gost64_key *key);
