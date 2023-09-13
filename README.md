# pico-samples

A set of sample programs for the Pico and Pico W

* uarts - Uses both UARTs on the Pico simultaneously
* dma - Tests dma transfers vs memcpy

## Building

This will build all of the samples:

```
cmake .
make -j 4
```

This creates for example uarts/uarts.uf2 which can then be installed as usual.

## Install

Plug in your Pico with BOOTSEL pressed.

Then, to flash uarts/uarts.uf2 to device sdx:

```
./flash-pico.sh sdx uarts/uarts.uf2
```

## Running

Connect to the USB serial terminal, like this:

```
minicom -o -D /dev/ttyACM0
```