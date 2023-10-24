//////////////////////
#define servo 3
#define solar_panel A7
/////////////////////

#include <Solarpanel.h>
Solarpanel panel_1(servo, solar_panel);

void setup()
{
  Serial.begin(9600);
  panel_1.begin();
  Serial.println("Starting...");
  delay(3000);
}

void loop()
{
  panel_1.operate();
  Serial.print("Angle: "); Serial.print(panel_1.get_angle()); Serial.println("deg");
  Serial.print("Voltage: "); Serial.print(panel_1.get_voltage()); Serial.println("V");
  delay(500);
}