int potentiometer_red = A0;
int potentiometer_green = A1;
int potentiometer_blue = A2;

int rgb_red = 3;
int rgb_green = 5;
int rgb_blue = 6;

void setup()
{
  pinMode(potentiometer_red, INPUT); //b10k Potentiometer Input (Red)
  pinMode(potentiometer_green, INPUT); //b10k Potentiometer Input (Green)
  pinMode(potentiometer_blue, INPUT); //b10k Potentiometer Input (Blue)
  
  pinMode(rgb_red, OUTPUT); //RGB Output (Red)
  pinMode(rgb_green, OUTPUT); //RGB Output (Green)
  pinMode(rgb_blue, OUTPUT); //RGB Output (Blue)
}

void loop()
{
  analogWrite(rgb_red, map(analogRead(potentiometer_red), 0, 1023, 0, 255));
  analogWrite(rgb_green, map(analogRead(potentiometer_green), 0, 1023, 0, 255));
  analogWrite(rgb_blue, map(analogRead(potentiometer_blue), 0, 1023, 0, 255));
  delay(10); // Delay a little bit to improve simulation performance
}
