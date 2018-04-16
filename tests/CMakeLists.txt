set(TARGET_NAME miceOS-tests)
add_definitions(-DFORCE_WORD_SIZE=8)
add_executable(${TARGET_NAME}
    ${CMAKE_SOURCE_DIR}/src/kernel/allocator/basic_allocator.cpp
    main.cpp
)
include_directories(${CMAKE_SOURCE_DIR}/src)
set_target_properties(${TARGET_NAME} PROPERTIES LANGUAGE CXX)
