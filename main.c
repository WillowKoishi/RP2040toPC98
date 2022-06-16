#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

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
#define G_C 13
#define G_D 14
#define G_ESC 15
#define G_UP 16
#define G_DOWN 17
#define G_LEFT 18
#define G_RIGHT 19
typedef struct P98KEYs
{
    int key_code;
    int gpio;
    long key_state;
    int key_state2;
} P98KEYs;

P98KEYs lP98KEYS[10];

void setS98Keys(int index,int keycode,int keygpio){
    gpio_init(keygpio);
    gpio_pull_up(keygpio);
    lP98KEYS[index].gpio=keygpio;
    lP98KEYS[index].key_code=keycode;
    lP98KEYS[index].key_state=0;
    lP98KEYS[index].key_state2=0;
}
void init_keys(){
    setS98Keys(0,K_SHIFT,G_SHIFT);
    setS98Keys(1,K_Z,G_Z);
    setS98Keys(2,K_X,G_X);
    setS98Keys(3,K_C,G_C);
    setS98Keys(4,K_D,G_D);
    setS98Keys(5,K_ESC,G_ESC);
    setS98Keys(6,K_UP,G_UP);
    setS98Keys(7,K_DOWN,G_DOWN);
    setS98Keys(8,K_LEFT,G_LEFT);
    setS98Keys(9,K_RIGHT,G_RIGHT);
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
long board_millis(){
    long mt =to_ms_since_boot( get_absolute_time());
    return mt;
}
void pushing(int i){
    //int key_code,long key_state,int key_state2,int mGPIO
    if(gpio_get(lP98KEYS[i].gpio)==0){
        if(lP98KEYS[i].key_state==0){
            send98(lP98KEYS[i].key_code);//make
            lP98KEYS[i].key_state = board_millis();
            gpio_put(25,1);
            return;
        }
        else if((gpio_get(lP98KEYS[i].gpio)==0)&&(lP98KEYS[i].key_state > 0 ) && ((board_millis()-lP98KEYS[i].key_state) >=500)){
            send98(lP98KEYS[i].key_code | 0x80);//break,and waiting 0.5s make
            lP98KEYS[i].key_state = - board_millis();
            gpio_put(25,0);
            return;
        }
        else if((gpio_get(lP98KEYS[i].gpio)==0)&&(lP98KEYS[i].key_state2 == 0 ) && (lP98KEYS[i].key_state < 0) && ((board_millis()+lP98KEYS[i].key_state) >= 30)){
            send98(lP98KEYS[i].key_code);     //still push,then make again
            lP98KEYS[i].key_state = - board_millis();
            lP98KEYS[i].key_state2 = 1;
            gpio_put(25,1);
            return;
        }
        else if((gpio_get(lP98KEYS[i].gpio)==0)&&(lP98KEYS[i].key_state2 == 1 ) && (lP98KEYS[i].key_state < 0) && ((board_millis()+lP98KEYS[i].key_state) >= 30)){
            send98(lP98KEYS[i].key_code | 0x80);    //still push,waiting 30ms,break
            lP98KEYS[i].key_state= - board_millis();
            lP98KEYS[i].key_state2= 0;
            gpio_put(25,0);
            return;
            
        }
        
    }
    if(gpio_get(lP98KEYS[i].gpio)==1){
        if((gpio_get(lP98KEYS[i].gpio)==1)&&lP98KEYS[i].key_state!=0){
            send98(lP98KEYS[i].key_code | 0x80);//break
            lP98KEYS[i].key_state = 0;
            lP98KEYS[i].key_state2 = 0;
            gpio_put(25,0);
            return;
        }
        else{
            return;
        }
    }
}

int main() {
    
    gpio_init(2);
    gpio_set_dir(2, GPIO_IN);
    gpio_init(3);
    gpio_set_dir(3, GPIO_IN);
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    uart_init(UART_ID, 19200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);//设置TX
    uart_set_format(UART_ID,8,2,UART_PARITY_ODD);
    sleep_ms(1000);
    gpio_init(26);
    gpio_set_dir(26, GPIO_OUT);
    gpio_put(26,1);
    init_keys();
    gpio_init(27);
    gpio_set_dir(27, GPIO_OUT);
    gpio_put(27,1);
    int i;
    while (1)
    {
        for(i=0;i<10;i++){
            pushing(i);
        }
    gpio_init(28);
    gpio_set_dir(28, GPIO_OUT);
    gpio_put(28,1);
    }
    
}
