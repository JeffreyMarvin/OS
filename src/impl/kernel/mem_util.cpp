#include "mem_util.h"

void memset(void* start, uint8_t value, uint64_t num){
    for (uint64_t i = 0; i < num; i++){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}

bool memcmp(const void* a, const void* b, uint64_t bytes){
    uint8_t* char_a = (uint8_t*) a;
    uint8_t* char_b = (uint8_t*) b;
    while(bytes > 0){
        if(*char_a != *char_b){
            return false;
        }
        bytes--;
        char_a++;
        char_b++;
    }
    return true;
}
