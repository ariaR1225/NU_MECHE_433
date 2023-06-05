#include "nu32dip.h" 
#include "i2c.h"
#include "ssd1306.h"
#include "mpu6050.h"
#include "font.h"

void drawLetter(char l, char pos_x, char pos_y);
void drawString(char *m, char pos_x, char pos_y);

int main(void) {
    NU32DIP_Startup(); 
    i2c_master_setup();
    ssd1306_setup();
    init_mpu6050();
  
    unsigned char d[14];
    float acc_z;
    char m[100];
    unsigned int f=24000000;

    while(1){
            _CP0_SET_COUNT(0);

            burst_read_mpu6050(d);

            acc_z = conv_zXL(d);
            sprintf(m, "z-acc: %f", acc_z);
            drawString(m,10,10);

            float fps = 24000000.0/f;
            sprintf(m, "fps: %f", fps);
            drawString(m,10,20);

            ssd1306_update();

            f = _CP0_GET_COUNT();
        }
}

void drawLetter(char l, char pos_x, char pos_y){
    for(int i = 0; i < 5; i++){
        char col = ASCII[l - 0x20][i];
        for(int j = 0; j < 8; j++){
            char c = (col >> j) & 0b1;
            ssd1306_drawPixel(pos_x + i, pos_y + j, c);
        }
    }
}

void drawString(char *m, char pos_x, char pos_y){
    int k = 0;
    while(m[k]!=0){
        drawLetter(m[k], pos_x + 10*k ,pos_y);
        k++;
    }
}
