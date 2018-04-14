cmake_minimum_required(VERSION 3.8)

project(miceOS-project C ASM)

if (${CMAKE_CROSSCOMPILING})
    add_subdirectory(src)
else()
    add_subdirectory(tests)
endif()
