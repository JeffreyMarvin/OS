#pragma once

#include <stdint.h>

namespace PIT {

    extern double TimeSinceBoot;
    const uint64_t Base_Frequency = 1193182;
    const uint8_t PIT_port = 0x20;

    void sleep(uint64_t milliseconds);

    void set_divisor(uint32_t divisor);
    uint64_t get_frequency();
    void set_frequency(uint64_t frequency);
    void tick();
}
