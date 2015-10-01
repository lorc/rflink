
add_custom_target(flash DEPENDS ${CMAKE_PROJECT_NAME}.bin COMMAND st-flash write --reset ${CMAKE_PROJECT_NAME}.bin 0x8000000)
add_custom_target(erase  COMMAND st-flash erase)
add_custom_target(link COMMAND st-util -1)
add_custom_target(gdb DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND arm-none-eabi-gdb --tui --se=${CMAKE_PROJECT_NAME}.elf --command=${CMAKE_CURRENT_LIST_DIR}/gdbcommands.txt)