#include <LiquidCrystal.h>

const int buttonPin = 7; // digital pin for push button
const int switchPin = 6; // digital pin for the switch
const int lcdPin = 8; // this is where the LCD power goes
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // follow link in google doc for wiring

int buttonState = 0; // state of the button
float probetemp = 0; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F? yes = true, no = false
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(lcdPin, OUTPUT);
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

void powerLCD(boolean power){ // todo test this
  if (power){
    digitalWrite(lcdPin, HIGH);
  }
  else{
    digitalWrite(lcdPin, LOW);
  }
}
