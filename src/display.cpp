#include "display.h"

#include <stdint.h>

#include "SDL.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "iostream"

Display::Display() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    exit(1);
  }

  window =
      SDL_CreateWindow("c8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH * SCALE, HEIGHT * SCALE, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    exit(1);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    exit(1);
  }

  clear();
}

Display::~Display() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Display::clear() {
  for (auto& row : screen) {
    row.fill(false);
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

bool Display::draw_sprite(uint8_t x, uint8_t y, const uint8_t* sprite,
                          uint8_t n) {
  bool collision = false;

  for (uint8_t row = 0; row < n; ++row) {
    uint8_t sprite_byte = sprite[row];
    for (uint8_t col = 0; col < 8; ++col) {
      if ((sprite_byte & (0x80 >> col)) != 0) {
        uint8_t screen_x = (x + col) % WIDTH;
        uint8_t screen_y = (y + row) % HEIGHT;
        if (screen[screen_y][screen_x]) {
          collision = true;
        }
        screen[screen_y][screen_x] ^= true;
      }
    }
  }
  return collision;
}

void Display::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (screen[y][x]) {
        SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(renderer);
}
