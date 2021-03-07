#include "text_console.h"

Text_Console::Text_Console(uint64_t framebuffer_addr, uint32_t framebuffer_width, uint32_t framebuffer_height, uint8_t foreground, uint8_t background) {
    buffer = (Char*) framebuffer_addr;
    width = framebuffer_width;
    height = framebuffer_height;
    set_color(foreground, background);
}

void Text_Console::set_position(uint64_t x, uint64_t y) {
    col = x < width? x : width - 1;
    row = y < height? y : height - 1;
}
void Text_Console::set_x_position(uint64_t x){
    set_position(x, row);
}
void Text_Console::set_y_position(uint64_t y){
    set_position(col, y);
}

void Text_Console::set_color(uint8_t foreground, uint8_t background){
color = foreground | background << 4;
}

void Text_Console::clear_row (uint64_t r) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };
    for(int c = 0; c < width; c++) {
        buffer[r * width + c] = empty;
    }
}

void Text_Console::print_newline() {
    col = 0;

    if(row++ < height) {
        return;
    }

    for(uint64_t r = 0; r < height - 1; r++) {
        for(uint64_t c = 0; c < width; c++) {
            buffer[r * width + c] = buffer[(r + 1) * width + c];
        }
    }

    row = height - 1;
    clear_row(row);
}

void Text_Console::clear_screen() {
    for(uint64_t r = 0; r < height; r++) {
        clear_row(r);
    }
    set_position(0,0);
}

void Text_Console::print_char(char character) {

    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > width) {
        print_newline();
    }

    buffer[col + width * row] = (Char) {
        character: (uint8_t) character,
        color: color,
    };
    col++;
}

void Text_Console::print_str(const char* string){
    for (int i = 0; string[i] > 0; i++){
        print_char(string[i]);
    }
}
void Text_Console::print_line(const char* string) {
    print_str(string);
    print_newline();
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

void Text_Console::print_hex(uint8_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}
void Text_Console::print_hex(uint16_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}
void Text_Console::print_hex(uint32_t num){
    print_str("0x");
    print_str(hex_to_string(num));
}
void Text_Console::print_hex(uint64_t num){
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

void Text_Console::print_num(uint8_t num){
    print_str(integer_to_string(num));
}
void Text_Console::print_num(uint16_t num){
    print_str(integer_to_string(num));
}
void Text_Console::print_num(uint32_t num){
    print_str(integer_to_string(num));
}
void Text_Console::print_num(uint64_t num){
    print_str(integer_to_string(num));
}

extern Text_Console GlobalConsole = Text_Console();
