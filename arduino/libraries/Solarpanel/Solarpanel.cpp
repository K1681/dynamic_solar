/*
    Solarpanel.h - Library to controll a solar panel, attached to a servo.
    Created by Mohammed Khalid, October 1, 2023.
    Released into private domain.
*/
#include "Arduino.h"
#include "Solarpanel.h"

Solarpanel::Solarpanel(void)
{
    servo_pin = NULL;
    panel_pin = NULL;
    angle = 0;
    brightness = 0;
    calibrated = 0;
    time = 0;
    calibrate_time = 0;
}

Solarpanel::Solarpanel(uint8_t servo_pin, uint8_t panel_pin)
{
    this->servo_pin = servo_pin;
    this->panel_pin = panel_pin;
    angle = 0;
    brightness = 0;
    calibrated = 0;
    time = 0;
    calibrate_time = 0;
}

void Solarpanel::begin(void)
{
    pinMode(servo_pin, OUTPUT);
    pinMode(panel_pin, INPUT);
}

void Solarpanel::calibrate(void)
{
    set_servo(this->start_angle, 1);
    int max_brightness = analogRead(this->panel_pin), curr_brightness;
    double max_angle = this->start_angle;

    for(double curr_angle = start_angle + delta_angle; curr_angle <= end_angle; curr_angle += delta_angle)
    {
        set_servo(curr_angle, 1);
        curr_brightness = analogRead(panel_pin);
        if(curr_brightness > max_brightness)
        {
            max_angle = curr_angle;
            max_brightness = curr_brightness;
        }
    }

    set_servo(max_angle, 1);
    this->brightness = analogRead(panel_pin);
    this->time = millis();
    this->calibrate_time = millis();
    this->calibrated = 1;
}

void Solarpanel::micro_calibrate(void)
{
    int max_brightness = analogRead(this->panel_pin), curr_brightness;
    double max_angle = this->angle;

    for(double curr_angle = this->angle + delta_angle; curr_angle <= end_angle; curr_angle += delta_angle)
    {
        set_servo(curr_angle, 1);
        curr_brightness = analogRead(panel_pin);
        if(curr_brightness > max_brightness)
        {
            max_brightness = curr_brightness;
            max_angle = curr_angle;
        }
        else if(curr_brightness < max_brightness)
        {
            curr_angle -= 2 * delta_angle;
            for(curr_angle; curr_angle >= start_angle; curr_angle -= delta_angle)
            {
                set_servo(curr_angle, 1);
                curr_brightness = analogRead(panel_pin);
                if(curr_brightness > max_brightness)
                {
                    max_brightness = curr_brightness;
                    max_angle = curr_angle;
                }
                else if(curr_brightness < max_brightness)
                {
                    set_servo(max_angle, 1);
                    break;
                }
            }
            break;
        }
    }

    this->brightness = analogRead(panel_pin);
    this->time = millis();
}

void Solarpanel::operate(void)
{
    if(!calibrated)
    {
        calibrate();
        return;
    }

    double delta_brightness = analogRead(panel_pin) - brightness;
    double delta_time = millis() - this->time;
    delta_brightness = (delta_brightness >= 0) ? delta_brightness : -delta_brightness;
    if((delta_brightness / delta_time) >= threshold_rate)
    {
        calibrate();
        return;
    }

    if(delta_time > micro_calibrate_delay)
    {
        micro_calibrate();
    }
}

double Solarpanel::get_angle(void)
{
    return angle;
}

double Solarpanel::get_voltage(void)
{
    return (double)(brightness * 5) / (double)1024;
}

void Solarpanel::set_servo(double target_angle, int wait = 0)
{
    if(target_angle < start_angle)
    {
        target_angle = start_angle;
    }
    else if(target_angle > end_angle)
    {
        target_angle = end_angle;
    }

    analogWrite(servo_pin, start_duty_cycle + (int)(((target_angle - start_angle)*(double)(end_duty_cycle - start_duty_cycle))/(end_angle - start_angle)));
    if(wait)
    {
        double delta_angel = target_angle - angle;
        delta_angel = (delta_angel >= 0) ? delta_angel : -delta_angel;
        delay((int)((delta_angle*(double)turn_delay)/(end_angle - start_angle)));
    }
    angle = target_angle;
}
