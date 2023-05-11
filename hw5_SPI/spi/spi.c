#include "nu32dip.h"
#include <math.h>

#define pi 3.14159

// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn off analog pins
    // ANSELB = 0;
    // Make an output pin for CS
    TRISBbits.TRISB7 = 0; // output
    LATBbits.LATB7 = 1; // high
    // Set SDO1
    RPB6Rbits.RPB6R = 0b0011; //B6
    // Set SDI1 not using
    SDI1Rbits.SDI1R = 0b0100; //B8

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 400; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}


unsigned short io (unsigned short x, int a_or_b){
  unsigned short out = 0;
  out = out | 0b111 << 12;
  out = out | (a_or_b) << 15;
  out = out | (x << 2);
  LATBbits.LATB7 = 0;
  spi_io(out >> 8);
  spi_io(out & 0xFF);
  LATBbits.LATB7 = 1;
}


int main(){
    initSPI();
    NU32DIP_Startup(); 

    // -1 ~ +1 >> 1023

    int i = 0, j = 0, t = 0, delay = 1; //s  //2400000;
    float sin_val, tri_val; 
    unsigned int sine;
    unsigned int trig;
    int len = 400;
    int sinwave[len];
    int triwave[len];

  for(i = 0; i < len; i++){
    // make sine wave 2Hz
    sin_val = 1023/2*sin(16*pi*i/len) + 1023/2;
    sinwave[i] = sin_val;
  }

  for(j = 0; j < len; j++)
  {
    // make trig wave 1Hz  
    if(j < len/200){
      tri_val = 1023 * j /(len/4);
      triwave[j] = tri_val; 
    }
    else{
      tri_val = 1023 * (len - j) /(len/4);
      triwave[j] = tri_val; 
    }
  }

  while(1){

    // unsigned char sin_out = 0;
    // unsigned char tri_out = 0;
    // unsigned char a = 0b0, b = 0b1;


    for(t = 0; t < len; t++){
      // sin_out = sinwave[k] | 0b111;
      // sin_out = sin
      io(sinwave[t],0);
      io(triwave[t],1);
      _CP0_SET_COUNT(0);
      while(_CP0_GET_COUNT() < 48000000 / 2 / 100){}
    }
  }
}
    // // sin output
    // sin_out = 0b111 << 12;
    // sin_out = sin_out | (a << 15);

    // // tri output
    // tri_out = 0b111 << 12;
    // tri_out | (b << 15);

    // // send sine
    
    // LATbits.LATB7 = 0;
    // spi_io(sin_out >> 8);
    // spi_io(sin_out);
    // LATbits.LATB7 = 1; 

    // // send trig
    // CS = 0;
    // spi_io(tri_out >> 8);
    // spi_io(tri_out);
    // CS = 1; 
    // delay
//     _CP0_SET_COUNT(0);
//     while(_CP0_GET_COUNT() < 2400000){} // 1 sec
//     }

//     if (t < len - 1){
//       t++;
//     }
//     else{
//       t = 0;
//     }
// }


