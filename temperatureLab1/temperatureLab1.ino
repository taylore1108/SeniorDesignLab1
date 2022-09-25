#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>


#include "SerialTransfer.h"
SerialTransfer myTransfer;

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

//sms set up 
const char* resource = "https://maker.ifttt.com/trigger/YOUR EVENT NAME HERE/with/key/YOUR KEY HERE";
const char* server = "maker.ifttt.com";
String numberToText = "16308006164";
String messageToText = "This is a text of the arduino functions";
int TextTopBoundry;
int TextBottomBoundry;
const char* ssid     = "BlueFish";
const char* password = "RedTurtle";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
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

void sendTextTest1(){
  WiFiClient client;
  client.print(String("GET ") + resource + 
                    " HTTP/1.1\r\n" +
                    "Host: " + server + "\r\n" + 
                    "Connection: close\r\n\r\n");
                
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0))
  {
    delay(100);
  }
  while(client.available())
  {
    Serial.write(client.read());
  }
  client.stop();
}

void sendTextTest2(){
  HttpClient http;   
  http.begin(resource);  
  http.addHeader("Content-Type", "application/json");         
   
  StaticJsonDocument<200> doc;
  doc["Value1"] = messageToText;
  doc["Value2"] = numberToText;
   
  String requestBody;
  serializeJson(doc, requestBody);
   
  int httpResponseCode = http.POST(requestBody);

  if(httpResponseCode>0){
    String response = http.getString();                       
     
    Serial.println(httpResponseCode);   
    Serial.println(response);
  }
  else {
    Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
  }
}

float readProbe(){
  //todo read probe temperature in celcius here

  //if disconnected
  return -1;
}

String changeReading(float temp, boolean inFahrenheit){ //this is done

  if(temp == -1){
    return "Unplugged Sensor";
  }
  
  if(inFahrenheit){
    return String((temp * 9/5)+32) + "F";
  }
  return String(temp) + "C";
}

void powerLCD(boolean power){ // todo test this
  if (power){
    digitalWrite(lcdPin, HIGH);
  }
  else{
    digitalWrite(lcdPin, LOW);
  }
}
