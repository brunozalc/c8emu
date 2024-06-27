#pragma once

#include <array>
#include <cstdint>

#include "display.h"
#include "input.h"
#include "memory.h"

class CPU {
 public:
  CPU(Memory& memory, Display& display, Input& input);
  void initialize();
  void cycle();

 private:
  // registers
  std::array<uint8_t, 16> V;       // general purpose registers (V0 to VF)
  uint16_t I;                      // index register
  uint16_t pc;                     // program counter
  uint8_t sp;                      // stack pointer
  std::array<uint16_t, 16> stack;  // stack

  // memory reference for easier access
  Memory& memory;

  // display reference for easier access
  Display& display;

  // input reference for easier access
  Input& input;

  // timers
  uint8_t delay_timer;
  uint8_t sound_timer;

  // fetch, decode and execute cycle
  void fetch_decode_execute();
  void process_opcode(uint16_t opcode);
};
