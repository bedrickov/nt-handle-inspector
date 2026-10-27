cmake_minimum_required(VERSION 3.20)
project(RingHandle VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(ringhandle_core STATIC
    src/handle_table.cpp
    src/nt_query.cpp
)

add_executable(ringhandle
    src/main.cpp
)
target_link_libraries(ringhandle PRIVATE ringhandle_core ntdll)

add_executable(ringhandle_tests
    tests/test_main.cpp
)
target_link_libraries(ringhandle_tests PRIVATE ringhandle_core ntdll)
