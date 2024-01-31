#include <SDL.h>
#include <cchip8/display.h>

#include <iostream>

namespace cchip8 {

void Display::Init(float pos_x, float pos_y, SDL_Renderer* renderer) {
  m_pos_x = pos_x;
  m_pos_y = pos_y;
  m_renderer = renderer;
}

void Display::DrawPixel(float x, float y) {
  if (m_renderer == nullptr) {
    std::cerr << "Renderer is not set!" << std::endl;
    return;
  }
  SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  const SDL_FRect block{(m_pos_x + x) * DISPLAY_SCALE,
                        (m_pos_y + y) * DISPLAY_SCALE, DISPLAY_SCALE,
                        DISPLAY_SCALE};
  SDL_RenderRect(m_renderer, &block);
  SDL_RenderFillRect(m_renderer, &block);
}

void Display::HandleEvent(const SDL_Event& event) {
  switch (event.type) {}
}

}  // namespace cchip8
