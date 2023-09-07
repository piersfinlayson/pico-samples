cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(uarts
  uarts.c
)
add_definitions(-DPICO_DEFAULT_UART=1
  -DPICO_DEFAULT_UART_TX_PIN=4
  -DPICO_DEFAULT_UART_RX_PIN=5
)
pico_enable_stdio_usb(uarts 1)
pico_enable_stdio_uart(uarts 0)
pico_add_extra_outputs(uarts)
target_link_libraries(uarts pico_stdlib)