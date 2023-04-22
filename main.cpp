#include "mbed.h"

InterruptIn joystickcenter(p14); //using joystick as a button to modify the low frequency period
PwmOut spkr(p26);
AnalogIn pot1(p19);
InterruptIn button(p9);
Timer debounce;

float low_freq = 1000.0; //value to be modified by user
float high_freq = 5000.0; //user changeable variable
float inc = 150; //another user variable

void mod() { //debounce function 
    if (debounce.read_ms() > 100) {
        low_freq += 500;
        debounce.reset();
    }
}

int main() {
    joystickcenter.rise(&mod); //attaching joystick address to mod function rising edge
    button.mode(PullUp); //pull up resistor
    button.rise(&mod); //attach function address to rising edge
    debounce.start(); //starting the debounce timer
 
    while (1) {
        for (float i = low_freq; i < high_freq; i += inc) { //new for loop includes user variables
            spkr.period(1.0 / i);
            spkr.write(0.5);
            wait(0.1);
        }
 
        if (low_freq == high_freq) { //resetting the low_freq variable when low_freq = high_freq so that it can repeat without becoming inaudible
            low_freq = 1000.0;
        }
 
        spkr.write(0.0);
        while (pot1.read() < 0.5) {}
    }
}
