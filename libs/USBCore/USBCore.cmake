add_library(
    USBCore
    STATIC
    ${CMAKE_CURRENT_LIST_DIR}/Src/usbd_core.c
    ${CMAKE_CURRENT_LIST_DIR}/Src/usbd_ctlreq.c
    ${CMAKE_CURRENT_LIST_DIR}/Src/usbd_ioreq.c
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Inc)
