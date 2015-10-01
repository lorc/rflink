cmake_minimum_required (VERSION 2.8.11)
project (Demo)

SET(_definitions STM32F103xB;)

# Set the build definitions
SET(BUILD_DEFINITIONS_TARGET "")
SET(BUILD_DEFINITIONS_DEBUG "")
SET(BUILD_DEFINITIONS_RELEASE "")

SET(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_DIR}/toolchain)
SET(MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/modules)
SET(DRIVERS_DIR ${CMAKE_CURRENT_LIST_DIR}/drivers)
SET(LIBRARIES_DIR ${CMAKE_CURRENT_LIST_DIR}/libs)

enable_language(ASM)

# Toolchain cmake includes
include(${TOOLCHAIN_DIR}/toolchain.cmake)

# Library cmake includes
include(${LIBRARIES_DIR}/HAL/HAL.cmake)
include(${LIBRARIES_DIR}/CMSIS/CMSIS.cmake)

# Programmer
include(${TOOLCHAIN_DIR}/stlink.cmake)

# Current directory includes
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_definitions("-DSTM32F103xB")

add_executable (
    ${CMAKE_PROJECT_NAME}.elf
    main.c
)

target_link_libraries (
    ${CMAKE_PROJECT_NAME}.elf
    CMSIS
    HAL
)

