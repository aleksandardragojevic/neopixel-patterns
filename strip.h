//
// Strip of lights going back and forth.
//
// author: aleksandar
//

#pragma once

#include "pattern.h"
#include "colour_picker.h"

// all indices here are inclusive (from 10 to 20 means both 10 and 20 are on)
class Strip : public Pattern {
public:
  Strip(
    const int pixel_count,
    Adafruit_NeoPixel &pixels,
    uint32_t colour,
    int first_pixel,
    int last_pixel,
    int strip_len,
    int delay_ms,
    bool start_from_left)
      : Pattern(pixel_count, pixels),
      colour(colour),
      first_pixel(first_pixel),
      last_pixel(last_pixel),
      strip_len(strip_len),
      delay_ms(delay_ms),
      start_from_left(start_from_left) {
    // empty
  }

  void Initialize() override {
    if(start_from_left) {
      one = first_pixel;
      one_forward = true;
  
      two = one - strip_len + 1;
      two_forward = true;

      old_from = -1;
      old_to = -1;

    } else {
      one = last_pixel;
      one_forward = false;

      two = last_pixel + strip_len - 1;
      two_forward = false;

      old_from = last_pixel + 1;
      old_to = last_pixel + 1;
    }
        
    curr_round = 0;
  }

  void Iteration() override {
    int from;
    int to;

    if(one_forward && two_forward) {
      from = two;
      to = one;
    } else if(!one_forward && !two_forward) {
      from = one;
      to = two;
    } else if(!one_forward && two_forward) {
      from = min(one, two);
      to = last_pixel;
    } else {
      from = 0;
      to = max(one, two);
    }

    // turn on new pixels
    if(from < old_from) {
      if(IsPixelInMyBounds(from)) {
        pixels.setPixelColor(from, pixels.getPixelColor(from) + colour);
      }      
    }

    if(to > old_to) {
      if(IsPixelInMyBounds(to)) {
        pixels.setPixelColor(to, pixels.getPixelColor(to) + colour);
      }      
    }

    // turn off old
    if(old_from < from) {
      if(IsPixelInMyBounds(old_from)) {
        pixels.setPixelColor(old_from, pixels.getPixelColor(old_from) - colour);
      }
    }

    if(old_to > to) {
      if(IsPixelInMyBounds(old_to)) {
        pixels.setPixelColor(old_to, pixels.getPixelColor(old_to) - colour);
      }
    }

    // remember current window
    old_from = from;
    old_to = to;

    // update one and two
    if(one_forward) {
      one += 1;

      if(one > last_pixel) {
        one = last_pixel;
        one_forward = false;
      }
    } else {
      one -= 1;

      if(one < first_pixel) {
        one = first_pixel;
        one_forward = true;
      }
    }

    if(two_forward) {
      two += 1;

      if(two > last_pixel) {
        two = last_pixel;
        two_forward = false;
      }

      if(!start_from_left) {
        curr_round += 1;
      }
    } else {
      two -= 1;

      if(two < first_pixel) {
        two = first_pixel;
        two_forward = true;

        if(start_from_left) {
          curr_round += 1;
        }
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
  bool IsPixelInMyBounds(int pixel) const {
    return pixel >= first_pixel && pixel <= last_pixel;
  }

  uint32_t colour;
  const int first_pixel;
  const int last_pixel;
  const int strip_len;
  const int delay_ms;
  const bool start_from_left;

  int one;
  bool one_forward;

  int two;
  int two_forward;

  int old_from;
  int old_to;
  
  int curr_round;
};

