#ifndef CCHIP8_AUDIO_H_
#define CCHIP8_AUDIO_H_

#include <SDL.h>

#include <array>

#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_CHANNELS 1
#define AUDIO_8BIT_BIAS 127
#define AUDIO_BUFFER_SIZE 4096

namespace cchip8 {

class Audio {
 public:
  [[nodiscard]] bool Init();

  void Reset();
  void StartTone();
  void PauseTone();
  bool IsPaused() const;

  void setFrequency(double frequency) { m_frequency = frequency; }
  void setVolume(double volume) { m_volume = volume; }

  void Quit();

 private:
  static void Callback(void *userdata, SDL_AudioStream *stream,
                       [[maybe_unused]] int additional_amount,
                       int total_amount) {
    auto audio = reinterpret_cast<Audio *>(userdata);
    for (int sample = 0; sample < total_amount; ++sample) {
      audio->m_audio_buffer.at(sample) = audio->GetSample();
      audio->IncrementWavePosition();
    }
    SDL_PutAudioStreamData(stream, audio->m_audio_buffer.data(), total_amount);
  }

  double GetSample() const;
  void IncrementWavePosition();

  const SDL_AudioSpec m_spec{
      .format = SDL_AUDIO_U8,
      .channels = AUDIO_CHANNELS,
      .freq = AUDIO_SAMPLE_RATE,
  };
  SDL_AudioStream *m_stream;

  double m_frequency{440.0};  // A4
  double m_volume{32.0};
  double m_wave_pos{0.0};

  std::array<uint8_t, AUDIO_BUFFER_SIZE> m_audio_buffer;
};

}  // namespace cchip8

#endif  // CHIP8_AUDIO_H_
