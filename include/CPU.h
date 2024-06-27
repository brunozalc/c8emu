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

  // getters
  Memory& get_memory();
  Display& get_display();
  Input& get_input();
  uint8_t* get_registers();
  uint8_t& get_vx(uint16_t opcode);
  uint8_t& get_vy(uint16_t opcode);
  uint16_t& get_I();
  uint16_t& get_pc();
  uint8_t& get_sp();
  uint16_t* get_stack();
  uint8_t& get_delay_timer();
  uint8_t& get_sound_timer();

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

  // opcode execution logic
  void process_opcode(uint16_t opcode);
};

// getters
inline Memory& CPU::get_memory() { return memory; }
inline Display& CPU::get_display() { return display; }
inline Input& CPU::get_input() { return input; }
inline uint8_t* CPU::get_registers() { return V.data(); }
inline uint8_t& CPU::get_vx(uint16_t opcode) {
  return V[(opcode & 0x0F00u) >> 8u];
}
inline uint8_t& CPU::get_vy(uint16_t opcode) {
  return V[(opcode & 0x00F0u) >> 4u];
}
inline uint16_t& CPU::get_I() { return I; }
inline uint16_t& CPU::get_pc() { return pc; }
inline uint8_t& CPU::get_sp() { return sp; }
inline uint16_t* CPU::get_stack() { return stack.data(); }
inline uint8_t& CPU::get_delay_timer() { return delay_timer; }
inline uint8_t& CPU::get_sound_timer() { return sound_timer; }
