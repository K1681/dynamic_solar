//////////////////////
#define servo 9
#define solar_panel A7
/////////////////////

#include <Solarpanel.h>
Solarpanel panel_1(servo, solar_panel);

void setup()
{
  panel_1.begin();
}

void loop()
{
  panel_1.operate();
  delay(500);
}