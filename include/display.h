#pragma once

#include <array>
#include <cstdint>

#include "SDL.h"

class Display {
 public:
  static const int WIDTH = 64;
  static const int HEIGHT = 32;
  static const int SCALE = 10;

  Display();
  ~Display();
  void clear();
  bool draw_sprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t n);
  void render();

 private:
  std::array<std::array<bool, WIDTH>, HEIGHT> screen;
  SDL_Window* window;
  SDL_Renderer* renderer;
};
