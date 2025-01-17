#pragma once


enum class Instrument {
  Sine,
};

struct Note {
  float frequency;
  Instrument instrument;
  
  Note(float f = 0, Instrument i = Instrument::Sine)
  {
    frequency = f;
    instrument = i;
  }
};


// Linear map from [x0, x1] -> [y0, y1]
static float lmap(float x0, float x1, float y0, float y1, float t) {
  return y0 + ((t - x0) / (x1 - x0)) * (y1 - y0);
}