#define motor 9
#define panel A7

#define number_of_samples 225
int samples[number_of_samples];

void setup()
{
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(panel, INPUT);

  analogWrite(motor, 30);
  Serial.println("Motor ready");
  delay(5000);
  Serial.println("Motor operational");
}

void loop()
{
  analogWrite(motor, search());
  delay(5000);
}

int search(void)
{
  for(int i = 30; i < 255; i++)
  {
    analogWrite(motor, i);
    delay(50);
    samples[i] = analogRead(panel);
  }

  int max = 0;
  for(int i = 0; i < number_of_samples; i++)
  {
    if(samples[i] > samples[max])
    {
      max = i;
    }
  }

  return 30 + max;
}
