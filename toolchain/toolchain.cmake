# Make sure CMAKE can find our custom toolchain file
SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/arm.cmake")

# Set the compiler executables to gcc-arm-embedded. These need to be on the system path
SET(CMAKE_C_COMPILER "arm-none-eabi-gcc")
SET(CMAKE_CXX_COMPILER
"arm-none-eabi-g++")
SET(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
SET(CMAKE_OBJCOPY "arm-none-eabi-objcopy")
SET(CMAKE_OBJDUMP "arm-none-eabi-objdump")
SET(CMAKE_RANLIB "arm-none-eabi-ranlib")
SET(CMAKE_AR "arm-none-eabi-ar")

# Set the common compiler flags
SET(CMAKE_C_FLAGS "-mthumb -fno-builtin -mcpu=cortex-m3 --specs=nano.specs -Wall -std=c99 -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-unroll-loops -ffast-math -ftree-vectorize -g")
SET(CMAKE_CXX_FLAGS "-mthumb -fno-builtin -mcpu=cortex-m3 --specs=nano.specs -Wall -Wno-unused-function -Wno-unused-local-typedefs -std=c++11 -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-unroll-loops -ffast-math -ftree-vectorize")

# Set the custom compiler flags for Debug and Release builds
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g3 ${BUILD_DEFINITIONS_DEBUG} ${BUILD_DEFINITIONS_TARGET} ${BUILD_DEFINITIONS}")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
SET(CMAKE_ASM_FLAGS_DEBUG "-g")

SET(CMAKE_C_FLAGS_RELEASE "-O0 ${BUILD_DEFINITIONS_RELEASE} ${BUILD_DEFINITIONS_TARGET} ${BUILD_DEFINITIONS}")
SET(CMAKE_CXX_FLAGS_RELEASE "-O0")
SET(CMAKE_ASM_FLAGS_RELEASE "")

# Set the linker flags
SET(CMAKE_C_LINK_FLAGS "") #For OSX, not empty by default
#SET(CMAKE_EXE_LINKER_FLAGS "-T${TOOLCHAIN_DIR}/STM32F103X8_FLASH.ld -nostartfiles -mthumb -static -mcpu=cortex-m3 -Wl,--gc-sections -Wl,-Map,${CMAKE_PROJECT_NAME}.map")
SET(CMAKE_EXE_LINKER_FLAGS "-T${TOOLCHAIN_DIR}/STM32F103X8_FLASH.ld  -mthumb -static -mcpu=cortex-m3 -Wl,--gc-sections -Wl,-Map,${CMAKE_PROJECT_NAME}.map")

# Make sure CMAKE doesn't try to create shared libraries. Otherwise our linker gets a -rdynamic flag
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_ASM_FLAGS "")
SET(SHARED_LIBS OFF)
SET(STATIC_LIBS ON)


# Add a custom target to generate the .hex and .bin files using objcopy, and output them to the Output directory
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin)


ADD_CUSTOM_TARGET(reset DEPENDS COMMAND JLinkExe -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/resetcommands.txt)


# Save the current directory so we can use it in the function later on
SET(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_DIR})
