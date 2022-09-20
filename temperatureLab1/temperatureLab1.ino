#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFi.h>

//hardware set up
const int buttonPin = 7; // digital pin for push button
const int switchPin = 6; // digital pin for the switch
const int lcdPin = 8; // this is where the LCD power goes
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // follow link in google doc for wiring
const byte ip[] = { 192, 168, 0, 50 };
const byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };

//code set up
int buttonState = 0; // state of the button
float probetemp = 0; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F? yes = true, no = false
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  //todo add 
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(lcdPin, OUTPUT);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(switchPin);
  if(digitalRead(switchPin) == HIGH){
    
    buttonState = digitalRead(buttonPin);
    //todo update server here and 
    //read is "virtual button" is pressed
    //if("vitrualButton == HIGH) buttonState =HIGH;
  
      powerLCD(true);
      lcd.clear();
      lcd.setCursor(0, 0); //todo double check this prints in the right spot 
      lcd.print(changeReading(readProbe(), changetoF));
      delay(1000);
    
    
    // }
    }
  }
}

float readProbe(){
  //todo read probe temperature in celcius here
    DS18B20 ds(6); //sets the temp probe to pin 6
    float temp = ds.getTempC(); //gets the temperature of the probe
      Serial.println(String(temp,2));
      return temp;

  //if disconnected
  return -1;
}

String changeReading(float temp, boolean inFahrenheit){ //this is done

  if(temp == -0.06){
  if(temp == -1){
    return "Unplugged Sensor";
  }
  
  if(inFahrenheit){
  }
}

void powerLCD(boolean power){ // todo test this
  if (power){
    digitalWrite(lcdPin, HIGH);
  }
  else{
    digitalWrite(lcdPin, LOW);
  }
}
