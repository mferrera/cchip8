#include <SDL.h>
#include <cchip8/display.h>

#define WINDOW_TITLE "CChip8"

namespace cchip8 {

bool Display::Init() {
  SDL_CreateWindowAndRenderer(DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale, 0,
                              &m_window, &m_renderer);
  if (m_window == nullptr || m_renderer == nullptr) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  SDL_SetWindowTitle(m_window, WINDOW_TITLE);
  running = true;
  return running;
}

void Display::Clear() {
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(m_renderer);
}

void Display::DrawPixel(float x, float y) {
  SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  const SDL_FRect block{x * scale, y * scale, scale, scale};
  SDL_RenderRect(m_renderer, &block);
  SDL_RenderFillRect(m_renderer, &block);
}

void Display::HandleEvent(const SDL_Event &event) {
  switch (event.type) {
    case SDL_EVENT_QUIT:
      running = false;
      break;
  }
}

void Display::RenderPresent() { SDL_RenderPresent(m_renderer); }

void Display::Quit() {
  if (m_window != nullptr) SDL_DestroyWindow(m_window);
  if (m_renderer != nullptr) SDL_DestroyRenderer(m_renderer);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

}  // namespace cchip8
