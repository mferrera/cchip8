#ifndef CCHIP8_WINDOW_H_
#define CCHIP8_WINDOW_H_

#include <SDL.h>
#include <cchip8/display.h>
#include <cchip8/memory.h>

#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define WINDOW_SCALE 10
#define WINDOW_TITLE "CChip8"

namespace cchip8 {

class Window {
 public:
  bool running{false};

  [[nodiscard]] bool Init();

  void Draw(const Memory& memory);
  void HandleEvent(const SDL_Event& event);
  void Quit();

  void Clear();

 private:
  void DrawDisplay(const Memory& memory);
  void Render();

  Display m_display{};
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
};

}  // namespace cchip8

#endif  // CCHIP8_WINDOW_H_
