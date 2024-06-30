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
  auto last_cycle_time = std::chrono::high_resolution_clock::now();
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

    // get the current time
    auto current_time = std::chrono::high_resolution_clock::now();
    float elapsed_time =
        std::chrono::duration<float, std::chrono::milliseconds::period>(
            current_time - last_cycle_time)
            .count();

    if (elapsed_time > 1.0f / 300.0f) {
      // update the last cycle time
      last_cycle_time = current_time;

      // run the CPU
      cpu.cycle();

      // update the display
      display.render();
    }
  }

  SDL_Quit();
  return 0;
}
