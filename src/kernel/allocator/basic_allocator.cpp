#include "basic_allocator.h"

#define WORD size_t
#define WORD_SIZE sizeof(size_t)

namespace {

void* memory_start;
Segment* free_segment_list;
Segment* used_segment_list;

void pop_from_list(Segment*& head, Segment* segment)
{
    if (segment->previous) {
        segment->previous->next = segment->next;
    }
    else if (segment->next) {
        segment->next->previous = nullptr;
        head = segment->next;
    } else {
        head = nullptr;
    }

    segment->previous = nullptr;
    segment->next = nullptr;
}

void push_to_list(Segment*& head, Segment* segment)
{
    if (head) {
        head->next = segment;
        segment->previous = head;
        segment->next = nullptr;
    } else {
        head = segment;
        segment->previous = nullptr;
        segment->next = nullptr;
    }
}

size_t count_list(Segment* head)
{
    size_t result = 0;
    Segment* segment = head;
    while (segment) {
        ++result;
        segment = segment->next;
    }
    return result;
}

}

void BasicAllocator::initialize(void* start, size_t num_bytes) {
    ::memory_start = start;

    ::free_segment_list = reinterpret_cast<Segment*>(::memory_start);
    ::used_segment_list = nullptr;

    ::free_segment_list->size = num_bytes - 3 * sizeof(size_t); // Size
    ::free_segment_list->previous = 0; // Previous
    ::free_segment_list->next = 0; // Next
}

void* BasicAllocator::alloc(size_t bytes_to_alloc) {
    bytes_to_alloc = ((bytes_to_alloc/sizeof(size_t)) + 1) * sizeof(size_t);

    Segment* current_segment = free_segment_list;

    while (current_segment) {
        if (current_segment->size < bytes_to_alloc) {
            current_segment = current_segment->next;
            continue;
        }

        if (current_segment->size > bytes_to_alloc) {
            Segment* remainder = current_segment + 3 + (bytes_to_alloc / sizeof(size_t));
            remainder->size = current_segment->size - bytes_to_alloc - 3 * sizeof(size_t);
            remainder->previous = current_segment;
            remainder->next = current_segment->next;

            current_segment->size = bytes_to_alloc;
            current_segment->next = remainder;
        }

        ::pop_from_list(::free_segment_list, current_segment);
        ::push_to_list(::used_segment_list, current_segment);

        return current_segment + 3;
    }

    return nullptr;
}

void BasicAllocator::free(void* address) {
    Segment* segment = ::used_segment_list;
    while (segment) {
        if (segment + 3 == address) {
            ::pop_from_list(::used_segment_list, segment);
            ::push_to_list(::free_segment_list, segment);
            return;
        } else {
            segment = segment->next;
        }
    }
}

size_t BasicAllocator::num_free_segments()
{
    return count_list(::free_segment_list);
}

size_t BasicAllocator::num_used_segments()
{
    return count_list(::used_segment_list);
}

Segment *BasicAllocator::free_head()
{
    Segment * result = ::free_segment_list;
    return result;
}

Segment *BasicAllocator::used_head()
{
    Segment * result = ::used_segment_list;
    return result;
}
