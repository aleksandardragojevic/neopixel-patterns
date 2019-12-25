//
// Turn all one by one, then turn them off from the end.
//
// author: aleksandar
//

#pragma once

#include "pattern.h"
#include "colour_picker.h"

class AllOnSliding : public Pattern {
public:
  AllOnSliding(
    int pixel_count,
    Adafruit_NeoPixel &pixels,
    ColourPicker &colours,
    int delay_on,
    int delay_blink,
    int iter_step)
      : Pattern(pixel_count, pixels),
      colours(colours),
      delay_on(delay_on),
      delay_blink(delay_blink),
      iter_step(iter_step) {
    colours.Initialize();
  }

  void Initialize() override {
    phase = On;
    prev_pixel = -1;
    next_pixel = 0;
    last_pixel = pixel_count;
    curr_round = 0;
  }

  void Iteration() override {
    if(phase == On) {
      // turn on the new pixel and off the old one
      pixels.setPixelColor(next_pixel, colours.Get());

      if(prev_pixel >= 0) {
        pixels.setPixelColor(prev_pixel, pixels.Color(0, 0, 0));
      }

      prev_pixel = next_pixel;

      // calculate the next pixel to turn off
      next_pixel += iter_step;

      // make sure we turn on the last pixel we want to
      if((next_pixel >= last_pixel) && (prev_pixel != last_pixel - 1)) {
        next_pixel = last_pixel - 1;
      }

      // is this iteration done
      if(next_pixel >= last_pixel) {
        if(last_pixel == 1) {
          phase = Blinking;
          blink_count = 0;
        } else {
          prev_pixel = -1;
          next_pixel = 0;
          last_pixel--;
        }
      }
    } else if(phase == Blinking) {
      uint32_t c;

      if(blink_count % 2 == 1) {
        c = colours.Get();
      } else {
        c = pixels.Color(0, 0, 0);
      }
      
      for(int i = 0;i < pixel_count;i++) {
        pixels.setPixelColor(i, c);
      }

      blink_count++;

      if(blink_count == BlinksInRound) {
        phase = On;
        prev_pixel = -1;
        next_pixel = 0;
        last_pixel = pixel_count;

        curr_round++;

        colours.NextColour();
      }
    }
  }

  int GetDelayMs() override {
    if(phase == On) {
      return delay_on;
    }

    return delay_blink;
  }

  int GetCurrentRound() override {
    return curr_round;
  }


private:
  static const int BlinksInRound = 17;

  enum Phase {
    On = 0,
    Blinking = 1
  };

  Phase phase;

  int prev_pixel;
  int next_pixel;
  int last_pixel;

  int blink_count;

  int curr_round;

  ColourPicker &colours;

  const int delay_on;
  const int delay_blink;
  const int iter_step;
};



