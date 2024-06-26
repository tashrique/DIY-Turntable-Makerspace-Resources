#include <LiquidCrystal.h>
#include <AccelStepper.h>

#define motorInterfaceType 1

//LCD instance
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //rs, enable, d4, d5, d6, d7

//Stepper Pins
const int dirPin = 5;
const int stepPin = 6;

// Stepper instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {

  //Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Turntable Project");
  lcd.setCursor(0, 1);
  lcd.print("by Tashrique");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  //Initialize Stepper
  myStepper.setMaxSpeed(1000);
  myStepper.setAcceleration(50);
  myStepper.setSpeed(200);
  myStepper.moveTo(200);





  //Finished initializing
  delay(1000);
  lcd.clear();
  lcd.print("Ready to rock!");
  delay(2000);
  lcd.clear();
}

void loop() {

}


/*
 * #include <CheapStepper.h>

int buzzer = 2;
int inPin = 7; // przycisk
int val = 0; // zmienna?
int stanButton = 0; // stan

CheapStepper stepper (8,9,10,11); 

void setup() 
{
pinMode(inPin, INPUT_PULLUP);
pinMode(buzzer, OUTPUT);
stepper.setRpm(12);
Serial.begin(9600);
startSound();
}

void loop() 
{
option();
if(stanButton == 0)
{
  Serial.println("1");
  stepper.moveDegreesCW(0);
  delay(100);
}
else if(stanButton == 1)
{
  Serial.println("2");
  stepper.moveDegreesCCW(10);
}
else if(stanButton == 2)
{
  Serial.println("3");
  stepper.moveDegreesCW(10);
}
}

//FUNCTIONS
void option()
{
  val = digitalRead(inPin);   // read the input pin
  if(val == LOW)
{
  selectSound();
  stanButton=(stanButton + 1) % 3; // dodaje
}
}

void startSound()
{
  digitalWrite(buzzer, 1);
  delay(300);
  digitalWrite(buzzer, 0);
}

void selectSound()
{
  digitalWrite(buzzer, 1);
  delay(40);
  digitalWrite(buzzer, 0);
  delay(40);
}

 * /
 */
