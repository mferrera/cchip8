#include <SDL.h>
#include <cchip8/display.h>
#include <cchip8/window.h>

namespace cchip8 {

bool Window::Init() {
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH * WINDOW_SCALE,
                              WINDOW_HEIGHT * WINDOW_SCALE, 0, &m_window,
                              &m_renderer);
  if (m_window == nullptr || m_renderer == nullptr) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }
  SDL_SetWindowTitle(m_window, WINDOW_TITLE);
  m_display.Init(0, 0, m_renderer);
  running = true;
  return running;
}

void Window::Clear() {
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(m_renderer);
}

void Window::DrawDisplay(const Memory &memory) {
  for (auto y = 0; y < DISPLAY_HEIGHT; ++y) {
    for (auto x = 0; x < DISPLAY_WIDTH; ++x) {
      if (memory.vram.at(x + (y * DISPLAY_WIDTH))) {
        m_display.DrawPixel(x, y);
      }
    }
  }
}

void Window::Draw(const Memory &memory) {
  Clear();
  DrawDisplay(memory);
  Render();
}

void Window::HandleEvent(const SDL_Event &event) {
  switch (event.type) {
    case SDL_EVENT_QUIT:
      running = false;
      break;
  }
}

void Window::Render() { SDL_RenderPresent(m_renderer); }

void Window::Quit() {
  if (m_renderer != nullptr) SDL_DestroyRenderer(m_renderer);
  if (m_window != nullptr) SDL_DestroyWindow(m_window);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

}  // namespace cchip8
