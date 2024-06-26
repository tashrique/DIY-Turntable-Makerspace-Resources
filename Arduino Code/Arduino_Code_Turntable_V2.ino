#include <LiquidCrystal.h>
#include <AccelStepper.h>

void(* resetFunc) (void) = 0;

/*
   LCD Pin Map
   Reset = 7;
   Enable = 8;
   D4 = 9;
   D5 = 10;
   D6 = 11;
   D7 = 12;

   Stepper PIN Map
   Step = 6
   Direction = 5
   (Type of driver: with 2 pins, STEP, DIR)

*/


AccelStepper stepper(1, 6, 5);

const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int green = 2;
int red = 3;
int button = 4;
int controls = A1;
int speeds = A0;


String currentStat = "Reset";
String prevStat = "Reset";
int stepsTaken = 0;
bool buttonPressed = false;
bool actionTaken = false;
int buttonClicked = 0;
int currentSpeed = 0;


void setup() {
  lcd.begin(16, 2);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(button, INPUT);

  resetControls();
}


void loop() {
  runProgram();
}

void runProgram() {
  currentSpeed = readSpeed();
  currentStat = getStatus();
  buttonClicked = buttonClick();

  digitalWrite(red, HIGH);

  lcd.setCursor(0, 0);
  lcd.print(": " + currentStat);

  lcd.setCursor(8, 0);
  lcd.print("-> " + String(currentSpeed) + "ms");


  if (buttonClicked == 1) {
    lcd.clear();

    //Reset
    if (currentStat == "Reset") {
      lcd.setCursor(0, 0);
      lcd.print("RESETTING...");
      stepsTaken = 0;
      prevStat = currentStat;
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      resetFunc();
    }

    //Resume
    else if (currentStat == "Start" && prevStat == "Pause") {
      lcd.setCursor(0, 1);
      lcd.print("RESUMED @" + String(currentSpeed));
      prevStat = currentStat;
      stepsTaken = commandStart(currentSpeed, stepsTaken);
    }


    //Start
    else if (currentStat == "Start") {
      lcd.setCursor(0, 1);
      lcd.print("STARTED @" + String(currentSpeed));
      prevStat = currentStat;
      stepsTaken = commandStart(currentSpeed, 0);
    }

    else if (currentStat == "Pause" && prevStat == "Pause") {
      lcd.setCursor(0, 1);
      lcd.print("Already Paused");
    }

    //Undefined
    else {
      lcd.setCursor(0, 1);
      lcd.print("Invalid Command");
    }
  }
}


/*--------------------------------------*/

int commandStart(int currentSpeed, int initial) {

  lcd.clear();
  int steps = 0;

  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);

  for (int i = initial; i <= 200; i++) {
    stepper.moveTo(i);
    stepper.runToPosition();
    lcd.setCursor(0, 1);
    lcd.print(i);

    lcd.setCursor(4, 1);
    lcd.print("/ 200 steps");
    steps = i;
    delay(currentSpeed);


    //Check if any other button is pressed while started
    String check = getStatus();
    lcd.setCursor(0, 0);
    lcd.print(check);

    int clicked = buttonClick();
    String clickedIndicator = clicked ? "*" : "";
    lcd.setCursor(6, 0);
    lcd.print(clickedIndicator);

    if (clicked) {
      if (check == "Reset") {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RESETTING...");
        delay(200);
        stepsTaken = 0;
        prevStat = "Reset";

        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);

        resetFunc();
      }

      else if (check == "Pause") {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Paused");
        delay(200);
        prevStat = "Pause";

        digitalWrite(green, HIGH);
        digitalWrite(red, HIGH);
        return steps;
      }
    }
  }

  return steps;
}

/*--------------------------------------*/

int buttonClick()
{
  int reading = digitalRead(button);
  return reading;
}


void resetControls() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turntable - Tash!");
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  delay(500);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  lcd.clear();
}


String getStatus() {
  int controlStatus = analogRead(controls);
  int controlRange = map(controlStatus, 0, 1023, 1, 4);
  String stat = "";

  if (controlRange == 1)
    stat = "Reset";

  else if (controlRange == 2)
    stat = "Pause";

  else if (controlRange == 3 || controlRange == 4)
    stat = "Start";

  else
    stat = "-----" ;
  delay(100);

  return stat;
}


int readSpeed() {
  int sensorVal = analogRead(speeds);
  int stepSpeed = map(sensorVal, 0, 1023, 250, 5000);
  return stepSpeed;
}
