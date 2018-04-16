set(TARGET_NAME miceOS-tests)
add_executable(${TARGET_NAME}
    ${CMAKE_SOURCE_DIR}/src/kernel/allocator/basic_allocator.cpp
    main.cpp
)
include_directories(${CMAKE_SOURCE_DIR}/src)
set_target_properties(${TARGET_NAME} PROPERTIES LANGUAGE CXX)
