//HIGH is ON on the four channel Inland Relay Board
//LOW is OFF on the four channel Inland Relay Board
//HIGH is OFF on the two channel Inland Relay Board
//LOW is ON on the two channel Inland Relay Board

int credits = 0;
int hopperstate = 0;
const int button = 1; //player button
const int sensor = 3;   //hoper sensor
const int credit = 8; //credit input from semnox
const int light = 5; //relay
const int hopper = 7; //coin hopper, high for off
const int error = 12; //error light

// variables will change:
int buttonState = 0;
int sensorState = 0;
int creditState = 0;
long int t1 = 0;
long int t2 = 0;
int t3 = 0;
int t4 = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(hopper, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(error, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button, INPUT_PULLUP);
  pinMode(sensor, INPUT_PULLUP);
  pinMode(credit, INPUT_PULLUP);
  digitalWrite(hopper, LOW);
}

void loop() { 
  creditState = digitalRead(credit);
  if (creditState == LOW && t4 ==0){
    t1 = millis(); // t1 for initial start of 5 minute timer function
    credits = credits + 1; //when the card is tapped, credits increases by 1
    digitalWrite(light, HIGH); // turn button light on
    delay(500); //wait half a second
  }
  
  buttonState = digitalRead(button);
  if (buttonState == LOW && t4 == 0 && credits > 0) { //check if button is pressed, the board is in error, and credits are more than 0
    t2 = millis(); //start counting incase there is no tokens
    digitalWrite(hopper, HIGH); //turn hopper on
    while(digitalRead(sensor) == HIGH){  //While the sensor has no tokens in it, keep the code paused, and count to 10 for no token error
      delay(10);
      if (millis() - t2 >= 10000){ //if there is no tokens for 10 senconds, go into error
        t4 = 1;
        digitalWrite(hopper, LOW);
        digitalWrite(light, LOW);
        digitalWrite(error, HIGH); //connect error indicator to this relay
       }
    }
    t2 = millis(); //start counting incase there is no tokens
    while(digitalRead(sensor) == LOW){ //sensor will be LOW when there is a token present
      delay(10);
      if (millis() - t2 >= 10000){ //if the sensor is tripped for 10 seconds, go into error
        t4 = 1;
        digitalWrite(hopper, LOW);
        digitalWrite(light, LOW);
        digitalWrite(error, HIGH); //connect error indicator to this relay
      }
    }
    digitalWrite(hopper, LOW); //turn hopper off
    t1 = millis();  //reset the 5 minute timer
    if (credits > 0){
      credits = credits - 1;
    } else {
      credits = 0;
      digitalWrite(light, LOW);
    }
    delay(250);
  }
  if (millis() - t1 >= 300000 && t4 == 0 && credits > 0){
    digitalWrite(hopper, LOW);
    digitalWrite(light, LOW);
    credits = 0;
  }
  if (credits == 0){
    digitalWrite(light, LOW);
  }
}
