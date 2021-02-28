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

void clear_row (size_t r) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };
    for(int c = 0; c < NUM_COLS; c++) {
        buffer[r * NUM_COLS + c] = empty;
    }
}

void print_newline() {
    col = 0;

    if(row++ < NUM_ROWS) {
        return;
    }

    for(size_t r = 0; r < NUM_ROWS - 1; r++) {
        for(size_t c = 0; c < NUM_COLS; c++) {
            buffer[r * NUM_COLS + c] = buffer[(r + 1) * NUM_COLS + c];
        }
    }

    row = NUM_ROWS - 1;
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

void print_str(const char* string) {
    for (int i = 0; string[i] > 0; i++){
        print_char(string[i]);
    }
}

void print_line(const char* string) {
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

char hexToStringOutput[128];
template<typename T>
const char* hex_to_string(T num){
    T* valPtr = &num;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = (sizeof(T)) * 2 - 1;
    
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        temp = ((*ptr  >> 4) & 0x0F);
        hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
        temp = ((*ptr & 0x0F));
        hexToStringOutput[size - (i * 2)] = temp + (temp > 9 ? 55 : 48);
    }
    hexToStringOutput[size + 1] = 0;

    return hexToStringOutput;
}

void print_hex(uint8_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}
void print_hex(uint16_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}
void print_hex(uint32_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}
void print_hex(uint64_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}

char integerToStringOutput[128];
template<typename T>
const char* integer_to_string(T value) {

	uint8_t isNegative = 0;

	if (value < 0) {
		isNegative = 1;
		value *= -1;
		integerToStringOutput[0] = '-';
	}

	uint8_t size = 0;
	uint64_t sizeTester = (uint64_t)value;

	while (sizeTester / 10 > 0) {
		sizeTester /= 10;
		size++;
	}

	uint8_t index = 0;
	uint64_t newValue = (uint64_t)value;

	while (newValue / 10 > 0) {
		uint8_t remainder = newValue % 10;
		newValue /= 10;
		integerToStringOutput[isNegative + size - index] = remainder + 48; 
		index++;
	}

	uint8_t remainder = newValue % 10;
	integerToStringOutput[isNegative + size - index] = remainder + 48;
	integerToStringOutput[isNegative + size + 1] = 0;

	return integerToStringOutput;
}

void print_num(uint8_t num){
    print_str(integer_to_string(num));
}
void print_num(uint16_t num){
    print_str(integer_to_string(num));
}
void print_num(uint32_t num){
    print_str(integer_to_string(num));
}
void print_num(uint64_t num){
    print_str(integer_to_string(num));
}