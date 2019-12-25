#include <Adafruit_NeoPixel.h>

#include "colour_picker_preset.h"
#include "colour_picker_random.h"

#include "one_looping.h"
#include "all_on_all_off.h"
#include "all_on_sliding.h"
#include "glowing.h"
#include "strip.h"
#include "combiner.h"

void turn_off_all_pixels();

// set 50 for small and 240 for long strip
#define TOTAL_PIXELS 240
#define PIN 6

#if TOTAL_PIXELS==240
#define EXPECTED_ITER_DELAY 10l
#else
#define EXPECTED_ITER_DELAY 50l
#endif /* TOTAL_PIXELS==240 */
#define MIN_DELAY 10l
#define ROUNDS_IN_ONE_PATTERN 10l
#define TIME_IN_ONE_PATTERN (ROUNDS_IN_ONE_PATTERN * TOTAL_PIXELS * EXPECTED_ITER_DELAY)

#define INTENSITY 215

//
// Long strip.
//

#if TOTAL_PIXELS==240
// looping
#define LOOPING_DELAY 10
// all on then all off
#define ALL_ON_ALL_OFF_DELAY 10
// sliding
#define SLIDING_DELAY_ON 5
#define SLIDING_DELAY_BLINK 200
#define SLIDING_STEP 5

#define STRIP_LEN 10
#else
//
// Small chain.
//
// looping
#define LOOPING_DELAY 50
// all on then all off
#define ALL_ON_ALL_OFF_DELAY 50
// sliding
#define SLIDING_DELAY_ON 20
#define SLIDING_DELAY_BLINK 200
#define SLIDING_STEP 1
// strip
#define STRIP_LEN 3
#endif /* TOTAL_PIXELS==240 */

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(TOTAL_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

ColourPickerRandom colours_rnd(pixels, INTENSITY);

Looping looping(TOTAL_PIXELS, pixels, INTENSITY, LOOPING_DELAY);
ColourPickerPreset all_on_all_off_colours(pixels, INTENSITY);
AllOnAllOff all_on_all_off(TOTAL_PIXELS, pixels, all_on_all_off_colours, ALL_ON_ALL_OFF_DELAY);
ColourPickerPreset all_on_sliding_colours(pixels, INTENSITY);
AllOnSliding all_on_sliding(TOTAL_PIXELS, pixels, all_on_sliding_colours, SLIDING_DELAY_ON, SLIDING_DELAY_BLINK, SLIDING_STEP);
AllOnAllOff all_on_all_off_rnd(TOTAL_PIXELS, pixels, colours_rnd, ALL_ON_ALL_OFF_DELAY);
AllOnSliding all_on_sliding_rnd(TOTAL_PIXELS, pixels, colours_rnd, SLIDING_DELAY_ON, SLIDING_DELAY_BLINK, SLIDING_STEP);

Glowing glowing_blue(TOTAL_PIXELS, pixels, 8, 0, 0, 10, 0, 0, 250, 20, 20, 0, 0, 1, 0, 0, 30, 50);

// combined patterns
Strip strip_green_lr(TOTAL_PIXELS, pixels, pixels.Color(0, 125, 0), 0, TOTAL_PIXELS / 2 - 1, STRIP_LEN, 40, true);
Strip strip_red_rl(TOTAL_PIXELS, pixels, pixels.Color(125, 0, 0), TOTAL_PIXELS / 2, TOTAL_PIXELS - 1, STRIP_LEN, 40, false);
Strip strip_blue_mid_lr(TOTAL_PIXELS, pixels, pixels.Color(0, 0, 125), TOTAL_PIXELS / 3, TOTAL_PIXELS - TOTAL_PIXELS / 3 - 1, STRIP_LEN, 40, true);
Strip strip_pink_mid_rl(TOTAL_PIXELS, pixels, pixels.Color(70, 20, 20), TOTAL_PIXELS / 3, TOTAL_PIXELS - TOTAL_PIXELS / 3 - 1, STRIP_LEN, 40, false);
Pattern *combined_patterns[] = {
  &strip_green_lr,
  &strip_red_rl,
  &strip_blue_mid_lr,
  &strip_pink_mid_rl
};
static const size_t CombinedPatternCount = sizeof(combined_patterns) / sizeof(combined_patterns[0]);
PatternCombiner combiner(TOTAL_PIXELS, pixels, combined_patterns, CombinedPatternCount);

// use just last for the big 240 effect from 2017
Pattern *patterns[] = {
  &looping,
  &all_on_all_off,
  &all_on_sliding,
  &all_on_all_off_rnd,
  &all_on_sliding_rnd,
  &glowing_blue,
  &combiner
};

static const size_t PatternCount = sizeof(patterns) / sizeof(patterns[0]);

long total_delay = 0;
int last_seen_round;

int pattern_idx = 0;

void setup() {
  pixels.begin();
}

void loop() {
  // next pattern
  if(total_delay == 0) {
    turn_off_all_pixels();
    patterns[pattern_idx]->Initialize();
    last_seen_round = patterns[pattern_idx]->GetCurrentRound();
  }

  // update pixels and show
  patterns[pattern_idx]->Iteration();
  pixels.show();

  // wait between pixel changes
  int iter_delay = patterns[pattern_idx]->GetDelayMs();

  if(iter_delay < MIN_DELAY) {
    iter_delay = MIN_DELAY;
  }
  
  delay(iter_delay);
  total_delay += iter_delay;

  // switch to next pattern
  int this_round = patterns[pattern_idx]->GetCurrentRound();

  if(total_delay >= TIME_IN_ONE_PATTERN && this_round != last_seen_round) {
    pattern_idx++;
    if(pattern_idx == PatternCount) {
      pattern_idx = 0;
    }
    total_delay = 0;
  } else {
    last_seen_round = this_round;
  }
}

void turn_off_all_pixels() {
  for(int i = 0;i < TOTAL_PIXELS;i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }

  pixels.show();
}
