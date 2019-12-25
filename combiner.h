//
// Combine multiple patterns into one.
//
// author: aleksandar
//

#include <limits.h>

#include "pattern.h"

class PatternCombiner : public Pattern {
public:
  PatternCombiner(
    const int pixel_count,
    Adafruit_NeoPixel &pixels,
    Pattern **patterns,
    int count)
      : Pattern(pixel_count, pixels),
      patterns(patterns),
      count(count) {
    // empty
  }

  void Initialize() override {
    for(int i = 0;i < count;i++) {
      patterns[i]->Initialize();
    }
  }

  void Iteration() override {
    for(int i = 0;i < count;i++) {
      patterns[i]->Iteration();
    }
  }

  int GetDelayMs() override {
    int d = INT_MAX;

    for(int i = 0;i < count;i++) {
      int pd = patterns[i]->GetDelayMs();

      if(pd < d) {
        d = pd;
      }
    }

    return d;
  }

  int GetCurrentRound() override {
    int r = INT_MAX;

    for(int i = 0;i < count;i++) {
      int pr = patterns[i]->GetCurrentRound();

      if(pr < r) {
        r = pr;
      }
    }

    return r;    
  }

private:
  Pattern ** const patterns;
  const int count;
};

