/*
    Solarpanel.h - Library to controll a solar panel, attached to a servo.
    Created by Mohammed Khalid, October 23, 2023.
    Released into private domain.
*/
#ifndef Solarpanel_h
#define Solarpanel_h
#include "Arduino.h"

class Solarpanel
{
    private:
    static const int start_angle = 30, endg_angle = 255, number_of_angles = 225;
    static const float threshold_rate = 0.1;  // 10%(of 1024) per second. => 102.4 units/1s => 0.1units/ms.
    uint8_t servo_pin, panel_pin;
    int angle, brightness, calibrated;
    unsigned long long int time;

    public:
    Solarpanel(void);
    Solarpanel(uint8_t servo_pin, uint8_t panel_pin);
    void begin(void);
    void calibrate(void);
    void re_calibrate(void);
    void operate(void);
    float get_angle(void);
    float get_voltage(void);

};

#endif
