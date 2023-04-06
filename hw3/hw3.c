// include header files here
#include "nu32dip.h" 
#include <stdio.h>
#include <math.h>


int main() {
    char message[100];
    NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
    //NU32DIP_WriteUART1("Hello\r\n");
    while(1) {
        int i = 0;
        float c = 0;
        if (!NU32DIP_USER){
        for(i = 0; i<250; i++){
            c = sin(i*3.3/100);
            sprintf(message,"%f\r\n",c);
            NU32DIP_WriteUART1(message);
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000){}
        }
        while(_CP0_GET_COUNT()<24000000*5){}
        }
    }
}
