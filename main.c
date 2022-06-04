/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

/// \tag::hello_uart[]

#define UART_ID uart1
#define BAUD_RATE 19200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 4
#define UART_RX_PIN 1

int main() {
    // Set up our UART with the required speed.
    uart_init(UART_ID, 19200);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);//设置TX

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    char data = 0x1c;
    // Send out a character without any conversions
    uart_putc_raw(UART_ID, data);
    
    // Send out a character but do CR/LF conversions
    uart_putc(UART_ID, 'B');

    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
}
void send98(int msg){
    
}
/// \end::hello_uart[]