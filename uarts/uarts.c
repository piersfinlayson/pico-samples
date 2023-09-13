#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <hardware/uart.h>

//
// This program drives both UARTs on the Pico
// * It uses UART1 as the USB serial port, rather than using the default of USB0, and using pins 4 and 5.
// * It uses UART0 as another UART.
// * 10 digits are transmitted on UART0, 1 per second. 
// * Any data in UART0's RX buffer is then read and logged to to stdout.
//
// This is useful if you are using GP0/1 for other functions, but still want the USB serial port to function.
//
// Both uarts.c and CMakeLists.txt contain necessary function for this to work.
//
// The simplest way to test this is working is:
// * Connect GP0 to GP1 (UART0 TX to UART0 RX, physical pins 1 and 2)
// * You should then see 10 digits send over UART0 1 per second, followed by all 10 received at once (strictly, read at once)
//

// Some tests to ensure that the USB serial UART is UART1, on pins 4/5, as we're going to use UART0 and 0/1 below
// If the program fails to compile due to these tests, check you have this in CMakeLists.txt
// add_definitions(-DPICO_DEFAULT_UART=1
//  -DPICO_DEFAULT_UART_TX_PIN=4
//  -DPICO_DEFAULT_UART_RX_PIN=5
// )
#if PICO_DEFAULT_UART != 1
    Dont compile
#endif
#if PICO_DEFAULT_UART_TX_PIN != 4
    Dont compile
#endif
#if PICO_DEFAULT_UART_RX_PIN != 5
    Dont compile
#endif

int main()
{
    uint __attribute__((unused)) speed;
    int ii;
    char c;

    // Set binary information for this image (see https://github.com/raspberrypi/picotool#readme)
    bi_decl(bi_program_description("Program to test both UARTs\nUSB serial is UART1"));
    bi_decl(bi_program_url("https://github.com/piersfinlayson/pico-samples"));
    bi_decl(bi_program_version_string("Build date and time: " __DATE__ " " __TIME__));
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_UART_RX_PIN, PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART));
    bi_decl(bi_2pins_with_func(0, 1, GPIO_FUNC_UART));

    // Initialize stdio - this sets up USB serial using UART1, and pins 4 and 5 due to the definitions in CMakeLists.txt.
    // We pause 5 seconds to give time to connect to the USB serial port from your host
    stdio_init_all();
    sleep_ms(5000);
    printf("USB serial: initialized\n");

    // Initialize UART0, which we'll use to send/transmit data
    speed = uart_init(uart0, 2400);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    uart_set_format(uart0, 8, 1, UART_PARITY_EVEN);
    printf("UART0: initialized\n");

    // Do the contents of this while loop forever
    while (1)    
    {
        // Send ASCII digits 48-57 (chararacters 0, 1, 2 ... 9) out of the UART0, one digit per second
        printf("UART0: Send 10 digits, one per second\n");
        for (c=48; c < 58; c++)
        {
            sleep_ms(1000);
            printf("UART0: Send char: %c\n", c);
            uart_putc(uart0, c);
        }

        // Now read whatever data we have already receivedor receive over the next 10 seconds
        printf("UART0: Receive for 10 seconds\n");
        for (ii=0; ii < 10; ii++)
        {
            sleep_ms(1000);
            while (uart_is_readable(uart0))
            {
                c=uart_getc(uart0);
                printf("UART0: Received char: %c\n", c);
            }
        }
    }
}

