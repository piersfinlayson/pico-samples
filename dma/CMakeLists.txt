add_executable(dma
  dma.c
)

target_link_libraries(dma pico_stdlib hardware_dma)

pico_add_extra_outputs(dma)

pico_enable_stdio_usb(dma 1)
