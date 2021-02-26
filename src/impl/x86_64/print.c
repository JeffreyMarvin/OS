#include "print.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;

size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row (size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };
    for(int col = 0; col < NUM_COLS; col++) {
        buffer[row * NUM_COLS + col] = empty;
    }
}

void print_newline() {
    col = 0;

    if(row++ < NUM_ROWS) {
        return;
    }

    row--;

    for(size_t r = 0; r < NUM_ROWS - 1; r++) {
        for(size_t c = 0; c < NUM_COLS; c++) {
            buffer[r * NUM_COLS + c] = buffer[(r + 1) * NUM_COLS + c];
        }
    }

    clear_row(row);
}

void print_clear() {
    for(size_t r = 0; r < NUM_ROWS; r++) {
        clear_row(r);
    }
}

void print_char(char character) {

    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    };
    col++;
}

void print_str(char* string) {
    for (int i = 0; string[i] > 0; i++){
        print_char(string[i]);
    }
}

void print_line(char* string) {
    print_str(string);
    print_newline();
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground | background << 4;
}

void print_nybble(uint8_t nybble) { 
    char c;
    if(nybble < 10){
        c = '0' + nybble;
    } else {
        c = 'A' + nybble - 10;
    }
    print_char(c);
}

void print_byte(uint8_t byte) {
    print_str("0x");
    for(int i = 1; i >= 0; i--) {
        print_nybble((byte >> (4 * i)) & 0x0F);
    }
}

void print_word(uint16_t word) {
    print_str("0x");
    for(int i = 3; i >= 0; i--) {
        print_nybble((word >> (4 * i)) & 0x0F);
    }
}

void print_double_word(uint32_t double_word) {
    print_str("0x");
    for(int i = 7; i >= 0; i--) {
        print_nybble((double_word >> (4 * i)) & 0x0F);
    }
}

void print_quad_word(uint64_t quad_word) {
    print_str("0x");
    for(int i = 15; i >= 0; i--) {
        print_nybble((quad_word >> (4 * i)) & 0x0F);
    }
}