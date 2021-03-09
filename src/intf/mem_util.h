#pragma once
#include <stdint.h>

void memset(void* start, uint8_t value, uint64_t num);
bool memcmp(const void* a, const void* b, uint64_t bytes);
