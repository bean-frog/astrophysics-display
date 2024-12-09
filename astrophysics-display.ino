/*
* Astrophysics Display - General Relativity
* https://github.com/bean-frog/astrophysics-display
* Copyright (c) 2024 by Graeme Kieran and Giacomo Resmini
* MIT License - see LICENSE.md
*/
#include "Arduino.h"
#include "Stepper.h"
#include "Adafruit_NeoPixel.h"

// constants and setup
#define NEOPIXEL_DATA_PIN 6
#define NUM_PIXELS 31
#define DFPLAYER_BUSY 7 
#define BUTTON_CW 8 //turns stepper clockwise
#define BUTTON_CCW 9 //turns stepper counterclockwise

const int CHASER_DELAY = 100; // in ms
const int CHASER_BRIGHTNESS = 8; //brightness of secondary pixels (all but original star)
const int STEPS_PER_REVOLUTION = 2048;
bool isMoving = false; //tracks stepper state

Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_DATA_PIN, NEO_GRB + NEO_KHZ800);
Stepper elevator = Stepper(STEPS_PER_REVOLUTION, 2, 4, 3, 5);

void setup() {
  pinMode(DFPLAYER_BUSY, INPUT_PULLUP); 
  strip.begin();
  strip.clear();
  strip.setPixelColor(15, strip.Color(255, 255, 255));
  strip.show();

  pinMode(BUTTON_CW, INPUT_PULLUP);
  pinMode(BUTTON_CCW, INPUT_PULLUP);
  elevator.setSpeed(15);
}

void loop() {
  if (digitalRead(DFPLAYER_BUSY) == LOW) {
      delay(100)
      for (int i = 0; i < 5; i)
      chaser(CHASER_DELAY);
    delay(500);
  }
  if (digitalRead(BUTTON_CW) == LOW) {
    elevator.step(STEPS_PER_REVOLUTION / 4);
    isMoving = true;
  } else if (digitalRead(BUTTON_CCW) == LOW) {
    elevator.step(-STEPS_PER_REVOLUTION / 4);
    isMoving = true;
  } else {
    isMoving = false;
  }
  delay(50);
}

void chaser(int delayTime) {
  strip.setPixelColor(15, strip.Color(255, 255, 255)); 
  strip.show();
  for (int i = 0; i < NUM_PIXELS / 2; i++) {
    // Right side (14-0)
    strip.setPixelColor(14 - i, strip.Color(CHASER_BRIGHTNESS, CHASER_BRIGHTNESS, CHASER_BRIGHTNESS));
    if (14 - i - 1 >= 0) {
      strip.setPixelColor(14 - i - 1, strip.Color(0, 0, 0)); 
    }
    // Left side (16-30)
    strip.setPixelColor(16 + i, strip.Color(CHASER_BRIGHTNESS, CHASER_BRIGHTNESS, CHASER_BRIGHTNESS)); 
    if (16 + i + 1 < NUM_PIXELS) {
      strip.setPixelColor(16 + i + 1, strip.Color(0, 0, 0)); 
    }
    strip.show();
    delay(delayTime);
  }
  strip.clear();
  strip.setPixelColor(15, strip.Color(255, 255, 255));
  strip.show();
}
