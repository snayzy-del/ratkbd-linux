cmake_minimum_required(VERSION 3.10)
project(madvxe C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

include_directories(include)

add_executable(madctl
    src/main.c
    src/hid_raw.c
    src/keyboard_via.c
)
