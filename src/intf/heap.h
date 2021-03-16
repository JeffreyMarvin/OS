#pragma once
#include <stddef.h>


struct Heap_Segment_Header{
    size_t length;
    Heap_Segment_Header* next;
    Heap_Segment_Header* last;
    bool free;
    void combine_forward();
    void combine_backward();
    Heap_Segment_Header* split(size_t size);
};

void initialize_heap(void* heap_address, size_t page_count);

void* malloc(size_t size);
void free(void* address);

void expand_heap(size_t size);

inline void* operator new(size_t size) {return malloc(size);}
inline void* operator new[](size_t size) {return malloc(size);}

inline void operator delete(void* p) {free(p);}
