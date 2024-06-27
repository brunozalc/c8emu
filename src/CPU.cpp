#include "CPU.h"

#include <stdint.h>

#include <iostream>

CPU::CPU(Memory &memory, Display &display, Input &input)
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

void CPU::cycle() { fetch_decode_execute(); }

void CPU::fetch_decode_execute() {
  // fetch instruction
  uint16_t opcode = memory.read(pc) << 8 | memory.read(pc + 1);

  // decode and execute
  pc += 2;
  process_opcode(opcode);
}

void CPU::process_opcode(uint16_t opcode) {
  switch (opcode & 0xF000) {  // mask the opcode to get the first nibble
    case 0x0000:
      switch (opcode & 0x00FF) {
        case 0x00E0:  // 00E0: clear the screen
          display.clear();
          break;
        case 0x00EE:  // 00EE: return from subroutine
          pc = stack[sp];
          sp--;
          break;
        default:
          std::cerr << "Unknown opcode: " << opcode << std::endl;
          break;
      }
      break;
    case 0x1000:  // 1NNN: JMP NNN
      pc = opcode & 0x0FFFu;
      break;
    case 0x2000:  // 2NNN: CALL at NNN
      sp++;
      stack[sp] = pc;
      pc = opcode & 0x0FFFu;
      break;
    case 0x3000:  // 3XNN: skip next instruction if VX = NN
      if (V[(opcode & 0x0F00u) >> 8u] == (opcode & 0x00FFu)) {
        pc += 2;
      }
      break;
    case 0x4000:  // 4XNN: skip next instruction if VX != NN
      if (V[(opcode & 0x0F00u) >> 8u] != (opcode & 0x00FFu)) {
        pc += 2;
      }
      break;
    case 0x5000:  // 5XY0: skip next instruction if VX = VY
      if (V[(opcode & 0x0F00u) >> 8u] == V[(opcode & 0x00F0u) >> 4u]) {
        pc += 2;
      }
      break;
    case 0x6000:  // 6XNN: VX = NN
      V[(opcode & 0x0F00u) >> 8u] = opcode & 0x00FFu;
      break;
    case 0x7000:  // 7XNN: VX += NN
      V[(opcode & 0x0F00u) >> 8u] += opcode & 0x00FFu;
      break;
    case 0x8000:  // 8XYN: bitwise operations
      switch (opcode & 0x000F) {
        case 0x0000:  // 8XY0: set VX = VY
          V[(opcode & 0x0F00u) >> 8u] = V[(opcode & 0x00F0u) >> 4u];
          break;
        case 0x0001:  // 8XY1: set VX = (VX | VY)
          V[(opcode & 0x0F00u) >> 8u] |= V[(opcode & 0x00F0u) >> 4u];
          break;
        case 0x0002:  // 8XY2: set VX = (VX & VY)
          V[(opcode & 0x0F00u) >> 8u] &= V[(opcode & 0x00F0u) >> 4u];
          break;
        case 0x0003:  // 8XY3: set VX = (VX ^ VY)
          V[(opcode & 0x0F00u) >> 8u] ^= V[(opcode & 0x00F0u) >> 4u];
          break;
        case 0x0004:  // 8XY4: set VX = VX + VY, set VF = carry
          V[0xF] = (V[(opcode & 0x0F00u) >> 8u] >
                    (0xFF - V[(opcode & 0x00F0u) >> 4u]))
                       ? 1
                       : 0;
          V[(opcode & 0x0F00u) >> 8u] += V[(opcode & 0x00F0u) >> 4u];
          break;
        case 0x0005:  // 8XY5: set VX = VX - VY, set VF = NOT borrow
          V[0xF] = (V[(opcode & 0x0F00u) >> 8u] > V[(opcode & 0x00F0u) >> 4u])
                       ? 1
                       : 0;
          V[(opcode & 0x0F00u) >> 8u] -= V[(opcode & 0x00F0u) >> 4u];
          break;
        case 0x0006:  // 8XY6: set VX = VX >> 1, set VF = LSB of VX
          V[0xF] = V[(opcode & 0x0F00u) >> 8u] & 0x1;
          V[(opcode & 0x0F00u) >> 8u] >>= 1;
          break;
        case 0x0007:  // 8XY7: set VX = VY - VX, set VF = NOT borrow
          V[0xF] = (V[(opcode & 0x00F0u) >> 4u] > V[(opcode & 0x0F00u) >> 8u])
                       ? 1
                       : 0;
          V[(opcode & 0x0F00u) >> 8u] =
              V[(opcode & 0x00F0u) >> 4u] - V[(opcode & 0x0F00u) >> 8u];
          break;
        case 0x000E:  // 8XYE: set VX = VX << 1, set VF = MSB of VX
          V[0xF] = V[(opcode & 0x0F00u) >> 8u] >> 7;
          V[(opcode & 0x0F00u) >> 8u] <<= 1;
          break;
        default:
          std::cerr << "Unknown opcode [8NNN] " << opcode << std::endl;
          break;
      }
      break;
    case 0x9000:  // 9XY0: skip next instruction if VX != VY
      if (V[(opcode & 0x0F00u) >> 8u] != V[(opcode & 0x00F0u) >> 4u]) {
        pc += 2;
      }
      break;
    case 0xA000:  // ANNN: set I = NNN
      I = opcode & 0x0FFFu;
      break;
    case 0xB000:  // BNNN: jump to NNN + V0
      pc = (opcode & 0x0FFFu) + V[0];
      break;
    case 0xC000:  // CXNN: set VX = random byte AND NN
      V[(opcode & 0x0F00u) >> 8u] = (rand() % 0xFF) & (opcode & 0x00FFu);
      break;
    case 0xD000: {  // DXYN: draw sprite at (VX, VY) and set VF = collision

      uint8_t x = V[(opcode & 0x0F00u) >> 8u];
      uint8_t y = V[(opcode & 0x00F0u) >> 4u];
      uint8_t height = opcode & 0x000Fu;
      const uint8_t *sprite = memory.get_pointer(I);

      bool collision = display.draw_sprite(x, y, sprite, height);
      V[0xF] = collision ? 1 : 0;
    } break;
    case 0xE000: {
      switch (opcode & 0x00FFu) {
        case 0x009E:  // EX9E: skip next instruction if key VX is pressed
          if (input.is_key_down(V[(opcode & 0x0F00u) >> 8u])) {
            pc += 2;
          }
          break;
        case 0x00A1:  // EXA1: skip next instruction if key VX is not pressed
          if (!input.is_key_down(V[(opcode & 0x0F00u) >> 8u])) {
            pc += 2;
          }
          break;
        default:
          std::cerr << "Unknown opcode [ENNN] " << opcode << std::endl;
          break;
      }
      break;
    }
    case 0xF000: {
      switch (opcode & 0x00FFu) {
        case 0x0007:  // FX07: set VX = delay timer
          V[(opcode & 0x0F00u) >> 8u] = delay_timer;
          break;
        case 0x000A:  // FX0A: wait for key press, store key in VX
        {
          bool key_pressed = false;
          for (int i = 0; i < 16; ++i) {
            if (input.is_key_down(i)) {
              V[(opcode & 0x0F00u) >> 8u] = i;
              key_pressed = true;
            }
          }
        } break;
        case 0x0015:  // FX15: set delay timer = VX
          delay_timer = V[(opcode & 0x0F00u) >> 8u];
          break;
        case 0x0018:  // FX18: set sound timer = VX
          sound_timer = V[(opcode & 0x0F00u) >> 8u];
          break;
        case 0x001E:  // FX1E: set I = I + VX
          I += V[(opcode & 0x0F00u) >> 8u];
          break;
        case 0x0029:  // FX29: set I = location of sprite for digit VX
          I = V[(opcode & 0x0F00u) >> 8u] * 0x5;
          break;
        case 0x0033:  // FX33: store BCD representation of VX in memory
          memory.write(I, V[(opcode & 0x0F00u) >> 8u] / 100);
          memory.write(I + 1, (V[(opcode & 0x0F00u) >> 8u] / 10) % 10);
          memory.write(I + 2, V[(opcode & 0x0F00u) >> 8u] % 10);
          break;
        case 0x0055:  // FX55: store V0 to VX in memory starting at address I
          for (int i = 0; i <= ((opcode & 0x0F00u) >> 8u); ++i) {
            memory.write(I + i, V[i]);
          }
          break;
        case 0x0065:  // FX65: fill V0 to VX with values from memory starting at
                      // address I
          for (int i = 0; i <= ((opcode & 0x0F00u) >> 8u); ++i) {
            V[i] = memory.read(I + i);
          }
        default:
          std::cerr << "Unknown opcode [FNNN] " << opcode << std::endl;
          break;
      }
      break;
    }
    default:
      std::cerr << "Unknown opcode [general] " << opcode << std::endl;
      break;
  }
}
