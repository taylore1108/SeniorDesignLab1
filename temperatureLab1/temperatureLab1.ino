
const int buttonPin = 2; // digital pin two is wired to push button
const int switchPin = 4;
int buttonState = 0; // state of the button

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(switchPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(switchPin);
  if(digitalRead(switch_pin) == HIGH){
    buttonState = digitalRead(buttonPin);
    //todo update server here and 
    //read is "virtual button" is pressed
    //if("vitrualButton == HIGH) buttonState =HIGH;
  
    if (buttonState == HIGH) { // we only want to display locally when the button is pressed
    
    }else{ // turn display off clear display
    
    }

  }

}
