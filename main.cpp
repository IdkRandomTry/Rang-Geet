#include "header/img_proccessing.h"

// OpenAL Headers
#include "al.h"
#include "alc.h"

int main()
{
  Image img;
  img.load("image/garbage man dilbert.jpg");
  
  Pixel p = img.get_pixel(0, 0);
  std::cout << "Pixel (0,0): Hue = " << p.hue << ", Saturation = " << p.saturation << ", Brightness = " << p.brightness << "%" << std::endl;
  
  
  
  
  
  
  // OpenAL Initialization
  
  ALCdevice* audio_device = alcOpenDevice(nullptr);
  if (!audio_device) {
    std::cout << "Failed to create OpenAL Device" << std::endl;
  }
  
  ALCcontext* audio_context = alcCreateContext(audio_device, NULL);
  if (!alcMakeContextCurrent(audio_context)) {
    std::cout << "Failed to enable OpenAL Context on main thread" << std::endl;
  }
  
  ALfloat listener_ori[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  alListenerfv(AL_ORIENTATION, listener_ori);
  
  ALuint audio_source;
  alGenSources((ALuint)1, &audio_source);
  alSourcef(audio_source,  AL_PITCH, 1);
  alSourcef(audio_source,  AL_GAIN,  0.5);
  alSource3f(audio_source, AL_POSITION, 0, 0, 0);
  alSource3f(audio_source, AL_VELOCITY, 0, 0, 0);
  alSourcei(audio_source,  AL_LOOPING,  AL_TRUE);
  
  //
  
  ALuint beepbuffer;
  alGenBuffers((ALuint)1, &beepbuffer);
  
  // Sin Wave Props
  float freq = 480.f;
  int seconds = 1;
  unsigned sample_rate = 44100;
  double my_pi = 3.14159;
  size_t buf_size = seconds * sample_rate;
  short* samples = new short[buf_size];
  for (int i = 0; i < buf_size; i++) {
    samples[i] = 32760 * sin( (2.f * my_pi * freq) / sample_rate * i);
  }
  alBufferData(beepbuffer, AL_FORMAT_MONO16, samples, buf_size, sample_rate);
  delete[] samples;
  
  
  alSourcei(audio_source, AL_BUFFER, beepbuffer);
  alSourcePlay(audio_source);
  int a;
  std::cin >> a;
  alSourceStop(audio_source);
  
  alDeleteBuffers(1, &beepbuffer);
  alDeleteSources(1, &audio_source);
  alcDestroyContext(audio_context);
  alcCloseDevice(audio_device);
  
  return 0;
}