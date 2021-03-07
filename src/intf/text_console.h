#pragma once

#include <stdint.h>

struct Char {
    uint8_t character;
    uint8_t color;
};

class Text_Console {
    public:
        enum {
            PRINT_COLOR_BLACK = 0,
            PRINT_COLOR_BLUE = 1,
            PRINT_COLOR_GREEN = 2,
            PRINT_COLOR_CYAN = 3,
            PRINT_COLOR_RED = 4,
            PRINT_COLOR_MAGENTA = 5,
            PRINT_COLOR_BROWN = 6,
            PRINT_COLOR_LIGHT_GRAY = 7,
            PRINT_COLOR_DARK_GRAY = 8,
            PRINT_COLOR_LIGHT_BLUE = 9,
            PRINT_COLOR_LIGHT_GREEN = 10,
            PRINT_COLOR_LIGHT_CYAN = 11,
            PRINT_COLOR_LIGHT_RED = 12,
            PRINT_COLOR_PINK = 13,
            PRINT_COLOR_YELLOW = 14,
            PRINT_COLOR_WHITE = 15,
        };

        Text_Console(uint64_t framebuffer_addr = 0xb8000, uint32_t framebuffer_width = 80, uint32_t framebuffer_height = 25, uint8_t foreground = PRINT_COLOR_WHITE, uint8_t background = PRINT_COLOR_BLACK);

        void set_position(uint64_t x, uint64_t y);
        void set_x_position(uint64_t x);
        void set_y_position(uint64_t y);
        void set_color(uint8_t foreground, uint8_t background);

        void clear_screen();

        void print_str(const char* string);
        void print_line(const char* string = "");

        void print_hex(uint8_t num);
        void print_hex(uint16_t num);
        void print_hex(uint32_t num);
        void print_hex(uint64_t num);

        void print_num(uint8_t num);
        void print_num(uint16_t num);
        void print_num(uint32_t num);
        void print_num(uint64_t num);

    private:

    Char* buffer;

    uint64_t width;
    uint64_t height;
    uint8_t color;

    uint64_t col;
    uint64_t row;

        void print_char(char character);
        void print_newline();
        void clear_row (uint64_t r);
};