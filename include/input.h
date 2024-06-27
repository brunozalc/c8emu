#pragma once

#include <sys/_types/_u_int8_t.h>

#include <array>
#include <cstdint>

#include "SDL.h"

class Input {
 public:
  Input();
  void handle_event(const SDL_Event& event);
  bool is_key_down(u_int8_t key) const;

 private:
  std::array<bool, 16> key_state;
};
