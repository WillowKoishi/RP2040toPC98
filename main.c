#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart1

#define UART_TX_PIN 4
void send98(int msg);
int main() {
    gpio_init(2);
    gpio_set_dir(2, GPIO_IN);
    gpio_init(3);
    gpio_set_dir(3, GPIO_IN);
    uart_init(UART_ID, 19200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);//设置TX
    uart_set_format(UART_ID,8,2,UART_PARITY_ODD);
    sleep_ms(1000);
    send98(0x14);
    sleep_ms(100);
    send98(0x14 | 0x80);
    
}
//GPIO: RTY 2  RDY 3  RXD 4
void send98(int msg){
    while(gpio_get(2));
    char data = msg;
    uart_putc_raw(UART_ID,data);
    while(!gpio_get(2));
    sleep_us(20);
    if(!gpio_get(3)){
        send98(msg);
    }
}