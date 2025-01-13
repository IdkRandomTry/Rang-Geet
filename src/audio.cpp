
// OpenAL Headers
#include "al.h"
#include "alc.h"

typedef struct AudioContext {
  ALCdevice* device;
  ALCcontext* ctx;
  ALuint source;
  
  ALuint notes_played[8];
} AudioContext;

static AudioContext audio;



template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

static void generate_sine(int k, float freq) {
  int seconds = 1;
  unsigned sample_rate = 44100;
  double my_pi = 3.14159;
  size_t buf_size = seconds * sample_rate;
  
  short* samples = new short[buf_size];
  for (int i = 0; i < buf_size; i++) {
    samples[i] = 32760 * sin((2.f * my_pi * freq) / sample_rate * i);
    samples[i] = sgn(samples[i]) * 32760;
  }
  alBufferData(audio.notes_played[k], AL_FORMAT_MONO16, samples, buf_size, sample_rate);
  
  delete[] samples;
}


static void init_audio() {
  audio.device = alcOpenDevice(nullptr);
  if (!audio.device) {
    std::cout << "Failed to create OpenAL Device" << std::endl;
  }
  
  audio.ctx = alcCreateContext(audio.device, NULL);
  if (!alcMakeContextCurrent(audio.ctx)) {
    std::cout << "Failed to enable OpenAL Context on main thread" << std::endl;
  }
  
  
  ALfloat listener_ori[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  alListenerfv(AL_ORIENTATION, listener_ori);
  
  alGenSources((ALuint)1, &audio.source);
  alSourcef(audio.source,  AL_PITCH, 1);
  alSourcef(audio.source,  AL_GAIN,  0.1);
  alSource3f(audio.source, AL_POSITION, 0, 0, 0);
  alSource3f(audio.source, AL_VELOCITY, 0, 0, 0);
  alSourcei(audio.source,  AL_LOOPING,  AL_FALSE);
  
  alGenBuffers((ALuint) 8, audio.notes_played);
  
  generate_sine(0, 240);
  generate_sine(1, 270);
  generate_sine(2, 300);
  generate_sine(3, 320);
  generate_sine(4, 360);
  generate_sine(5, 400);
  generate_sine(6, 450);
  generate_sine(7, 480);
}

static void play_note(int i) {
  alSourcei(audio.source, AL_BUFFER, audio.notes_played[i]);
  alSourcePlay(audio.source);
}

static void stop_note() {
  alSourceStop(audio.source);
}


static void deinit_audio() {
  alDeleteBuffers(8, audio.notes_played);
  alDeleteSources(1, &audio.source);
  alcDestroyContext(audio.ctx);
  alcCloseDevice(audio.device);
}
