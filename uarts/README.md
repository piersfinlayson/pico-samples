# uarts

This program drives both UARTs on the Pico.
* It uses UART1 as the USB serial port, rather than using the default of UART0, and using pins 4 and 5.
* It uses UART0 as another UART.
* 10 digits are transmitted on UART0, 1 per second. 
* Any data in UART0's RX buffer is then read and logged to to stdout.

This is useful if you are using GP0/1 for other functions, but still want the USB serial port to function.

Both uarts.c and CMakeLists.txt contain necessary function for this to work.

The simplest way to test this is working is:
* Connect GP0 to GP1 (UART0 TX to UART0 RX, physical pins 1 and 2)
* Connect to the USB serial port
* You should then see 10 digits send over UART0 1 per second, followed by all 10 received at once (strictly, read at once)

## Build

```
cmake .
make -j 4
```

## Install

With your pico attached in BOOTSEL mode (as device sdx):

```
./flash-pico.sh sdx uarts
```

## Test

Connect GP0 and GP1 - physical pins 1 and 2.

Connect a serial terminal, for example:

```
minicom -o -D /dev/ttyACM0
```

You should see output like the following:

```
USB serial: initialized
UART0: initialized
UART0: Send 10 digits, one per second
UART0: Send char: 0
UART0: Send char: 1
UART0: Send char: 2
UART0: Send char: 3
UART0: Send char: 4
UART0: Send char: 5
UART0: Send char: 6
UART0: Send char: 7
UART0: Send char: 8
UART0: Send char: 9
UART0: Receive for 10 seconds
UART0: Received char: ÿ
UART0: Received char: 0
UART0: Received char: 1
UART0: Received char: 2
UART0: Received char: 3
UART0: Received char: 4
UART0: Received char: 5
UART0: Received char: 6
UART0: Received char: 7
UART0: Received char: 8
UART0: Received char: 9
```

I assume the extraneous 'ÿ' is an artifact of booting with GP0 and GP1 connected.  I would guess that the Pico briefly pulls GP0 (UART0 TX) high when booting, which is received on GP1 (UART0 RX) andinterpreted as all 1s (0xff - ÿ).

## UART pin mapping

If you have picotool installed (and if not, see [here](https://piers.rocks/2023/09/05/installing-pico-sdk-and-picotool.html)) you can also example the image, which shows which pins are mapped to which UARTs:

```
picotool info -a uarts.uf2
```

Gives output like:

```
File uarts.uf2:

Program Information
 name:          uarts
 version:       Build date and time: Sep  7 2023 10:33:48
 web site:      https://github.com/piersfinlayson/pico-samples
 description:   Program to test both UARTs
                USB serial is UART1
 features:      USB stdin / stdout
 binary start:  0x10000000
 binary end:    0x10008130

Fixed Pin Information
 0:  UART0 TX
 1:  UART0 RX
 4:  UART1 TX
 5:  UART1 RX

Build Information
 sdk version:       1.5.1
 pico_board:        pico
 boot2_name:        boot2_w25q080
 build date:        Sep  7 2023
 build attributes:  Release
```

