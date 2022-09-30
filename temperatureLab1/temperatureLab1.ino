#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <DS18B20.h>
 #include <AsyncTCP.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
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
        <button id = "turnOnLcdButton">
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
          <button id="phoneSubmit" type="submit">Submit #</button>
          <Label id="phoneDisplay">Num: N\A</label>
        </div>
      </form>
        <div style="text-align: center; margin:5px;" >
          <label for "minInput">Enter the min temp: </label>
          <input id = "minInput" type = "number" required name = "minInput" placeholder = "min"/>
        </div>
        <div style="text-align: center;">
          <button id = "minSubmit" type = "submit">Submit Min</button>
          <Label id = "minDisplay">Min: 23</Label>
        </div>
      </form>
    </div>
</body>
<script>
  var isCelsius = true;
  var graphY_Max = 50;
  var graphY_Min = 10;
  var phoneNumber = "N\\A";
  var safeMax = 25;
  var safeMin = 23;
  const degreeBtn = document.getElementById('degreeButton');
  const submitElem = document.getElementById('phoneSubmit');
  const lcdButton = document.getElementById('turnOnLcdButton');
  const maxBtn = document.getElementById('maxSubmit');
  const minBtn = document.getElementById('minSubmit');
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
      //title: { text: 'Time (Seconds)' },
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' },
        //max: 300,
        //min: 0,
        //reversed: true,

    },
    yAxis: {
      title: { text: 'Temperature (Celsius)' },
      max: graphY_Max,
      min: graphY_Min,

    },
    credits: { enabled: false }
  });
   setInterval(function ( ) {
     var xhttp = new XMLHttpRequest();
     xhttp.onreadystatechange = function() {
       if (this.readyState == 4 && this.status == 200) {
         var x = (new Date()).getTime(),
             y = parseFloat(this.responseText);
         //console.log(this.responseText);
         if(chartT.series[0].data.length > 40) {
           chartT.series[0].addPoint([x, y], true, true, true);
         } else {
           chartT.series[0].addPoint([x, y], true, false, true);
         }
       }
     };
     xhttp.open("GET", "/temperature", true);
     xhttp.send();
   }, 900 );
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

  lcdButton.addEventListener('mousedown', function handleClick(event){
    event.preventDefault();
    document.getElementById("turnOnLcdButton").innerHTML = "LCD on";
    
  });
  lcdButton.addEventListener('mouseup', function handleClick(event){
    event.preventDefault();
    document.getElementById("turnOnLcdButton").innerHTML = "Turn on LCD";
  });
  maxBtn.addEventListener('click', function handleClick(event) {
    event.preventDefault();
    const maxInput = document.getElementById('maxInput');
    maxNumber = maxInput.value;
    maxInput.value = '';
    const maxDisplay = document.getElementById('maxDisplay');
    maxDisplay.innerHTML = 'Max:' + maxNumber;
  });
//  setInterval(function () {
//    var yhttp = new XMLHttpRequest();
//    yhttp.onreadystatechange = function(){
//      if(this.readyState == 4 && this.status == 200){
//        var newMin =document.getElementById('minDisplay').value;
//        document.getElementById('minDisplay').innerHTML = "Value Pulled";
//        String y = newMin.toString(10);
//      }
//    };
//    yhttp.open("POST", "/min", true);
//    yhttp.setRequestHeader("Content-type", "text/plain");
//    yhttp.send(y);
//  },500);
  minBtn.addEventListener('click', function handleClick(event) {
    event.preventDefault();
    const minInput = document.getElementById('minInput');
    minNumber = minInput.value;
    minInput.value = '';
    const minDisplay = document.getElementById('minDisplay');
    minDisplay.innerHTML = 'Min:' + minNumber;
    var newHttp = new XMLHttpRequest();
    newHttp.open("GET", "/min?newMin="+minNumber, true);
    newHttp.send();
  });

</script>

</html>
)=="==";

//hardware set up
int buttonPin = 35; // digital pin for push button
const int switchPin = 34; // digital pin for the switch
const int lcdPin = 19; // this is where the LCD power goes
const int rs = 14, en = 27, d4 = 26, d7 = 32, d6 = 33, d5 = 25; // follow link in google doc for wiring
//const byte ip[] = { 192, 168, 0, 50 };


int switchVal =-1;
int buttonVal=-1;
String tempVal = "0";
const byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };

//code set up
int buttonState = 0; // state of the button
String probetemp = "0"; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F? yes = true, no = false
 LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//sms set up 
