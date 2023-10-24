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
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    static const int start_duty_cycle = 30, end_duty_cycle = 255, turn_delay = 2000;
    static const double start_angle = 0, end_angle = 180, delta_angle = 5, threshold_rate = 0.1;  // 10%(of 1024) per second. => 102.4 units/1s => 0.1units/ms.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    uint8_t servo_pin, panel_pin;
    int brightness, calibrated;
    double angle;
    unsigned long long int time, calibrate_time;
    void set_servo(double angle, int wait = 0);

    public:
    Solarpanel(void);
    Solarpanel(uint8_t servo_pin, uint8_t panel_pin);
    void begin(void);
    void calibrate(void);
    void micro_calibrate(void);
    void operate(void);
    double get_angle(void);
    double get_voltage(void);
};

#endif
