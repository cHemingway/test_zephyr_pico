cmake_minimum_required(VERSION 3.20.0)

# Set board to rpi_pico, must be before find_package(Zephyr)
set(BOARD rpi_pico)

find_package(Zephyr)
project(test_zephyr_pico)

target_sources(app PRIVATE src/main.c)