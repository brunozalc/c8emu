#pragma once

#include <array>
#include <cstdint>

#include "SDL.h"

// CHIP-8 has a 16-key hexadecimal keypad
// the keys are mapped to the following keys on a standard keyboard:
// 1 2 3 C -> 1 2 3 4
// 4 5 6 D -> Q W E R
// 7 8 9 E -> A S D F
// A 0 B F -> Z X C V

class Input {
 public:
  Input();
  void handle_event(const SDL_Event& event);
  bool is_key_down(uint8_t key) const;
  bool is_any_key_down() const;

 private:
  std::array<bool, 16> key_state;
};
