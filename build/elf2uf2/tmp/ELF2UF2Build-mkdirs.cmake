# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-sdk-master/tools/elf2uf2"
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2"
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2"
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2/tmp"
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2/src/ELF2UF2Build-stamp"
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2/src"
  "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Users/WillowKoishi/Documents/2040py/C2040/pico-uart98/RP2040toPC98/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
