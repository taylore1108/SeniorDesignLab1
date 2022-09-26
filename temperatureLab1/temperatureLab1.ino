#include <LiquidCrystal.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DS18B20.h>

#include "SerialTransfer.h"
SerialTransfer myTransfer;

//hardware set up
const int buttonPin = 5; // digital pin for push button
const int switchPin = 4; // digital pin for the switch
//const int lcdPin = 6; // this is where the LCD power goes
const int rs = 7, en = 6, d4 = 3, d5 = 2, d6 = 1, d7 = 0; // follow link in google doc for wiring
const byte ip[] = { 192, 168, 0, 50 };
const byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };

//code set up
int buttonState = 0; // state of the button
float probetemp = 0; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F? yes = true, no = false
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//sms set up 
const char* resource = "https://maker.ifttt.com/trigger/TempTrigger/with/key/fOZGnZt3fxbSPMjgMfd_Xij7h7Z5Gg_B_gQv8RhImyV";
const char* server = "maker.ifttt.com";
String numberToText = "16308006164";
String messageToText = "This is a text of the arduino functions";
int TextTopBoundry;
int TextBottomBoundry;
const char* ssid     = "BlueFish";
const char* password = "RedTurtle";

void setup() {
  Serial.begin(115200);
  initWiFi();
  
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);
  //pinMode(lcdPin, OUTPUT);
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
  
    if (buttonState == HIGH) { // we only want to display locally when the button is pressed
      powerLCD(true);
      lcd.clear();
      lcd.setCursor(0, 0); //todo double check this prints in the right spot 
      lcd.print(changeReading(readProbe(), changetoF));
    
    }else{ 
      powerLCD(false);
      lcd.clear();
    
    }
  }
}

void initWiFi() {
  WiFi.setHostname("Group17");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void sendTextTest2(){
  HTTPClient http;   
  WiFiClient client;
  http.begin(client, resource);  
  http.addHeader("Content-Type", "application/json");       
  int httpResponseCode = http.POST("{\"value1\":\"Message body here\",\"value2\":\"16308006164\"}");

   
//  StaticJsonDocument<200> doc;
//  doc["Value1"] = messageToText;
//  doc["Value2"] = numberToText;
//   
//  String requestBody;
//  serializeJson(doc, requestBody);
   
 //int httpResponseCode = http.POST(requestBody);

  if(httpResponseCode>0){
    String response = http.getString();                       
     
    Serial.println(httpResponseCode);   
    Serial.println(response);
  }
  else {
    //Serial.printf("Error occurred while sending HTTP POST: %s\n", http.errorToString(statusCode));
  }
}

float readProbe(){
  DS18B20 ds(6); //sets the temp probe to pin 6
  float temp = ds.getTempC(); //gets the temperature of the probe
  Serial.println(String(temp,2));
  return temp;
}

String changeReading(float temp, boolean inFahrenheit){ //this is done

  if(temp == -0.06){
    return "Unplugged Sensor";
  }
  
  if(inFahrenheit){
    return String((temp * 9/5)+32) + "F";
  }
  return String(temp) + "C";
}

void powerLCD(boolean power){ // todo test this
  if (power){
    //digitalWrite(lcdPin, HIGH);
  }
  else{
    //digitalWrite(lcdPin, LOW);
  }
}
