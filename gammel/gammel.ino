#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define NEOPIXEL_PIN 0

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
byte mode = 0;
byte lastButtonValue = 0;
byte col = 0;
byte dir = 1;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  byte val = digitalRead(4);
  if( val == HIGH && lastButtonValue != val ){
    mode++;
    delay(100);
  }
  lastButtonValue = digitalRead(4);
  
  if( mode == 0){
    for(uint8_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, 0,  0,  col);
      strip.show();
      delay(1);
      if( digitalRead(4) == HIGH){ mode++;delay(500);return; }
    }
    
    if( dir == 1 ){
      col++;
    } else {
      col--;
    }
    
    if(col >= 254 && dir == 1){
      dir = 0;
    } else if(col <= 25 ){
      dir = 1;
    }
  }
  else if(mode == 1)
  {
    rainbowCycle(20);
  }
  else if(mode == 2)
  {
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    if( digitalRead(4) == HIGH){ mode++;delay(500);return; }
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    if( digitalRead(4) == HIGH){ mode++;delay(500);return; }
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
  }
  else if(mode == 3)
  {
    colorWipe(strip.Color(255, 0, 0), 50); // just red
  }
  else if(mode == 4)
  {
    colorWipe(strip.Color(0, 0, 255), 50); // just blue
  }
  else if(mode == 5)
  {
    colorWipe(strip.Color(255, 255, 255), 50); // just white
  }
  else if(mode == 6)
  {
    colorWipe(strip.Color(0, 0, 0), 50); // off
  }
  if( mode >= 7 ){
    mode = 0;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
      if( digitalRead(4) == HIGH){ mode++;delay(500);return; }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    if( digitalRead(4) == HIGH){ mode++;delay(500);return; }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    if( digitalRead(4) == HIGH){ mode++;delay(500);return; }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

