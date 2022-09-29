#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <DS18B20.h>
 #include <AsyncTCP.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WiFiMulti.h>
#include <Wifi.h>
#include <HTTPClient.h>


#define USE_SERIAL Serial


 const char html[] PROGMEM = R"=="==(
    <!DOCTYPE HTML>
<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <style>
    body {
      min-width: 310px;
      max-width: 800px;
      height: 400px;
      margin: 0 auto;
    }

    h2 {
      font-family: Arial;
      font-size: 2.5rem;
      text-align: center;
    }

    h3 {
      text-Align: center;
      font-family: Arial;
    }

    div {
      align-items: center;
    }
 
  </style>
</head>

<body>

  <h3>Four Engineers walk into a bar</h3>
  <div id="chart-temperature" class="container"></div>
    <div>
      <div id="buttonSection" class="container" style="text-align: center;">
        <button>
          Turn on LCD
        </button>
        <button id="degreeButton">Change to Fahrenheit</button>
      </div>
      <form>
        <div style="text-align: center; margin:5px;">
          <label for "phoneInput">Enter your number: </label>
          <input id="phoneInput" type="tel" required name="phoneInput" placeholder="###-###-####"
            pattern="[0-9]{3}-[0-9]{3}-[0-9]{4}" />
        </div>
        <div style="text-align: center;">
          <button id="phoneSubmit" type="submit">Submit</button>
          <Label id="phoneDisplay">Num: N\A</label>
        </div>
      </form>
    </div>
</body>
<script>
  var isCelsius = true;
  var graphY_Max = 50;
  var graphY_Min = 10;
  var phoneNumber = "N\\A";
  const degreeBtn = document.getElementById('degreeButton');
  const submitElem = document.getElementById('phoneSubmit');
  // const numDisplay = document.getElementById('phoneDisplay');
  degreeBtn.addEventListener("click", () => {
    isCelsius = !isCelsius;
    if (isCelsius) {
      degreeBtn.textContent = 'Change to Fahrenheit';
      chartT.yAxis[0].setTitle({ text: "Temperature (Celsius)" });
      chartT.yAxis[0].setExtremes(10, 50);
    } else {
      degreeBtn.textContent = 'Change to Celsius';
      chartT.yAxis[0].setTitle({ text: "Temperature (Fahrenheit)" });
      chartT.yAxis[0].setExtremes(10, 122);
    }
  });
  var chartT = new Highcharts.Chart({
    chart: {
      renderTo: 'chart-temperature',
      events: {
        load: function () {
          this.clickedOnce = false;
        },
        click: function () {
          const chart = this;
          if (chart.clickedOnce) {
            chart.zoomOut();
            chart.clickedOnce = false;
          }
        }
      },
    },

    title: { text: 'ECE:4880 Temperature Lab' },

    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: {
        animation: false,
        dataLabels: { enabled: true }
      },
      series: {
        color: '#059e8a',
        events: {
          click: function (e) {
            const chart = this.chart,
              yAxis = chart.yAxis[0],
              xAxis = chart.xAxis[0];
            let x,
              y,
              rangeX,
              rangeY;
            if (!chart.clickedOnce) {
              x = xAxis.toValue(e.chartX);
              y = yAxis.toValue(e.chartY);
              rangeX = xAxis.max - xAxis.min;
              rangeY = yAxis.max - yAxis.min;
              xAxis.setExtremes(x - rangeX / 10, x + rangeX / 10, false);
              yAxis.setExtremes(y - rangeY / 10, y + rangeY / 10, false);
              chart.redraw();
              chart.clickedOnce = true;
            } else {
              chart.zoomOut();
              chart.clickedOnce = false;
            }
          }
        }
      },
    },
    xAxis: {
      title: { text: 'Time (Seconds)' },
      type: 'datetime',
      dateTimeLabelFormats: { second: '%S.%L' },
      max: 300,
      min: 0,
      reversed: true,

    },
    yAxis: {
      title: { text: 'Temperature (Celsius)' },
      max: graphY_Max,
      min: graphY_Min,

    },
    series: [{
      name: 'testData',
      data: [10, 20, 30, 40, 50]
    },],
    credits: { enabled: false }
  });

  function cToF(celsius) {
    var cTemp = celsius;
    var cToFahr = cTemp * 9 / 5 + 32;
    var message = cTemp + '\xB0C is ' + cToFahr + ' \xB0F.';
  }

  submitElem.addEventListener('click', function handleClick(event) {
    event.preventDefault();
    const numInput = document.getElementById('phoneInput');
    phoneNumber = numInput.value;
    numInput.value = '';
    const numDisplay = document.getElementById('phoneDisplay');
    numDisplay.innerHTML = 'Num:' + phoneNumber;
  });

