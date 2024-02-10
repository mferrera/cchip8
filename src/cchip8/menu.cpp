#include <cchip8/display.h>
#include <cchip8/events.h>
#include <cchip8/menu.h>

namespace cchip8 {

MenuItem Menu::CreateMenuItem(std::string text, TTF_Font *font) {
  auto item = TTF_RenderUTF8_Blended(font, text.c_str(), m_textColor);
  auto texture = SDL_CreateTextureFromSurface(m_renderer, item);
  SDL_DestroySurface(item);

  int textWidth = 0;
  int textHeight = 0;
  TTF_SizeUTF8(font, text.c_str(), &textWidth, &textHeight);
  auto width = static_cast<float>(textWidth + MENU_ITEM_BORDER);
  auto height = static_cast<float>(textHeight + MENU_ITEM_BORDER);

  /* Center it in menu rect */
  auto x = m_pos_x + (MENU_WIDTH / 2) - (textWidth / 2);

  SDL_FRect rect{x, m_menu_height, width, height};

  /* Adjust the bottom of the menu to after this line height */
  m_menu_height += height;

  return MenuItem{.texture = texture,
                  .rect = rect,
                  .width = width,
                  .height = height,
                  .selected = false};
}

void Menu::Init(const float x, const float y, SDL_Renderer *renderer) {
  m_renderer = renderer;
  m_pos_x = MENU_X + x;
  m_pos_y = y;
  m_menu_height = m_pos_y;

  m_header_font = TTF_OpenFont("assets/PixeloidMono.ttf", MENU_FONT_SIZE);
  m_font = TTF_OpenFont("assets/PixeloidMono.ttf", MENU_FONT_SIZE);

  TTF_SetFontStyle(m_header_font, TTF_STYLE_BOLD);
  m_header = CreateMenuItem("Menu", m_header_font);

  auto resume = CreateMenuItem("Resume", m_font);
  resume.selected = true;
  resume.event.type = SDL_RESUME_GAME;
  m_menuItems.push_back(resume);

  auto reset = CreateMenuItem("Reset", m_font);
  reset.event.type = SDL_RESET_GAME;
  m_menuItems.push_back(reset);

  auto exit = CreateMenuItem("Exit", m_font);
  exit.event.type = SDL_EVENT_QUIT;
  m_menuItems.push_back(exit);

  CenterOnYAxis();
}

void Menu::CenterOnYAxis() {
  m_pos_y = ((DISPLAY_HEIGHT * DISPLAY_SCALE) - m_menu_height) / 2;
  m_header.rect.y += m_pos_y;
  for (auto &item : m_menuItems) {
    item.rect.y += m_pos_y;
  }
}

void Menu::ResetSelected() {
  m_menuItems.at(m_selected_idx).selected = false;
  m_menuItems.at(0).selected = true;
}

void Menu::Display() {
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xDD);
  SDL_FRect rect{m_pos_x, m_pos_y, MENU_WIDTH, m_menu_height};
  SDL_RenderFillRect(m_renderer, &rect);

  SDL_RenderTexture(m_renderer, m_header.texture, nullptr, &m_header.rect);
  for (const auto &item : m_menuItems) {
    if (item.selected) {
      SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(m_renderer, 0x3A, 0x9B, 0xDC, 0x99);
      SDL_FRect rect{m_pos_x, item.rect.y, MENU_WIDTH, item.height};
      SDL_RenderFillRect(m_renderer, &rect);
    }
    SDL_RenderTexture(m_renderer, item.texture, nullptr, &item.rect);
  }
}

void Menu::SelectUp() {
  m_menuItems.at(m_selected_idx).selected = false;
  m_selected_idx =
      (m_selected_idx - 1 + m_menuItems.size()) % m_menuItems.size();
  m_menuItems.at(m_selected_idx).selected = true;
  Display();
}

void Menu::SelectDown() {
  m_menuItems.at(m_selected_idx).selected = false;
  m_selected_idx = (m_selected_idx + 1) % m_menuItems.size();
  m_menuItems.at(m_selected_idx).selected = true;
  Display();
}

void Menu::DoOption() { SDL_PushEvent(&m_menuItems.at(m_selected_idx).event); }

void Menu::Quit() {
  TTF_CloseFont(m_font);
  TTF_CloseFont(m_header_font);
  SDL_DestroyTexture(m_header.texture);
  for (const auto &item : m_menuItems) {
    SDL_DestroyTexture(item.texture);
  }
}

}  // namespace cchip8
