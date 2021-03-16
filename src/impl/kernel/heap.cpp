#include "heap.h"
#include "page_frame_allocator.h"
#include "page_table_manager.h"

extern Page_Frame_Allocator GlobalAllocator;
extern Page_Table_Manager GlobalPageManager;

Heap_Segment_Header* first_segment;
Heap_Segment_Header* last_segment;
void* heap_start;
void* heap_end;

void initialize_heap(void* heap_address, size_t page_count){
    void* p = heap_address;
    for(size_t i = 0; i < page_count; i++){
        GlobalPageManager.map_memory(p, GlobalAllocator.request_page());
        p = (void*)((size_t)p + PAGE_SIZE);
    }

    size_t heap_length = page_count * PAGE_SIZE;

    heap_start = heap_address;
    heap_end = (void*)((size_t)heap_address + heap_length);

    first_segment = (Heap_Segment_Header*)heap_address;

    first_segment->length = heap_length - sizeof(Heap_Segment_Header);
    first_segment->next = NULL;
    first_segment->last = NULL;
    first_segment->free = true;

    last_segment = first_segment;
}

void* malloc(size_t size){
    if (size % 0x10 > 0){ //Ensure malloc size is 32-bit alligned
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) return NULL;

    Heap_Segment_Header* segment = first_segment;
    do {
        if(!segment->free){
            segment = segment->next;
            continue;
        }
        if(segment->length < size){
            segment = segment->next;
            continue;
        }
        if(segment->length > size){
            segment->split(size);
        }
        segment->free = false;
        return (void*)((size_t)segment + sizeof(Heap_Segment_Header));
    } while(segment != NULL);
    expand_heap(size);
    return malloc(size);
}

void free(void* address){
    Heap_Segment_Header* segment = (Heap_Segment_Header*) address;
    segment->free = true;
    segment->combine_forward();
    segment->combine_backward();
}

void expand_heap(size_t size){
    if(size % PAGE_SIZE) {
        size -= size % PAGE_SIZE;
        size += PAGE_SIZE;
    }

    size_t pageCount = size / 0x1000;
    Heap_Segment_Header* new_segment = (Heap_Segment_Header*)heap_end;

    for (size_t i = 0; i < pageCount; i++){
        GlobalPageManager.map_memory(heap_end, GlobalAllocator.request_page());
        heap_end = (void*)((size_t)heap_end + 0x1000);
    }

    new_segment->free = true;
    new_segment->last = last_segment;
    last_segment->next = new_segment;
    last_segment = new_segment;
    new_segment->next = NULL;
    new_segment->length = size - sizeof(Heap_Segment_Header);
    new_segment->combine_backward();
}

void Heap_Segment_Header::combine_forward(){
    if (this->next == NULL) return;
    if (!this->next->free) return;

    if (this->next->next != NULL){
        this->next->next->last = this;
    }
    if(last_segment == this->next) {
        last_segment = this;
    }

    this->next = this->next->next;

    this->length = this->length + this->next->length + sizeof(Heap_Segment_Header);
}

void Heap_Segment_Header::combine_backward(){
    if (last != NULL && last->free) last->combine_forward();
}

Heap_Segment_Header* Heap_Segment_Header::split(size_t size){
    Heap_Segment_Header* new_next = (Heap_Segment_Header*) ((size_t)this + size + sizeof(Heap_Segment_Header));

    if(last_segment == this) {
        last_segment = new_next;
    }

    new_next->length = this->length - size - sizeof(Heap_Segment_Header);
    new_next->next = this->next;
    new_next->last = this;
    new_next->free = this->free;

    this->length = size;
    this->next = new_next;
    this->next->last = new_next;

    return new_next;
}
