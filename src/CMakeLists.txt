set(TARGET_NAME miceOS.bin)

set(TARGET_SOURCES
    boot/boot.s
    kernel.c
)

set(TARGET_HEADERS
)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_link_libraries(${TARGET_NAME} gcc)
set_target_properties(${TARGET_NAME}
PROPERTIES
  LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
)
