//
// Lights are turned around.
//
// author: aleksandar
//

#pragma once

#include "pattern.h"

class Glowing : public Pattern {
public:
  Glowing(
    const int pixel_count,
    Adafruit_NeoPixel &pixels,
    int head_len,
    int head_red_min,
    int head_green_min,
    int head_blue_min,
    int head_red_max,
    int head_green_max,
    int head_blue_max,
    int tail_len,
    int tail_prob_on,
    int tail_red_min,
    int tail_green_min,
    int tail_blue_min,
    int tail_red_max,
    int tail_green_max,
    int tail_blue_max,
    int delay_ms)
      : Pattern(pixel_count, pixels),
      head_len(head_len),
      head_red_min(head_red_min),
      head_green_min(head_green_min),
      head_blue_min(head_blue_min),
      head_red_max(head_red_max),
      head_green_max(head_green_max),
      head_blue_max(head_blue_max),
      tail_len(tail_len),
      tail_prob_on(tail_prob_on),
      tail_red_min(tail_red_min),
      tail_green_min(tail_green_min),
      tail_blue_min(tail_blue_min),
      tail_red_max(tail_red_max),
      tail_green_max(tail_green_max),
      tail_blue_max(tail_blue_max),
      delay_ms(delay_ms) {
    // empty
  }

  void Initialize() override {
    next = 0;
    curr_round = 0;
  }

  void Iteration() override {
    if(IsPixelInBounds(next - head_len - tail_len)) {
      pixels.setPixelColor(next - head_len - tail_len, pixels.Color(0, 0, 0));
    }

    for(int i = 0;i < head_len;i++) {
      if(IsPixelInBounds(next - i)) {
        pixels.setPixelColor(next - i, GetHeadColour());
      }
    }

    if(IsPixelInBounds(next - head_len - 1)) {
      if(random(100) < tail_prob_on) {
        pixels.setPixelColor(next - head_len - 1, GetTailColour());
      } else {
        pixels.setPixelColor(next - head_len - 1, pixels.Color(0, 0, 0));
      }
    }

    next++;
  
    if(next - head_len - tail_len == pixel_count) {
      next = 0;
      curr_round++;
    }
  }

  uint32_t GetHeadColour() {
    return pixels.Color(
      random(head_red_min, head_red_max),
      random(head_green_min, head_green_max),
      random(head_blue_min, head_blue_max));
  }

  uint32_t GetTailColour() {
    return pixels.Color(
      random(tail_red_min, tail_red_max),
      random(tail_green_min, tail_green_max),
      random(tail_blue_min, tail_blue_max));
  }

  int GetDelayMs() override {
    return delay_ms;
  }

  int GetCurrentRound() override {
    return curr_round;
  }


private:
  const int head_len;
  const int head_red_min;
  const int head_green_min;
  const int head_blue_min;
  const int head_red_max;
  const int head_green_max;
  const int head_blue_max;
  const int tail_len;
  const int tail_prob_on;
  const int tail_red_min;
  const int tail_green_min;
  const int tail_blue_min;
  const int tail_red_max;
  const int tail_green_max;
  const int tail_blue_max;
  const int delay_ms;

  int next;
  int curr_round;
};

