//
// Base interface of a pattern.
//
// author: aleksandar
//

#pragma once

#include <Adafruit_NeoPixel.h>

class Pattern {
public:
  Pattern(
    const int pixel_count,
    Adafruit_NeoPixel &pixels)
      : pixel_count(pixel_count),
      pixels(pixels) {
      // empty
    }
  
  virtual ~Pattern() = default;

  // Initialize state. Should reset state to the initial one.
  virtual void Initialize() = 0;
  // One iteration. Prefferably, do not invokde delay();
  virtual void Iteration() = 0;
  // Return delay at the end of one iteration.
  virtual int GetDelayMs() = 0;
  // Return the current round count. The controller might use this
  // to avoid breaking up a round when switching between patterns.
  virtual int GetCurrentRound() = 0;

protected:
  bool IsPixelInBounds(int pixel) const {
    return pixel >= 0 && pixel < pixel_count;
  }

  const int pixel_count;
  Adafruit_NeoPixel &pixels;
};

