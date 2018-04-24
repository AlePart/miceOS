#include "basic_allocator.h"

#include <stdint.h>


typedef uint32_t Word;
static const size_t WORD_SIZE = sizeof(Word);

static void* memory_start;
static Segment* free_head;
static Segment* used_head;

static void linked_list_pop(Segment **head, Segment *segment)
{
    if (segment->previous) {
        segment->previous->next = segment->next;
        segment->next->previous = segment->previous;
    } else if (segment->next) {
        segment->next->previous = NULL;
        *head = segment->next;
    } else {
        *head = NULL;
    }

    segment->previous = NULL;
    segment->next = NULL;
}

static void linked_list_push(Segment **head, Segment *segment)
{
    if (*head) {
        segment->previous = NULL;
        segment->next = *head;
        (*head)->previous = segment;
    } else {
        *head = segment;
        segment->previous = NULL;
        segment->next = NULL;
    }
}

static size_t linked_list_size(Segment *head)
{
    size_t result = 0;
    Segment* segment = head;
    while (segment) {
        ++result;
        segment = segment->next;
    }
    return result;
}

static uint8_t* segment_data_begin_address(Segment* segment) {
    return (uint8_t*)segment + sizeof(Segment);
}

void basic_allocator_initialize(void *start, size_t memory_bytes_size)
{
    memory_start = start;

    free_head = (Segment*) memory_start;
    free_head->size = memory_bytes_size - sizeof(Segment);
    free_head->previous = NULL;
    free_head->next = NULL;

    used_head = NULL;
}

void *basic_allocator_alloc(size_t bytes_to_alloc)
{
    // Round bytes to word size
    bytes_to_alloc = ((bytes_to_alloc / WORD_SIZE) + 1) * WORD_SIZE;

    Segment* current_segment = free_head;

    while (current_segment) {
        // Segment can't hold the request size...so try next
        if (current_segment->size < bytes_to_alloc) {
            current_segment = current_segment->next;
            continue;
        }

        // Segment is greater than requested...so we split it in two pieces and pop the first one
        if (current_segment->size > bytes_to_alloc) {
            uint8_t* remainder_start = (uint8_t*)current_segment + sizeof(Segment) + bytes_to_alloc;
            Segment* remainder = (Segment*) remainder_start;
            remainder->size = current_segment->size - bytes_to_alloc - sizeof(Segment);
            remainder->previous = current_segment;
            remainder->next = current_segment->next;

            current_segment->size = bytes_to_alloc;
            current_segment->next = remainder;
        }

        // Here or the current segment had the same size of the request or we splitted it
        // So we put it in the used list
        //assert(current_segment->size == bytes_to_alloc);

        linked_list_pop(&free_head, current_segment);
        linked_list_push(&used_head, current_segment);

        return segment_data_begin_address(current_segment);
    }

    return NULL;
}

void basic_allocator_free(void *address)
{
    Segment* segment = used_head;

    while (segment) {
        if (segment_data_begin_address(segment) == address) {
            linked_list_pop(&used_head, segment);
            linked_list_push(&free_head, segment);
            return;
        } else {
            segment = segment->next;
        }
    }
}

size_t basic_allocator_num_free_segments()
{
    return linked_list_size(free_head);
}

size_t basic_allocator_num_used_fregments()
{
    return linked_list_size(used_head);
}

Segment *basic_allocator_free_head()
{
    return free_head;
}

Segment *basic_allocator_used_head()
{
    return used_head;
}
