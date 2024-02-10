#include <cchip8/audio.h>

#include <cmath>

namespace cchip8 {

bool Audio::Init() {
  m_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, &m_spec,
                                       Callback, this);
  if (m_stream == nullptr) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }
  return true;
}

void Audio::Reset() {
  PauseTone();
  m_frequency = 440.0;
  m_volume = 32.0;
  m_wave_pos = 0.0;
  m_audio_buffer.fill(0);
}

void Audio::StartTone() {
  SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(m_stream));
}

void Audio::PauseTone() {
  SDL_PauseAudioDevice(SDL_GetAudioStreamDevice(m_stream));
}

bool Audio::IsPaused() const {
  return SDL_AudioDevicePaused(SDL_GetAudioStreamDevice(m_stream)) == SDL_TRUE;
}

double Audio::GetSample() const {
  return std::sin(m_wave_pos) * m_volume + AUDIO_8BIT_BIAS;
}

void Audio::IncrementWavePosition() {
  m_wave_pos += (m_frequency * 2.0 * M_PI) / AUDIO_SAMPLE_RATE;
}

void Audio::Quit() {
  if (m_stream != nullptr) SDL_DestroyAudioStream(m_stream);
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

}  // namespace cchip8
