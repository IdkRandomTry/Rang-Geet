#pragma once


enum class Instrument {
  Sine,
};

struct Note {
  float frequency;
  Instrument instrument;
};