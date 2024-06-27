#include "CPU.h"

#include <stdint.h>

#include <iostream>

#include "display.h"
#include "opcodes.h"

CPU::CPU(Memory& memory, Display& display, Input& input)
    : memory(memory), display(display), input(input) {
  initialize();
}

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

void CPU::cycle() {
  // fetch instruction
  uint16_t opcode = memory.read(pc) << 8 | memory.read(pc + 1);

  // decode and execute
  pc += 2;
  process_opcode(opcode);
}

void CPU::process_opcode(uint16_t opcode) {
  switch (opcode & 0xF000) {
    case 0x0000:
      switch (opcode & 0x00FF) {
        case 0x00E0:
          opcode_00E0(*this);
          break;
        case 0x00EE:
          opcode_00EE(*this);
          break;
        default:
          std::cerr << "Unknown opcode [0x0000]: " << opcode << std::endl;
          break;
      }
      break;
    case 0x1000:
      opcode_1NNN(*this, opcode);
      break;
    case 0x2000:
      opcode_2NNN(*this, opcode);
      break;
    case 0x3000:
      opcode_3XNN(*this, opcode);
      break;
    case 0x4000:
      opcode_4XNN(*this, opcode);
      break;
    case 0x5000:
      opcode_5XY0(*this, opcode);
      break;
    case 0x6000:
      opcode_6XNN(*this, opcode);
      break;
    case 0x7000:
      opcode_7XNN(*this, opcode);
      break;
    case 0x8000:
      switch (opcode & 0x000F) {
        case 0x0000:
          opcode_8XY0(*this, opcode);
          break;
        case 0x0001:
          opcode_8XY1(*this, opcode);
          break;
        case 0x0002:
          opcode_8XY2(*this, opcode);
          break;
        case 0x0003:
          opcode_8XY3(*this, opcode);
          break;
        case 0x0004:
          opcode_8XY4(*this, opcode);
          break;
        case 0x0005:
          opcode_8XY5(*this, opcode);
          break;
        case 0x0006:
          opcode_8XY6(*this, opcode);
          break;
        case 0x0007:
          opcode_8XY7(*this, opcode);
          break;
        case 0x000E:
          opcode_8XYE(*this, opcode);
          break;
        default:
          std::cerr << "Unknown opcode [0x8000]: " << opcode << std::endl;
          break;
      }
      break;
    case 0x9000:
      opcode_9XY0(*this, opcode);
      break;
    case 0xA000:
      opcode_ANNN(*this, opcode);
      break;
    case 0xB000:
      opcode_BNNN(*this, opcode);
      break;
    case 0xC000:
      opcode_CXNN(*this, opcode);
      break;
    case 0xD000:
      opcode_DXYN(*this, opcode);
      break;
    case 0xE000:
      switch (opcode & 0x00FF) {
        case 0x009E:
          opcode_EX9E(*this, opcode);
          break;
        case 0x00A1:
          opcode_EXA1(*this, opcode);
          break;
        default:
          std::cerr << "Unknown opcode [0xE000]: " << opcode << std::endl;
          break;
      }
      break;
    case 0xF000:
      switch (opcode & 0x00FF) {
        case 0x0007:
          opcode_FX07(*this, opcode);
          break;
        case 0x000A:
          opcode_FX0A(*this, opcode);
          break;
        case 0x0015:
          opcode_FX15(*this, opcode);
          break;
        case 0x0018:
          opcode_FX18(*this, opcode);
          break;
        case 0x001E:
          opcode_FX1E(*this, opcode);
          break;
        case 0x0029:
          opcode_FX29(*this, opcode);
          break;
        case 0x0033:
          opcode_FX33(*this, opcode);
          break;
        case 0x0055:
          opcode_FX55(*this, opcode);
          break;
        case 0x0065:
          opcode_FX65(*this, opcode);
          break;
        default:
          std::cerr << "Unknown opcode [0xF000]: " << opcode << std::endl;
          break;
      }
      break;
    default:
      std::cerr << "Unknown opcode [general]: " << opcode << std::endl;
      break;
  }
}
