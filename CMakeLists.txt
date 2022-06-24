cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

project(RP2040TOPC98)
add_executable(RP2040TOPC98 main.c)
#pico_generate_pio_header(main ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
target_link_libraries(RP2040TOPC98 pico_stdlib)

pico_add_extra_outputs(RP2040TOPC98)