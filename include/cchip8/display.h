#ifndef CCHIP8_DISPLAY_H_
#define CCHIP8_DISPLAY_H_

#include <SDL.h>

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64
#define DISPLAY_SIZE 2048

namespace cchip8 {

class Display {
 public:
  bool running{false};
  float scale{10};

  [[nodiscard]] bool Init();

  void Clear();
  void DrawPixel(float x, float y);
  void HandleEvent(const SDL_Event& event);
  void RenderPresent();

  void Quit();

 private:
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
};

}  // namespace cchip8

#endif  // CCHIP8_DISPLAY_H_
