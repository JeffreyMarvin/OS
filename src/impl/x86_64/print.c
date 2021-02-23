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
    col++;

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
}

void print_str(char* string) {
    for (int i = 0; string[i] > 0; i++){
        print_char(string[i]);
    }
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground | background << 4;
}