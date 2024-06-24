#include "CPU.h"

#include <iostream>

CPU::CPU(Memory& memory) : memory(memory) { initialize(); }

void CPU::initialize() {
  pc = 0x200;  // program counter starts at 0x200
  I = 0;       // reset index register
  sp = 0;      // reset stack pointer

  // clear stack and registers
  stack.fill(0);
  V.fill(0);

  // clear timers
  delay_timer = 0;
  sound_timer = 0;
}

void CPU::emulate_cycle() { fetch_decode_execute(); }

void CPU::fetch_decode_execute() {
  // fetch instruction
  uint16_t opcode = memory.read(pc) << 8 | memory.read(pc + 1);

  // decode and execute
  // for now, we will just increment the program counter to avoid infinite loop
  std::cout << "Executing opcode: " << std::hex << opcode << std::endl;
  pc += 2;
}