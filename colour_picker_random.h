//
// Randomly select colours.
//
// author: aleksandar
//

#pragma once

#include <Adafruit_NeoPixel.h>

#include "colour_picker.h"

class ColourPickerRandom : public ColourPicker {
public:
  ColourPickerRandom(
    Adafruit_NeoPixel &pixels,
    int intensity)
      :pixels(pixels),
      intensity(intensity) {
    // empty
  }

  void Initialize() override {
    // empty
  }

  void NextColour() override {
    // empty
  }

  uint32_t Get() override {
    return pixels.Color(
      random(intensity * 2 / 3),
      random(intensity * 2 / 3),
      random(intensity * 2 / 3));
  }

private:
  Adafruit_NeoPixel &pixels;
  int intensity;
};

