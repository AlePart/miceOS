set(TARGET_NAME miceOS.bin)

set(TARGET_SOURCES
    boot/boot.s
    kernel/memory/GDT/segment_reloading.s
    kernel/libc/kmemset.s
    kernel/kernel.c
    kernel/memory/allocator/basic_allocator.c
    kernel/memory/allocator/static_allocator.c
    kernel/memory/mmu/paging_management.c
    kernel/memory/GDT/GDT.c
)

set(TARGET_HEADERS
    kernel/memory/allocator/basic_allocator.h
     kernel/memory/allocator/static_allocator.h
    kernel/memory/mmu/paging_management.h
    kernel/memory/GDT/GDT.h
)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_link_libraries(${TARGET_NAME} gcc)
set_target_properties(${TARGET_NAME}
PROPERTIES
  LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
)
