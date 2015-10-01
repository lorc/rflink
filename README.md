STM32-Template
==============

A Build template for projects using the stm32f103c8 IC,
GNU toolchain, and the STM32Cube HAL drivers.

Project uses newest HAL drivers instead of old Peripheral Drivers Library.
CMAKE used as build system. It simplifies adding new files and also
make project more modular. Just checkout Libs/ folder!

Template can be easily adapted to other STM32F10x series chips. You will need
only linker script for your chip. This script can be taken from STM32Cube
package.

By default, it compiles simple Blinky projects that blinks LED connected to
PC13 pin.

To test:

   (1) install the toolchain
   (2) execute ./configure script. It will create directory 'build'
   (3) type 'make Demo.bin' to get .bin file ready to flashing


Feel free to adapt it to your projects!
