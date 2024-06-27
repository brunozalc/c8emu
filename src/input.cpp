#include "input.h"

#include "SDL_events.h"

Input::Input() { key_state.fill(false); }

void Input::handle_event(const SDL_Event& event) {
  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
    bool is_pressed = (event.type == SDL_KEYDOWN);
    switch (event.key.keysym.sym) {
      case SDLK_1:
        key_state[0x1] = is_pressed;
        break;
      case SDLK_2:
        key_state[0x2] = is_pressed;
        break;
      case SDLK_3:
        key_state[0x3] = is_pressed;
        break;
      case SDLK_4:
        key_state[0xC] = is_pressed;
        break;
      case SDLK_q:
        key_state[0x4] = is_pressed;
        break;
      case SDLK_w:
        key_state[0x5] = is_pressed;
        break;
      case SDLK_e:
        key_state[0x6] = is_pressed;
        break;
      case SDLK_r:
        key_state[0xD] = is_pressed;
        break;
      case SDLK_a:
        key_state[0x7] = is_pressed;
        break;
      case SDLK_s:
        key_state[0x8] = is_pressed;
        break;
      case SDLK_d:
        key_state[0x9] = is_pressed;
        break;
      case SDLK_f:
        key_state[0xE] = is_pressed;
        break;
      case SDLK_z:
        key_state[0xA] = is_pressed;
        break;
      case SDLK_x:
        key_state[0x0] = is_pressed;
        break;
      case SDLK_c:
        key_state[0xB] = is_pressed;
        break;
      case SDLK_v:
        key_state[0xF] = is_pressed;
        break;
    }
  }
}

bool Input::is_key_down(uint8_t key) const { return key_state[key]; }

bool Input::is_any_key_down() const {
  for (bool key : key_state) {
    if (key) {
      return true;
    }
  }
  return false;
}
