#include <LiquidCrystal.h>

const int buttonPin = 2; // digital pin two is wired to push button
const int switchPin = 4;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int buttonState = 0; // state of the button
float probetemp = 0; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F?

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(switchPin);
  if(digitalRead(switchPin) == HIGH){
    probetemp = changeReading(readProbe(), changetoF);
    
    buttonState = digitalRead(buttonPin);
    //todo update server here and 
    //read is "virtual button" is pressed
    //if("vitrualButton == HIGH) buttonState =HIGH;
  
    if (buttonState == HIGH) { // we only want to display locally when the button is pressed
      powerLCD(true);
      lcd.setCursor(0, 1); //todo double check this prints in the right spot 
      lcd.print(probetemp);
      if (changetoF) lcd.print("F");
      else lcd.print("C");
    
    }else{ 
      powerLCD(false);
      lcd.clear();
    
    }
  }
}

float readProbe(){
  //todo read probe temperature in celcius here
  return -1;
}

float changeReading(float temp, boolean inFahrenheit){ //this is done
  if(inFahrenheit){
    return (temp * 9/5)+32;
  }
  return temp;
}

void powerLCD(boolean power){
  if (powerOn){
    //todo turn the LCD on
  }
  else{
    //todo turn the LCD off
  }
}
