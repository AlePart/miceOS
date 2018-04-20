set(TARGET_NAME miceOS-tests)

add_executable(${TARGET_NAME}
    main.c
    ../src/kernel/memory/allocator/basic_allocator.h
    ../src/kernel/memory/allocator/basic_allocator.c
)

include_directories(../src)

set_target_properties(${TARGET_NAME} PROPERTIES LANGUAGE C)
