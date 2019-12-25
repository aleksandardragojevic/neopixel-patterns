//
// Turn all one by one, then turn them off from the end.
//
// author: aleksandar
//

#pragma once

#include "pattern.h"
#include "colour_picker.h"

class AllOnAllOff : public Pattern {
public:
  AllOnAllOff(
    int pixel_count,
    Adafruit_NeoPixel &pixels,
    ColourPicker &colours,
    int delay_ms)
      : Pattern(pixel_count, pixels),
      colours(colours),
      delay_ms(delay_ms) {
    // empty
  }

  void Initialize() override {
    phase = On;
    next_pixel = 0;
    colours.Initialize();
    curr_round = 0;
  }

  void Iteration() override {
    if(phase == On) {
      pixels.setPixelColor(next_pixel, colours.Get());
    } else {
      pixels.setPixelColor(pixel_count - 1 - next_pixel, pixels.Color(0, 0, 0));
    }

    next_pixel++;

    if(next_pixel == pixel_count) {
      phase = 1 - phase;
      next_pixel = 0;

      if(phase == On) {
        curr_round++;
        colours.NextColour();
      }
    }
  }

  int GetDelayMs() override {
    return delay_ms;
  }

  int GetCurrentRound() override {
    return curr_round;
  }

private:
  enum Phase {
    On = 0,
    Off = 1
  };

  Phase phase;

  int next_pixel;
  int curr_round;

  ColourPicker &colours;

  const int delay_ms;
};



