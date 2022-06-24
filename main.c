#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
// #include "hardware/pio.h"
// #include "hardware/clocks.h"
// #include "ws2812.pio.h"

#define UART_ID uart1

#define UART_TX_PIN 4

#define K_SHIFT 0x70
#define K_Z 0x29
#define K_X 0x2A
#define K_C 0x2B
#define K_D 0x1F
#define K_ESC 0x00
#define K_UP 0x3A
#define K_DOWN 0x3D
#define K_LEFT 0x3B
#define K_RIGHT 0x3C

#define G_SHIFT 10
#define G_Z 11
#define G_X 12
#define G_C 14
#define G_D 13
#define G_ESC 19
#define G_UP 15
#define G_DOWN 18
#define G_LEFT 16
#define G_RIGHT 17

//uint32_t colors[10];
// void initColor(){
//     int ic=0;
//     for(ic=0;ic<0;ic++){
//         colors[ic]=WS2812B_24bit(255,0,0);
//     }
// }
typedef struct P98KEYs
{
    int key_code;
    int gpio;
    long key_state;
    int key_state2;
} P98KEYs;

P98KEYs lP98KEYS[10];
float R = 255;
float G = 255;
float B = 255;
int m = 0;
int X = 0;
int C = 0;

void setS98Keys(int index, int keycode, int keygpio)
{
    gpio_init(keygpio);
    gpio_pull_up(keygpio);
    lP98KEYS[index].gpio = keygpio;
    lP98KEYS[index].key_code = keycode;
    lP98KEYS[index].key_state = 0;
    lP98KEYS[index].key_state2 = 0;
}

