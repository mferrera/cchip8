#ifndef CCHIP8_DISPLAY_H_
#define CCHIP8_DISPLAY_H_

#include <SDL.h>

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64
#define DISPLAY_SIZE (DISPLAY_HEIGHT * DISPLAY_WIDTH)
#define DISPLAY_SCALE 10

namespace cchip8 {

class Display {
 public:
  void Init(const float pos_x, const float pos_y, SDL_Renderer* renderer);
  void DrawPixel(const float x, const float y);
  void HandleEvent(const SDL_Event& event);

 private:
  float m_pos_x{0};
  float m_pos_y{0};
  SDL_Renderer* m_renderer = nullptr;
};

}  // namespace cchip8

#endif  // CCHIP8_DISPLAY_H_
