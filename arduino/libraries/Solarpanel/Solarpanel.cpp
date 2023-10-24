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
    angle = start_angle;
    brightness = 0;
    calibrated = 0;
    time = 0;
}

Solarpanel::Solarpanel(uint8_t servo_pin, uint8_t panel_pin)
{
    this->servo_pin = servo_pin;
    this->panel_pin = panel_pin;
    angle = start_angle;
    brightness = 0;
    calibrated = 0;
    time = 0;
}

void Solarpanel::begin(void)
{
    pinMode(servo_pin, OUTPUT);
    pinMode(panel_pin, INPUT);
}

void Solarpanel::calibrate(void)
{
    analogWrite(servo_pin, start_angle);
    int max_brightness = analogRead(panel_pin), max_angle = start_angle, curr_brightness;

    for(int i = start_angle + 1; i <= endg_angle; i++)
    {
        analogWrite(servo_pin, i);
        delay(50);
        curr_brightness = analogRead(panel_pin);
        if(curr_brightness > max_brightness)
        {
            max_angle = i;
            max_brightness = curr_brightness;
        }
    }

    angle = max_angle;
    analogWrite(servo_pin, angle);
    brightness = analogRead(panel_pin);
    time = millis();
    calibrated = 1;
}

void Solarpanel::re_calibrate(void)
{
    int max_brightness = analogRead(panel_pin), max_angle = angle, curr_brightness;

    for(int i = angle + 1; i <= endg_angle; i++)
    {
        analogWrite(servo_pin, i);
        delay(50);
        curr_brightness = analogRead(panel_pin);
        if(curr_brightness > max_brightness)
        {
            max_brightness = curr_brightness;
            max_angle = i;
        }
        else if(curr_brightness < max_brightness)
        {
            i--;
            for(i; i >= start_angle; i--)
            {
                curr_brightness = analogRead(panel_pin);
                if(curr_brightness > max_brightness)
                {
                    max_brightness = curr_brightness;
                    max_angle = i;
                }
                else if(curr_brightness < max_brightness)
                {
                    break;
                }
            }
            break;
        }
    }

    angle = max_angle;
    analogWrite(servo_pin, angle);
    brightness = analogRead(panel_pin);
    time = millis();
}

void Solarpanel::operate(void)
{
    if(!calibrated)
    {
        calibrate();
        return;
    }
    float delta_brightness = analogRead(panel_pin) - brightness;
    delta_brightness = (delta_brightness > 0) ? delta_brightness : -delta_brightness;
    if((delta_brightness/ (float)(millis() - time)) >= threshold_rate)
    {
        calibrate();
        return;
    }
    re_calibrate();
}

float Solarpanel::get_angle(void)
{
    return (float)((angle - start_angle) * 180) / (float)number_of_angles;
}

float Solarpanel::get_voltage(void)
{
    return (float)(brightness * 5) / (float)1024;
}
