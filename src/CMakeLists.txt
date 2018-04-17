set(TARGET_NAME miceOS.bin)

set(TARGET_SOURCES
    boot/boot.s
    kernel.cpp
    kernel/algorithm.cpp
    kernel/allocator/basic_allocator.cpp
    kernel/memory.cpp
    kernel/string.cpp
    kernel/terminal.cpp
)

set(TARGET_HEADERS
    kernel/algorithm.h
    kernel/memory.h
    kernel/string.h
    kernel/terminal.h
    kernel/include/boot/multiboot.h
)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_link_libraries(${TARGET_NAME} gcc)
set_target_properties(${TARGET_NAME}
PROPERTIES
  LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
)
