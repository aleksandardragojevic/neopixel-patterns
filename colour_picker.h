//
// Interface to choosing colours.
//
// author: aleksandar
//

#pragma once

#include <stdint.h>

class ColourPicker {
public:
  virtual ~ColourPicker() = default;

  // initialize internal state
  virtual void Initialize() = 0;

  // move onto next colour
  virtual void NextColour() = 0;

  // get the current colour
  virtual uint32_t Get() = 0;
};