</script>
</html>
)=="==";
char webpage[] PROGMEM = R"=="==( 
<html>
  <head>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/svg.js/2.6.6/svg.min.js"></script>
  </head>
  <body>
  </body>
</html>
)=="==";
WiFiMulti wifiMulti;
//hardware set up
int buttonPin = 35; // digital pin for push button
const int switchPin = 34; // digital pin for the switch
const int lcdPin = 13; // this is where the LCD power goes
const int rs = 14, en = 27, d4 = 26, d7 = 32, d6 = 33, d5 = 25; // follow link in google doc for wiring
const byte ip[] = { 192, 168, 0, 50 };
const byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };

//code set up
int buttonState = 0; // state of the button
float probetemp = 0; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F? yes = true, no = false
 LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//sms set up 
const char* resource = "https://maker.ifttt.com/trigger/TempTrigger/with/key/fOZGnZt3fxbSPMjgMfd_Xij7h7Z5Gg_B_gQv8RhImyV";
//const char* server = "maker.ifttt.com";
String numberToText = "16308006164";
String messageToText = "This is a text of the arduino functions";
int TextTopBoundry;
int TextBottomBoundry;
const char* ssid     = "Galaxy S2267C8";
const char* password = "nxzz5758";
AsyncWebServer server(80);

void setup() {
  USE_SERIAL.begin(115200);
  
  //sendTextTest2();
  pinMode(buttonPin, INPUT); // buttonPin = 14 / D5;
  pinMode(switchPin, INPUT);
  pinMode(lcdPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Hello World");
 
  initWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(switchPin);
  String s = String( buttonState);
  
  //USE_SERIAL.printf("[DEBUG] SWITCH %s...\n",s);
  if(digitalRead(switchPin) == HIGH){
    
    buttonState = digitalRead(buttonPin);
    String s = String(buttonState);
    // USE_SERIAL.printf("[DEBUG] BUTTON %s...\n",s);
    //todo update server here and 
    //read is "virtual button" is pressed
    //if("vitrualButton == HIGH) buttonState =HIGH;
  
    if (buttonState == HIGH) { // we only want to display locally when the button is pressed
      powerLCD(true);
     // lcd.clear();
      //lcd.setCursor(0, 0); //todo double check this prints in the right spot 
      //lcd.print(changeReading(readProbe(), changetoF));
      lcd.print("Hello World");
    }else{ 
      powerLCD(false);
     // lcd.clear();
      lcd.print("Hello World");
    }
  }
}

void initWiFi() {
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

//OLD text sms stuff
    //wifiMulti.addAP(ssid, password);
     //if((wifiMulti.run() == WL_CONNECTED)) {
     //}else{
     // Serial.println("error while connecting using wifiMulti");
     //}
    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println(WiFi.localIP());
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", html, processor);
    });

//    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
//    String inputMessage1;
//    String inputMessage2;
//    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
//    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
//      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
//      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
//      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
//    }
//    else {
//      inputMessage1 = "No message sent";
//      inputMessage2 = "No message sent";
//    }
//    Serial.print("GPIO: ");
//    Serial.print(inputMessage1);
//    Serial.print(" - Set to: ");
//    Serial.println(inputMessage2);
//    request->send(200, "text/plain", "OK");
//  });
  server.begin();
    
}

String processor(const String& var){
//  //Serial.println(var);
//  if(var == "BUTTONPLACEHOLDER"){
//    String buttons = "";
//    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
//    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
//    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
//    return buttons;
//  }
  return String();
}


void sendTextTest2(){
      HTTPClient http;   
      http.begin(resource);  
      http.addHeader("Content-Type", "application/json");       
      int httpResponseCode = http.POST("{\"value1\":\"Message body here\",\"value2\":\"16308006164\"}");
USE_SERIAL.println("Response Code: "+httpResponseCode);
   
//  StaticJsonDocument<200> doc;
//  doc["Value1"] = messageToText;
//  doc["Value2"] = numberToText;
//   
//  String requestBody;
//  serializeJson(doc, requestBody);
   
 //int httpResponseCode = http.POST(requestBody);

      if(httpResponseCode > 0) { String response = http.getString();                         
      http.end();
      delay(5000);
    }
}

float readProbe(){
  DS18B20 ds(15); //sets the temp probe to pin 6
  float temp = ds.getTempC(); //gets the temperature of the probe
  USE_SERIAL.println(String(temp,2));
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
    digitalWrite(lcdPin, HIGH);
    
  }
  else{
    digitalWrite(lcdPin, LOW);
  }
  return;
}
