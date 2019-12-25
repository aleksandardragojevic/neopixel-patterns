//
// Preset colours.
//
// author: aleksandar
//

#pragma once

#include <Adafruit_NeoPixel.h>

#include "colour_picker.h"

class ColourPickerPreset : public ColourPicker {
public:
  ColourPickerPreset(
      Adafruit_NeoPixel &pixels,
      int intensity) {
    colours[0] = pixels.Color(0, 0, intensity);
    colours[1] = pixels.Color(intensity, 0, 0);
    colours[2] = pixels.Color(0, intensity, 0);
  }

  void Initialize() override {
    next_colour = 0;
  }

  void NextColour() override {
    next_colour++;

    if(next_colour == ColourCount) {
      next_colour = 0;
    }
  }

  uint32_t Get() override {
    return colours[next_colour];
  }

private:
  static const size_t ColourCount = 3;

  uint32_t colours[ColourCount];
  int next_colour;
};

