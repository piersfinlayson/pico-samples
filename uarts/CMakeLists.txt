add_executable(uarts
  uarts.c
)
add_definitions(-DPICO_DEFAULT_UART=1
  -DPICO_DEFAULT_UART_TX_PIN=4
  -DPICO_DEFAULT_UART_RX_PIN=5
)

target_link_libraries(uarts pico_stdlib)

pico_add_extra_outputs(uarts)

pico_enable_stdio_usb(uarts 1)
pico_enable_stdio_uart(uarts 0)
