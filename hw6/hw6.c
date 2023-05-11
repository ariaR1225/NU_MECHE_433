#include "nu32dip.h"
#include "i2c.h"

void olat(unsigned char msg){
    unsigned char set_msg;
    i2c_master_start();
    set_msg = 0b01000000;
    i2c_master_send(set_msg);
    set_msg = 0x0A;
    i2c_master_send(set_msg);
    i2c_master_send(msg);
    i2c_master_stop();
    
}

unsigned char gpio(){
    unsigned char set_msg, get_msg;
    i2c_master_start();
    set_msg = 0b01000000;
    i2c_master_send(set_msg);
    set_msg = 0x09;
    i2c_master_send(set_msg);
    i2c_master_restart();
    set_msg = 0b01000001;
    i2c_master_send(set_msg);
    get_msg = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return (get_msg & 0x01);
}

void blink(){
    unsigned char on_val = 0b1 << 7; 
    unsigned char off_val = 0b0 << 7;
    for(int i = 0; i < 5; i++){
        olat(on_val);
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 6000000){};
        olat(off_val);
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 6000000){};
    }
    
}

int main (){
    
    unsigned char send_buf = 0;
    unsigned char on_val = 0b1 << 7; 
    unsigned char off_val = 0b0 << 7;
    
    NU32DIP_Startup();
    i2c_master_setup();
    
    //set all pins to output
    i2c_master_start();
    send_buf = 0b01000000;
    i2c_master_send(send_buf);
    send_buf = 0x00;
    i2c_master_send(send_buf);
    send_buf = 0b01111111;
    i2c_master_send(send_buf);
    i2c_master_stop();
    
    //blink_led();
    
    while(1){
        if (gpio()) {
            olat(off_val);
        } else {
            blink();
        }
        
    }
     
}