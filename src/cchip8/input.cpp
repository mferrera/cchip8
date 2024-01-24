#include <SDL.h>
#include <cchip8/input.h>

namespace cchip8 {

void Input::Reset() { m_keyboard.fill(false); }

bool Input::IsDown(uint8_t key) { return m_keyboard.at(m_keycode_map.at(key)); }

bool Input::IsUp(uint8_t key) { return !m_keyboard.at(m_keycode_map.at(key)); }

void Input::HandleEvent(const SDL_Event &event) {
  auto keycode = static_cast<size_t>(event.key.keysym.sym);
  switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
      if (keycode < KEYBOARD_SIZE) m_keyboard.at(keycode) = true;
      break;
    case SDL_EVENT_KEY_UP:
      if (keycode < KEYBOARD_SIZE) m_keyboard.at(keycode) = false;
      break;
  }
}
}  // namespace cchip8
