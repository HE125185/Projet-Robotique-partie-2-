#include <LiquidCrystal.h> // includes the LiquidCrystal Library
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
const int trigPin = 9;
const int echoPin = 10;
const int RedLed = 11;
const int GreenLed = 12;

long duration;
int distanceCm, distanceInch;
void setup() {
  lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  pinMode(GreenLed, OUTPUT);
  pinMode(RedLed, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(GreenLed, LOW);
  digitalWrite(RedLed, LOW);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  distanceInch = duration * 0.0133 / 2;
  
  if(distanceCm <= 10){
    digitalWrite(GreenLed, LOW);
  }else{
    digitalWrite(RedLed, HIGH);
  }
 
  if(distanceCm >= 40){
    digitalWrite(RedLed, LOW);
  }else{
    digitalWrite(GreenLed, HIGH);
  }
  
  lcd.setCursor(0, 0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Distance: "); // Prints string "Distance" on the LCD
  lcd.print(distanceCm); // Prints the distance value from the sensor
  lcd.print("cm");
  delay(10);
  lcd.setCursor(0, 1);
  lcd.print("Distance: ");
  lcd.print(distanceInch);
  lcd.print("inch");
  delay(10);
}