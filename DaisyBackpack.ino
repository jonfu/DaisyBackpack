#include <Adafruit_NeoPixel.h>
 
#define PIN 4
#define STRIPSIZE 24
#define PETALSIZE 8
#define MODESIZE 4
#define trigPin 13
#define echoPin 12
#define distDiffThreshod 20
#define TOOCLOSE 3

const int petal[] = { 5, 6, 8, 9, 10, 11, 2, 3 };

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
long distance = 72;
long distDiff = 0;
int mode = 0;
int wait = 144;
int petalIndex = PETALSIZE-1;
int fadeValue = 200;
boolean pixelRunning = false;
boolean fadeOut = true;

void setup() {
  //Serial.begin (9600);
  
  for (int i=0; i<PETALSIZE; i++) {
    pinMode(petal[i], OUTPUT);
  }  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  strip.begin();
  strip.setBrightness(5); // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if (pixelRunning == false) {
    
      for (int i=0; i<PETALSIZE; i++) {
        digitalWrite(petal[i], LOW);
      }    
    
     switch (mode) {
       case 0:
       wiper();
       break;
       
       case 1:
       rainbow();
       break;
       
       case 2:
       rainbowCycle();
       break;
       
       case 3:
       colorWave();
       break;
     } 
  }
}



void calculateDistance() {
 
  long duration, dist;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration/2) / 29.1;
  
  if (dist > 0 && dist < 200){
    
    if ( (distance < TOOCLOSE) && (dist > distDiffThreshod)) {
      mode = 0;
    } else {
      distDiff = abs(distance - dist);
      if (distDiff > distDiffThreshod) {
        changeMode();
      }
    }
    distance = dist;
    wait = distance * 2;
  }
  else {
    //Serial.println("Out of range");
  }
  
}

void changeMode() {
  if (++mode >= MODESIZE) {
    mode = 0; 
  }
}

void updatePetal() {
  
  if (distance < TOOCLOSE) {
    
    if (fadeOut) {
      fadeValue -= 15;
    } else {
      fadeValue += 15;
    }
    
    fadeValue = constrain(fadeValue,0,100);   //  keep LED fade value from going into negative numbers!
    
    if (fadeValue == 0) {
      fadeOut = false;
    }
    
    if (fadeValue == 100) {
      fadeOut = true; 
    }
    
    for (int i=0; i<PETALSIZE; i++) {
      analogWrite(petal[i], fadeValue);
    }
    
  } else {
  
    digitalWrite(petal[petalIndex], LOW);
    if (++petalIndex == PETALSIZE) {
       petalIndex = 0; 
    }
    digitalWrite(petal[petalIndex], HIGH);
  }
  
}
