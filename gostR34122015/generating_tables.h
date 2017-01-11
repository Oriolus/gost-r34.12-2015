#pragma once

#include "r34122015.h"

#include <stdint.h>
#include <stdio.h>

uint16_t multiply(uint16_t lhs, uint16_t rhs);
void generateLinearTable();
void generateCoeffPositionTable();

void printB128(BLOCK128 a);
void print128key(uint8_t K128[10][16]);

