
const int buttonPin = 2; // digital pin two is wired to push button
const int switchPin = 4;

int buttonState = 0; // state of the button
float probetemp = 0; // this is the probe temp value we read
boolean changetoF = false; // do we want to convert probe temp in C to temp in F?

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(switchPin);
  if(digitalRead(switch_pin) == HIGH){
    probetemp = changeReading(readProbe(), changetoF);
    
    buttonState = digitalRead(buttonPin);
    //todo update server here and 
    //read is "virtual button" is pressed
    //if("vitrualButton == HIGH) buttonState =HIGH;
  
    if (buttonState == HIGH) { 
      // we only want to display locally when the button is pressed
      powerLCD(true);
      //todo write display code here
    
    }else{ 
      powerLCD(false);
      // turn clear display
    
    }
  }
}

float readProbe(){
  //todo read probe temperature in celcius here
  return -1;
}
float changeReading(float temp, boolean inFahrenheit){
  if(inFahrenheit){
    //to do change from F to C here 
    return temp;
  }
  return -temp;
}

void powerLCD(boolean power){
  if (powerOn){
    //todo turn the LCD on
  }
  else{
    //todo turn the LCD off
  }
}
