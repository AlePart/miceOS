#include <cassert>

#include "kernel/allocator/basic_allocator.h"



int main(int argc, char* argv[]) {
    size_t memory[1000];

    BasicAllocator::initialize(memory, 1024);
    assert(BasicAllocator::num_free_segments() == 1);
    assert(BasicAllocator::num_used_segments() == 0);
    assert(BasicAllocator::used_head() == nullptr);
    assert(BasicAllocator::free_head()->size == (1024 - 8 * 3));

    void* some_memory = BasicAllocator::alloc(63);

    assert(BasicAllocator::num_free_segments() == 1);
    assert(BasicAllocator::num_used_segments() == 1); 

    auto free_head = BasicAllocator::free_head();
    assert(BasicAllocator::free_head()->next == nullptr);
    assert(BasicAllocator::free_head()->previous == nullptr);
    assert(BasicAllocator::free_head()->size == (1024 - 8 * 3) - (64 + 8 * 3));

    auto used_head = BasicAllocator::used_head();
    assert(BasicAllocator::used_head()->next == nullptr);
    assert(BasicAllocator::used_head()->previous == nullptr);
    assert(BasicAllocator::used_head()->size == 64);

    auto t = sizeof(size_t);

    uint32_t* k = reinterpret_cast<uint32_t*>(some_memory);
    *k = 0xFFFF;


    BasicAllocator::free(some_memory);

    assert(BasicAllocator::num_free_segments() == 2);
    assert(BasicAllocator::num_used_segments() == 0);

    return 0;
}
