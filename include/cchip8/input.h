#ifndef CCHIP8_INPUT_H_
#define CCHIP8_INPUT_H_

#include <SDL.h>

#include <array>
#include <cstdint>

#define NUM_KEYS 16
#define KEYBOARD_SIZE 255

namespace cchip8 {

class Input {
 public:
  void Reset();
  bool IsDown(uint8_t key) const;
  bool IsUp(uint8_t key) const;
  void HandleEvent(const SDL_Event &event);

 private:
  std::array<bool, KEYBOARD_SIZE> m_keyboard{false};
  std::array<SDL_Keycode, KEYBOARD_SIZE> m_keycode_map{
      SDLK_x,                         // 0 -> x
      SDLK_1, SDLK_2, SDLK_3,         // 1 2 3
      SDLK_q, SDLK_w, SDLK_e,         // 4 5 6
      SDLK_a, SDLK_s, SDLK_d,         // 7 8 9
      SDLK_z, SDLK_c,                 // A   B
      SDLK_4, SDLK_r, SDLK_f, SDLK_v  // C D E F (right vertical side)
  };
};

}  // namespace cchip8

#endif  // CCHIP8_INPUT_H
