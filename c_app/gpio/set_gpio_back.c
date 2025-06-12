#include <stdio.h>
#include <stdint.h>
 
//#define GPIO_MUX_2 (*(unsigned int *)0x16000498)
typedef struct
{
    volatile unsigned int GPIO_MUX_2;
}GPIO_MUX;



int main(int argc, char *argv[])
 {
    printf("gpio start\r\n");
    //GPIO_MUX_2 = 0xFF00FFFF;
    GPIO_MUX *gpio_mux;
    gpio_mux = ((GPIO_MUX *)0x16000498);
    gpio_mux->GPIO_MUX_2 = 0xFF00FFFF;
    printf("gpio end\r\n");
    return 0;
 }