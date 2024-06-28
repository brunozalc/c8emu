#include <iostream>

#include "CPU.h"
#include "SDL.h"
#include "SDL_events.h"
#include "display.h"
#include "memory.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  Memory memory;
  Display display;
  Input input;

  CPU cpu(memory, display, input);

  memory.load_rom(argv[1]);

  // main loop
  bool running = true;
  SDL_Event event;

  while (running) {
    // handle events (through the input class)
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else {
        input.handle_event(event);
      }
    }

    // execute one cycle of the CPU
    cpu.cycle();

    // render the display
    display.render();

    // delay to match the CHIP-8 speed (16ms for approx. 60Hz)
    SDL_Delay(16);
  }

  SDL_Quit();
  return 0;
}