void init_keys()
{
    setS98Keys(0, K_SHIFT, G_SHIFT);
    setS98Keys(1, K_Z, G_Z);
    setS98Keys(2, K_X, G_X);
    setS98Keys(3, K_C, G_C);
    setS98Keys(4, K_D, G_D);
    setS98Keys(5, K_ESC, G_ESC);
    setS98Keys(6, K_UP, G_UP);
    setS98Keys(7, K_DOWN, G_DOWN);
    setS98Keys(8, K_LEFT, G_LEFT);
    setS98Keys(9, K_RIGHT, G_RIGHT);
}
// GPIO: RTY 2  RDY 3  RXD 4
void send98(int msg)
{
    while (gpio_get(3))
        ;
    char data = msg;
    uart_putc_raw(UART_ID, data);
    while (!gpio_get(3))
        ;
    sleep_us(25);
    if (!gpio_get(2))
    {
        send98(msg);
    }
}
long board_millis()
{
    long mt = to_ms_since_boot(get_absolute_time());
    return mt;
}
void pushing(int i)
{
    // int key_code,long key_state,int key_state2,int mGPIO
    if (gpio_get(lP98KEYS[i].gpio) == 0)
    {
        sleep_us(200);
        if (lP98KEYS[i].key_state == 0)
        {
            send98(lP98KEYS[i].key_code); // make
            lP98KEYS[i].key_state = board_millis();
            gpio_put(25, 1);
            return;
        }
        else if ((lP98KEYS[i].key_code != K_SHIFT) && (gpio_get(lP98KEYS[i].gpio) == 0) && (lP98KEYS[i].key_state > 0) && ((board_millis() - lP98KEYS[i].key_state) >= 500))
        {
            // send98(lP98KEYS[i].key_code | 0x80);//break,and waiting 0.5s make
            lP98KEYS[i].key_state = -board_millis();
            gpio_put(25, 0);
            return;
        }
        else if ((lP98KEYS[i].key_code != K_SHIFT) && (gpio_get(lP98KEYS[i].gpio) == 0) && (lP98KEYS[i].key_state2 == 0) && (lP98KEYS[i].key_state < 0) && ((board_millis() + lP98KEYS[i].key_state) >= 30))
        {
            send98(lP98KEYS[i].key_code); // still push,then make again
            lP98KEYS[i].key_state = -board_millis();
            lP98KEYS[i].key_state2 = 1;
            gpio_put(25, 1);
            return;
        }
        else if ((lP98KEYS[i].key_code != K_SHIFT) && (gpio_get(lP98KEYS[i].gpio) == 0) && (lP98KEYS[i].key_state2 == 1) && (lP98KEYS[i].key_state < 0) && ((board_millis() + lP98KEYS[i].key_state) >= 30))
        {
            // send98(lP98KEYS[i].key_code | 0x80);    //still push,waiting 30ms,break
            lP98KEYS[i].key_state = -board_millis();
            lP98KEYS[i].key_state2 = 0;
            gpio_put(25, 0);
            return;
        }
    }
    if (gpio_get(lP98KEYS[i].gpio) == 1)
    {
        if ((gpio_get(lP98KEYS[i].gpio) == 1) && lP98KEYS[i].key_state != 0)
        {
            send98(lP98KEYS[i].key_code | 0x80); // break
            lP98KEYS[i].key_state = 0;
            lP98KEYS[i].key_state2 = 0;
            gpio_put(25, 0);
            send98(lP98KEYS[i].key_code | 0x80);
            return;
        }
        else
        {
            return;
        }
    }
}
void HSV2RGB(int h, int s, int v)
{
    if (h < 0)
    {
        h = h + 360;
    }
    if (h > 360)
    {
        h = h - 360;
    }
    h = h - 360;
    C = v * s;
    int i = (int)h;
    X = C * (1 - abs(h % 2 - 1));
    switch (i)
    {
    case 0:
        R = C;
        G = X;
        B = 0;
        break;
    case 1:
        R = X;
        G = C;
        B = 0;
        break;
    case 2:
        R = 0;
        G = C;
        B = X;
        break;
    case 3:
        R = 0;
        G = X;
        B = C;
        break;
    case 4:
        R = X;
        G = 0;
        B = C;
        break;
    case 5:
        R = C;
        G = 0;
        B = X;
        break;
    default:
        R = C;
        G = X;
        B = 0;
        break;
    }
    R = ((R + m) * 255);
    G = ((G + m) * 255);
    B = ((B + m) * 255);
}
void sendNP_ll(int gpio)
{ //输出0，高电平时间小于低电平
     gpio_put(gpio, 1);
     sleep_us(0.6);
     gpio_put(gpio, 0);
     sleep_us(1.2);
    // int on = 21;
    // int off = 17;
    // gpio_put(gpio, 1);
    // while(on--) 
    // {
    //     asm("mov r0,r0");
    // }
    // gpio_put(gpio, 0);
    // while(off--) 
    // {
    //     asm("mov r0,r0");
    //     asm("mov r0,r0");
    //     asm("mov r0,r0");
    // }
}
void sendNP_hl(int gpio)
{ //输出1，高电平时间大于低电平
    gpio_put(gpio, 1);
    sleep_us(1.2);
    gpio_put(gpio, 0);
    sleep_us(0.6);
    // int on = 17;
    // int off = 21;
    // gpio_put(gpio, 1);
    // while(on--) 
    // {
    //     asm("mov r0,r0");
    //     asm("mov r0,r0");
    //     asm("mov r0,r0");
    // }
    // gpio_put(gpio, 0);
    // while(off--) 
    // {
    //     asm("mov r0,r0");
    // }
}
void sendNPData(int mR, int mG, int mB, int gpio)
{

    int ir = 0;
    for (ir = 0; ir < 8; ir++)
    { // G
        if (mG >> (7 - ir) & 1)
        {
            sendNP_hl(gpio);
        }
        else
        {
            sendNP_ll(gpio);
        }
    }
    for (ir = 0; ir < 8; ir++)
    { // R
        if (mR >> (7 - ir) & 1)
        {
            sendNP_hl(gpio);
        }
        else
        {
            sendNP_ll(gpio);
        }
    }
    for (ir = 0; ir < 8; ir++)
    { // B
        if (mB >> (7 - ir) & 1)
        {
            sendNP_hl(gpio);
        }
        else
        {
            sendNP_ll(gpio);
        }
    }
}

int main()
{
    stdio_init_all();
    set_sys_clock_khz(125000,true);
    gpio_init(2);
    gpio_set_dir(2, GPIO_IN);
    gpio_init(3);
    gpio_set_dir(3, GPIO_IN);
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    uart_init(UART_ID, 19200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); //设置TX
    uart_set_format(UART_ID, 8, 1, UART_PARITY_ODD);
    sleep_ms(1000);
    gpio_init(6);
    gpio_set_dir(6, GPIO_OUT);

    init_keys();

    int i,hh;
    hh=0;
    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            pushing(i);
        }
        HSV2RGB(hh, 1, 1);
        hh = hh + 0.01;
        gpio_put(6,0);
        sleep_us(100);
        for (i = 0; i < 10; i++)
        {
            //colors[i]=WS2812B_24bit((int)R,(int)G,(int)B);
            sendNPData(0,0,0,6);
        }
        // WS2812B_reset();
        // WS2812B_sendArray(colors,10);
    }
}
