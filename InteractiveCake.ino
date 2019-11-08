/******************************************************************************

When the piezo vibration sensor is triggered countdown the timer by 1,
then when countdown gets to zero activate the solenoid.


******************************************************************************/
const int PIEZO_PIN = A0; // Piezo output
int LED = 2; //the pin we connect the LED
int SOLENOID = 5; //the pin we connect the Solenoid
int flickerInterval = 100;
unsigned long startFlickerTime = 0;
const VIBRATION_LEVEL_FOR_HIT=3000;
int countDownValue = 10;

unsigned long lastHit = 0;
unsigned long timeToReactToHit = 1500;

boolean areFlickering = false;
boolean reactingToHit = false;
int tonePlayTime = 500;
int vibrationLevel = 0;
void setup() 
{
  Serial.begin(9600);
   pinMode(LED, OUTPUT); //set the LED pin as OUTPUT
}

void loop() 
{
  
  // Read Piezo ADC value in, and convert it to a voltage
  if(!reactingToHit){
    vibrationLevel = analogRead(PIEZO_PIN);
  }
  
  if(vibrationLevel > VIBRATION_LEVEL_FOR_HIT){
    // if not currently reacting to a hit read the time (i.e. it's a new hit)
    if(!reactingToHit){
      lastHit = millis();
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
    digitalWrite(LED_LIGHT_HIT, LOW); //write 0 or LOW to led pin
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