const char* resource = "https://maker.ifttt.com/trigger/TempTrigger/with/key/fOZGnZt3fxbSPMjgMfd_Xij7h7Z5Gg_B_gQv8RhImyV";
//const char* server = "maker.ifttt.com";
String numberToText = "16308006164";
String messageToText = "This is a text of the arduino functions";
float TextTopBoundryInC = 26.0; // keep in C
float TextBottomBoundryInC = 22.0; // keep in C
const char* ssid  = "Galaxy S2267C8";
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
  lcd.print("Hello Group 17");
  virtualPowerLCD(true);
  initWiFi();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(readProbe()).c_str());
    });

  
  server.on("/min", HTTP_GET, [](AsyncWebServerRequest *request){
    String minInput = request->getParam("newMin")->value();
    minFunct(minInput.toInt());
    request->send(200, "text/plain", "OK");
  });
  
    
    server.begin();
  //runClient();
}

void loop() {
  float t = readProbe();
  probetemp = changeReading(t, changetoF);
  if(tempVal != probetemp){
    tempVal = probetemp;
    lcd.clear();
    lcd.setCursor(0, 0);
    String ss = String(t)+"C";
    lcd.print(ss);
  }
  buttonState = digitalRead(switchPin);
  if(switchVal!=buttonState){
    switchVal = buttonState;
    String s = String( switchVal);
    USE_SERIAL.printf("[DEBUG] SWITCH %s...\n",s);
  }
  if(digitalRead(switchPin) == HIGH){
    buttonState = digitalRead(buttonPin);
    if(buttonVal!=buttonState){
      buttonVal = buttonState;
      String s = String(buttonVal);
      USE_SERIAL.printf("[DEBUG] BUTTON %s...\n",s);
      
    }
  }
  
  delay(500);
}


char httpResponse[90] = {0};
char responseIndex = 0;
void runClient(){
//  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
//    String inputMessage;
//    String inputParam;
//    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
//    if (request->hasParam("max1")) {
//      inputMessage = request->getParam("max1")->value();
//      inputParam = "max1";
//    }else{
//      Serial.println("html failed");
//    }
//    Serial.println("Max: "+inputMessage);
    
    
  //});
//      int virtualButtonState = 1;
//      if (virtualButtonState == 1) { 
//        virtualPowerLCD(true);
//      }else{ 
//        virtualPowerLCD(false);
//      }
  //server.onNotFound(notFound);
  //server.begin();
}

void minFunct(int newMin){
  TextBottomBoundryInC=newMin;
  Serial.print("Min: ");
  Serial.println(TextBottomBoundryInC);
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
    //WiFi.config(ip);
    WiFi.begin(ssid,password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println(WiFi.localIP());
//    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//      request->send_P(200, "text/html", html, processor);
//    });

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
  //server.begin();
    
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
  //wifiMulti.addAP(ssid, password);
  //if((wifiMulti.run() == WL_CONNECTED)) {
      HTTPClient http;   
      http.begin(resource);  
      http.addHeader("Content-Type", "application/json");    
      String packet = "{\"value1\":\""+messageToText+"\",\"value2\":\""+numberToText+"\"}";
      int httpResponseCode = http.POST(packet);
      //int httpResponseCode = http.POST("{\"value1\":\"Message body here\",\"value2\":\"16308006164\"}");
      USE_SERIAL.println("Response Code: "+httpResponseCode);
   
//  StaticJsonDocument<200> doc;
//  doc["Value1"] = messageToText;
//  doc["Value2"] = numberToText;
//   
//  String requestBody;
//  serializeJson(doc, requestBody);
 // int httpResponseCode = http.POST(requestBody);

      //if(httpResponseCode > 0) { 
        String response = http.getString();         
        USE_SERIAL.println("Response Code2: "+response);                
        http.end();
      //}

//    }
//    else{
//        Serial.println("error while connecting using wifiMulti");
//    }
}

float readProbe(){
  DS18B20 ds(4); //sets the temp probe to pin 4
  float temp = ds.getTempC(); //gets the temperature of the probe
  USE_SERIAL.println(String(temp));
  return temp;
}

String changeReading(float temp, boolean inFahrenheit){ //add savinf data here

  if(temp < 1){
    return "Unplugged Sensor";
  }

//  if(temp < TextBottomBoundryInC){
//    messageToText = "Temperature Probe detects temperature " +String(temp)+ "C, which is below lower boundry "+ String(TextBottomBoundryInC) +" C"; 
//    sendTextTest2();
//  }
//  else if( temp > TextTopBoundryInC){
//    messageToText = "Temperature Probe detects temperature " +String(temp)+ "C, which is above upper boundry "+ String(TextBottomBoundryInC) +" C"; 
//    sendTextTest2();
//  }
  
  if(inFahrenheit){
    return String((temp * 9/5)+32) + "F";
  }
  return String(temp) + "C";
}

void virtualPowerLCD(boolean power){ // todo test this
  if (power){
    digitalWrite(lcdPin, HIGH);
    
  }
  else{
    digitalWrite(lcdPin, LOW);
  }
  return;
}
