
// OpenAL Headers
#include "al.h"
#include "alc.h"

typedef struct AudioContext {
  ALCdevice* device;
  ALCcontext* ctx;
  ALuint source;
  
  ALuint* note_buffers;
  
  Note* notes;
  float time_accumulator;
  int current_note;
  bool started;
} AudioContext;

static AudioContext audio;



template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

static float simple_envelope(float pct) {
  // Attack
  if (pct >= 0.0f && pct <= 0.15f)
    return pct * (1.0 / 0.15); // Linear rise to 1
  // Decay
  else if (pct > 0.15f && pct <= 0.4f)
    return 1.0f - ((pct - 0.15) * ((1.0f-0.85f) / (0.4f-0.15f))); // Linear drop to 0.85
  // Sustain
  else if (pct > 0.4f && pct <= 0.85f)
    return 0.85f; // Sustain 0.85
  // Release
  else if (pct > 0.85f && pct <= 1.0f)
    return 0.85f - ((pct - 0.85) * (0.85f / (1.0f-0.85f)));
  std::cout << pct << " is out of bounds [0, 1]" << std::endl;
  return 1.0f;
}

static void generate_sine(int k, float freq) {
  float seconds = 1;
  unsigned sample_rate = 44100;
  double my_pi = 3.14159;
  size_t buf_size = seconds * sample_rate;
  
  short* samples = new short[buf_size];
  for (int i = 0; i < buf_size; i++) {
    //samples[i] = 32760 *
    //sin((2.f * my_pi * freq) / sample_rate * i);
    float fade = 1.0f - float (i)/buf_size;
    samples[i] = fade*32760 * sin((2.f * my_pi * freq) / sample_rate * i);
  }
  alBufferData(audio.note_buffers[k], AL_FORMAT_MONO16, samples, buf_size, sample_rate);
  
  delete[] samples;
}


static void init_audio(Note* notes) {
  
  audio.notes = notes;
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
  
  audio.note_buffers = new ALuint[pixel_count];
  alGenBuffers((ALuint) pixel_count, audio.note_buffers);
  
  for (int i = 0; i < pixel_count; i++) {
    generate_sine(i, notes[i].frequency);
  }
  
  audio.time_accumulator = 0;
  audio.current_note = 0;
  audio.started = false;
}

static void refresh_melody(Note* notes) {
  for (int i = 0; i < pixel_count; i++) {
    generate_sine(i, notes[i].frequency);
  }
  audio.time_accumulator = 0;
  audio.current_note = 0;
  audio.started = false;
}

static void play_note(int i) {
  alSourcei(audio.source, AL_BUFFER, audio.note_buffers[i]);
  alSourcePlay(audio.source);
}

static void stop_note() {
  alSourceStop(audio.source);
}



static void update_audio(float dt) {
  if (!audio.started) {
    play_note(0);
    audio.started = true;
  }
  audio.time_accumulator += dt;
  if (audio.time_accumulator >= note_time) {
    audio.time_accumulator -= note_time;
    audio.current_note += 1;
    audio.current_note %= pixel_count;
    stop_note();
    play_note(audio.current_note);
    printf("Playing note %d (%3.3f)\n", audio.current_note, audio.notes[audio.current_note].frequency);
    fflush(stdout);
  }
}


static void deinit_audio() {
  alDeleteBuffers(pixel_count, audio.note_buffers);
  delete[] audio.note_buffers;
  alDeleteSources(1, &audio.source);
  alcDestroyContext(audio.ctx);
  alcCloseDevice(audio.device);
}
