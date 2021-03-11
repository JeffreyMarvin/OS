#include "pit.h"
#include "x86io.h"
#include "text_console.h"

extern Text_Console GlobalConsole;

namespace PIT{
    double TimeSinceBoot = 0;

    const uint16_t min_divisor = Base_Frequency / 10000; //Minimum of ~0.1 ms / tick
    const uint16_t max_divisor = 0xFFFF; //Minimum of ~18 Hz
    uint16_t divisor = 65535;

    void sleep(uint64_t milliseconds){
        double seconds  = (double)milliseconds / 1000;
        double startTime = TimeSinceBoot;
        while (TimeSinceBoot < startTime + seconds){
            asm("hlt");
        }
    }

    void set_divisor(uint32_t div){
        if (div < min_divisor) div = min_divisor;
        if (div > max_divisor) div = max_divisor;
        divisor = div;
        outb(0x40, (uint8_t)(divisor & 0x00ff));
        io_wait();
        outb(0x40, (uint8_t)((divisor & 0xff00) >> 8));
    }

    uint64_t get_frequency(){
        return Base_Frequency / divisor;
    }

    void set_frequency(uint64_t frequency){
        set_divisor(Base_Frequency / frequency);
    }

    void tick(){
        TimeSinceBoot += (1.0 / (double)get_frequency());
    }
}