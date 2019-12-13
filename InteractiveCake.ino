#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
/******************************************************************************

When the piezo vibration sensor is triggered countdown the timer by 1,
then when countdown gets to zero activate the solenoid.


******************************************************************************/
Adafruit_7segment matrix = Adafruit_7segment();
const int PIEZO_PIN = A0; // Piezo output
int LED = 2; //the pin we connect the LED
int SOLENOID = 5; //the pin we connect the Solenoid
int flickerInterval = 100;
unsigned long startFlickerTime = 0;
const int VIBRATION_LEVEL_FOR_HIT=50;
const int HITS_TO_ACTIVATE = 3;
int countDownValue = HITS_TO_ACTIVATE;

unsigned long lastHit = 0;
unsigned long timeToReactToHit = 1500;

boolean areFlickering = false;
boolean reactingToHit = false;
int tonePlayTime = 500;
int vibrationLevel = 0;
void setup() 
{
  Serial.begin(9600);
  matrix.begin(0x70);
  pinMode(LED, OUTPUT); //set the LED pin as OUTPUT
  pinMode(SOLENOID, OUTPUT); //set the SOLENOID pin as OUTPUT
  digitalWrite(SOLENOID, LOW); // make sure solenoid is off
  matrix.print(countDownValue);
  matrix.writeDisplay();
}

void loop() 
{
  
  // Read Piezo ADC value in, and convert it to a voltage
  if(!reactingToHit){
    vibrationLevel = analogRead(PIEZO_PIN);
    Serial.println(vibrationLevel);
  }
  
  if(vibrationLevel > VIBRATION_LEVEL_FOR_HIT){
    // if not currently reacting to a hit read the time (i.e. it's a new hit)
    if(!reactingToHit){
      lastHit = millis();
      countDownValue= countDownValue - 1;
      matrix.print(countDownValue);
      matrix.writeDisplay();
    }
    reactingToHit = true;
    Serial.println("HIT");
    if(!areFlickering){
      digitalWrite(LED, HIGH); //write 1 or HIGH to led pin
      tone(13, 2000, 50);
    }
    
  }

  // see if time has run out for us to respond to hit
  if(millis() - lastHit > timeToReactToHit){
    // we've responded to the hit long enough
    reactingToHit = false;
    lastHit = 0;
    if(countDownValue==0){
      digitalWrite(SOLENOID, HIGH); 
      delay(150);
      digitalWrite(SOLENOID, LOW); 
      
      countDownValue = HITS_TO_ACTIVATE;
      matrix.print(countDownValue);
      matrix.writeDisplay();
    }
    
    
  }
  int mod = (millis()-lastHit)%100;

  if(reactingToHit && mod ==0){
    if(!areFlickering){
     startFlickerTime = millis(); 
    }
    areFlickering=true;
  }
  if(areFlickering){
    digitalWrite(LED, LOW); //write 0 or LOW to led pin
    
  }
  if(millis()-startFlickerTime > flickerInterval){
    areFlickering = false;
    startFlickerTime = 0;
  }
  if (!reactingToHit){
    vibrationLevel = 0;
    digitalWrite(LED, LOW); //write 0 or LOW to led pin
  }
    
}
