//
// Lights are turned around.
//
// author: aleksandar
//

#pragma once

#include "pattern.h"

class Looping : public Pattern {
public:
  Looping(
    const int pixel_count,
    Adafruit_NeoPixel &pixels,
    int intensity,
    int delay_ms)
      : Pattern(pixel_count, pixels),
      intensity(intensity),
      delay_ms(delay_ms) {
    // empty
  }

  void Initialize() override {
    next = -1;
    curr_round = 0;
  }

  void Iteration() override {
    if(next != -1) {
      pixels.setPixelColor(next, pixels.Color(0, 0, 0));
    }

    next++;
  
    if(next == pixel_count) {
      next = 0;
      curr_round++;
    }
    
    pixels.setPixelColor(next, pixels.Color(0, 0, intensity));
  }

  int GetDelayMs() override {
    return delay_ms;
  }

  int GetCurrentRound() override {
    return curr_round;
  }


private:
  const int intensity;
  const int delay_ms;

  int next;
  int curr_round;
};

