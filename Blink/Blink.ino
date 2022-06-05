#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 48

CRGB leds[NUM_LEDS];

void setup() { 
    FastLED.addLeds<SK6812, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() { 
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
}
