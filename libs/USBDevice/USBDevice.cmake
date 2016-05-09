add_library(
    USBDevice
    STATIC
    ${CMAKE_CURRENT_LIST_DIR}/Src/usbd.c
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Inc)
