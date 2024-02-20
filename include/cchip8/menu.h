#ifndef CCHIP8_MENU_H_
#define CCHIP8_MENU_H_

#include <SDL3_ttf/SDL_ttf.h>
#include <cchip8/display.h>

#include <string>
#include <vector>

#define MENU_FONT_SIZE 36
#define MENU_WIDTH (DISPLAY_WIDTH / 4) * DISPLAY_SCALE
#define MENU_X (MENU_WIDTH + (MENU_WIDTH / 2))
#define MENU_ITEM_BORDER 5

struct MenuItem {
  SDL_Texture *texture;
  SDL_FRect rect;
  float width;
  float height;
  bool selected;
  SDL_Event event;
};

namespace cchip8 {

class Menu {
 public:
  void Init(const float pos_x, const float pos_y, SDL_Renderer *renderer);
  void Display();
  void SelectUp();
  void SelectDown();
  void DoOption();
  void Quit();

 private:
  MenuItem CreateMenuItem(std::string text, TTF_Font *font);
  void CenterOnYAxis();
  void ResetSelected();

  float m_pos_x{0.0};
  float m_pos_y{0.0};

  float m_menu_height{0.0};

  SDL_Renderer *m_renderer = nullptr;
  MenuItem m_header{};

  std::vector<MenuItem> m_menuItems{};
  size_t m_selected_idx{0};

  TTF_Font *m_font = nullptr;
  TTF_Font *m_header_font = nullptr;
  SDL_Color m_textColor = {0xFF, 0xFF, 0xFF, 0xFF};
};

}  // namespace cchip8

#endif  // CCHIP8_MENU_H_
