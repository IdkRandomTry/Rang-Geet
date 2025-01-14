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

