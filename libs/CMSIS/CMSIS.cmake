add_library(
    CMSIS
    STATIC
    ${CMAKE_CURRENT_LIST_DIR}/Src/system_stm32f1xx.c
    ${CMAKE_CURRENT_LIST_DIR}/Src/startup_stm32f103xb.s
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Include)
