byte petalCounter = 0;

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (mode != 0) {
       return; 
    }
    strip.setPixelColor(i, c);
    strip.show();
    if (++petalCounter == 3) {
      petalCounter = 0;
      updatePetal();
    }
    delay(wait);
    calculateDistance();
  }
}

void colorWipeReverse(uint32_t c) {
  int pos = strip.numPixels();
  while (--pos >= 0) {
    if (mode != 0) {
       return; 
    }
    strip.setPixelColor(pos, c);
    strip.show();
    if (++petalCounter == 3) {
      petalCounter = 0;
      updatePetal();
    }
    delay(wait);
    calculateDistance();
  }
}

void wiper() {
  
  pixelRunning = true;
  
  while (true) {
    
    colorWipe(strip.Color(0, 0, 0)); // Black
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }    
  
    colorWipe(strip.Color(0, 0, 64)); // Blue
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipeReverse(strip.Color(64,0,64)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    
    colorWipe(strip.Color(0, 0, 0)); // Black
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }      
    
    colorWipe(strip.Color(64,64,0)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipeReverse(strip.Color(0, 64, 64));
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    
    colorWipe(strip.Color(0, 0, 255)); // Blue
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipeReverse(strip.Color(255,0,255)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipe(strip.Color(255,255,0)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipeReverse(strip.Color(0, 255, 255));
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipe(strip.Color(255,255,0)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipe(strip.Color(64,64,0)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipeReverse(strip.Color(255,255,0)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }
    colorWipeReverse(strip.Color(64,64,0)); 
    if (mode != 0) {
       pixelRunning = false;
       return; 
    }    
  
  }
  
}
 
void rainbow() {
  uint16_t i, j;
  
  pixelRunning = true;
  
  while (true) {
    for(j=0; j<256; j++) {
      if (mode != 1) {
         pixelRunning = false;
         return; 
      }
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+j) & 255));
      }
      strip.show();
      if (++petalCounter == 3) {
        petalCounter = 0;
        updatePetal();
      }
      delay(wait);
      calculateDistance();
    }
  }
}
 
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i, j;
  
  pixelRunning = true;
  
  while (true) {
   
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
      if (mode != 2) {
         pixelRunning = false;
         return; 
      }
      for(i=0; i< strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      if (++petalCounter == 3) {
          petalCounter = 0;
          updatePetal();
      }
      delay(wait);
      calculateDistance();
    }
  }
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
  uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
 
/**
* ^ ^ ^
* ~~~~~ ColorWave ~~~~~
* V V V
*/
void colorWave() {
  int i, j, stripsize, cycle;
  float ang, rsin, gsin, bsin, offset;
   
  static int tick = 0;
  pixelRunning = true;
  stripsize = strip.numPixels();
  cycle = stripsize * 25; // times around the circle...
   
  //while (++tick % cycle) {
  while (true) {
    if (++tick == cycle) {
      tick = 0; 
    }
    if (mode != 3) {
       pixelRunning = false;
       return; 
    }
    offset = map2PI(tick);
     
    for (i = 0; i < stripsize; i++) {
      ang = map2PI(i) - offset;
      rsin = sin(ang);
      gsin = sin(2.0 * ang / 3.0 + map2PI(int(stripsize/6)));
      bsin = sin(4.0 * ang / 5.0 + map2PI(int(stripsize/3)));
      strip.setPixelColor(i, strip.Color(trigScale(rsin), trigScale(gsin), trigScale(bsin)));
    }
     
    strip.show();
    if (++petalCounter == 3) {
        petalCounter = 0;
        updatePetal();
    }
    delay(wait);
    calculateDistance();
  }
 
}
 
/**
* Scale a value returned from a trig function to a byte value.
* [-1, +1] -> [0, 254]
* Note that we ignore the possible value of 255, for efficiency,
* and because nobody will be able to differentiate between the
* brightness levels of 254 and 255.
*/
byte trigScale(float val) {
  val += 1.0; // move range to [0.0, 2.0]
  val *= 127.0; // move range to [0.0, 254.0]
   
  return int(val) & 255;
}
 
/**
* Map an integer so that [0, striplength] -> [0, 2PI]
*/
float map2PI(int i) {
  return PI*2.0*float(i) / float(strip.numPixels());
}

