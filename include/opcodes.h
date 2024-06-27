#pragma once

#include <random>

#include "CPU.h"

// macros for easier access to CPU getters
// this is just for convenience
#define memory cpu.get_memory()
#define display cpu.get_display()
#define input cpu.get_input()
#define V cpu.get_registers()
#define I cpu.get_I()
#define pc cpu.get_pc()
#define sp cpu.get_sp()
#define stack cpu.get_stack()
#define delay_timer cpu.get_delay_timer()
#define sound_timer cpu.get_sound_timer()

// 00E0: clear the screen (CLS)
inline void opcode_00E0(CPU& cpu) { display.clear(); }

// 00EE: return from a subroutine (RET)
inline void opcode_00EE(CPU& cpu) {
  pc = stack[sp];
  sp--;
}

// 1NNN: jump to address NNN (JP addr)
inline void opcode_1NNN(CPU& cpu, uint16_t opcode) { pc = opcode & 0x0FFFu; }

// 2NNN: call subroutine at NNN (CALL addr)
inline void opcode_2NNN(CPU& cpu, uint16_t opcode) {
  sp++;
  stack[sp] = pc;
  pc = opcode & 0x0FFFu;
}

// 3XNN: skip next instruction if VX equals NN (SE Vx, byte)
inline void opcode_3XNN(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t byte = opcode & 0x00FFu;

  if (VX == byte) {
    pc += 2;
  }
}

// 4XNN: skip next instruction if VX doesn't equal NN (SNE Vx, byte)
inline void opcode_4XNN(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t byte = opcode & 0x00FFu;

  if (VX != byte) {
    pc += 2;
  }
}

// 5XY0: skip next instruction if VX equals VY (SE Vx, Vy)
inline void opcode_5XY0(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  if (VX == VY) {
    pc += 2;
  }
}

// 6XNN: set VX to NN (LD Vx, byte)
inline void opcode_6XNN(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t byte = opcode & 0x00FFu;

  VX = byte;
}

// 7XNN: add NN to VX (ADD Vx, byte)
inline void opcode_7XNN(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t byte = opcode & 0x00FFu;

  VX += byte;
}

// 8XY0: set VX to the value of VY (LD Vx, Vy)
inline void opcode_8XY0(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  VX = VY;
}

// 8XY1: set VX to VX OR VY (OR Vx, Vy)
inline void opcode_8XY1(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  VX |= VY;
}

// 8XY2: set VX to VX AND VY (AND Vx, Vy)
inline void opcode_8XY2(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  VX &= VY;
}

// 8XY3: set VX to VX XOR VY (XOR Vx, Vy)
inline void opcode_8XY3(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  VX ^= VY;
}

// 8XY4: add VY to VX, set VF to 1 if there's a carry, 0 otherwise (ADD Vx, Vy)
inline void opcode_8XY4(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  (VY > (0xFF - VX)) ? V[0xF] = 1 : V[0xF] = 0;

  VX += VY;
}

// 8XY5: subtract VY from VX, set VF to 0 if there's a borrow, 1 otherwise (SUB
// Vx, Vy)
inline void opcode_8XY5(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  (VX > VY) ? V[0xF] = 1 : V[0xF] = 0;

  VX -= VY;
}

// 8XY6: store the least significant bit of VX in VF and then shift VX to
// the right by 1 (SHR Vx)
inline void opcode_8XY6(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);

  V[0xF] = VX & 0x1;
  VX >>= 1;
}

// 8XY7: set VX to VY minus VX, set VF to 0 if there's a borrow, 1 otherwise
inline void opcode_8XY7(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  (VY > VX) ? V[0xF] = 1 : V[0xF] = 0;

  VX = VY - VX;
}

// 8XYE: store the most significant bit of VX in VF and then shift VX to the
// left by 1 (SHL Vx {, Vy})
inline void opcode_8XYE(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);

  V[0xF] = VX >> 7;
  VX <<= 1;
}

// 9XY0: skip next instruction if VX doesn't equal VY (SNE Vx, Vy)
inline void opcode_9XY0(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  if (VX != VY) {
    pc += 2;
  }
}

// ANNN: set I to the address NNN (LD I, addr)
inline void opcode_ANNN(CPU& cpu, uint16_t opcode) { I = opcode & 0x0FFFu; }

// BNNN: jump to the address NNN plus V0 (JP V0, addr)
inline void opcode_BNNN(CPU& cpu, uint16_t opcode) {
  pc = (opcode & 0x0FFFu) + V[0];
}

// CXNN: set VX to the result of a random byte & NN (RND Vx, byte)
inline void opcode_CXNN(CPU& cpu, uint16_t opcode) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dist(0, 255);

  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t byte = opcode & 0x00FFu;

  VX = dist(gen) & byte;
}

// DXYN: draw a sprite at position VX, VY with N bytes of sprite data starting
inline void opcode_DXYN(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  uint8_t& VY = cpu.get_vy(opcode);

  uint8_t x = VX;
  uint8_t y = VY;
  uint8_t height = opcode & 0x000Fu;
  const uint8_t* sprite = memory.get_pointer(I);

  bool collision = display.draw_sprite(x, y, sprite, height);
  V[0xF] = collision ? 1 : 0;
}

// EX9E: skip next instruction if key with the value of VX is pressed (SKP Vx)
inline void opcode_EX9E(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  if (input.is_key_down(VX)) {
    pc += 2;
  }
}

// EXA1: skip next instruction if key with the value of VX is not pressed (SKNP
// Vx)
inline void opcode_EXA1(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  if (!input.is_key_down(VX)) {
    pc += 2;
  }
}

// FX07: set VX to the value of the delay timer (LD Vx, DT)
inline void opcode_FX07(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  VX = delay_timer;
}

// FX0A: wait for a key press and store the result in VX (LD Vx, K)
inline void opcode_FX0A(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);

  if (input.is_any_key_down()) {
    for (int i = 0; i < 16; i++) {
      if (input.is_key_down(i)) {
        VX = i;
        return;
      }
    }
  } else {
    pc -= 2;
  }
}

// FX15: set the delay timer to the value of VX (LD DT, Vx)
inline void opcode_FX15(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  delay_timer = VX;
}

// FX18: set the sound timer to the value of VX (LD ST, Vx)
inline void opcode_FX18(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  sound_timer = VX;
}

// FX1E: add the value of VX to I (ADD I, Vx)
inline void opcode_FX1E(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  I += VX;
}

// FX29: set I to the memory address of the sprite data corresponding to the hex
// digit stored in VX (LD F, Vx)
inline void opcode_FX29(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);
  I = VX * 0x5;  // 5 bytes per character
}

// FX33: store the binary-coded decimal representation of VX at the addresses I,
// I+1, and I+2 (LD B, Vx)
inline void opcode_FX33(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);

  memory.write(I, VX / 100);
  memory.write(I + 1, (VX / 10) % 10);
  memory.write(I + 2, (VX % 100) % 10);
}

// FX55: store the values of V0 to VX in memory starting at address I (LD [I],
// Vx)
inline void opcode_FX55(CPU& cpu, uint16_t opcode) {
  uint8_t& VX = cpu.get_vx(opcode);

  for (int i = 0; i <= VX; i++) {
    memory.write(I + i, V[i]);
  }
}

// FX65: read V0 to VX with values from memory starting at address I (LD Vx,
// [I])
inline void opcode_FX65(CPU& cpu, uint16_t opcode) {
  uint8_t VX = (opcode & 0x0F00u) >> 8u;
  for (int i = 0; i <= VX; ++i) {
    V[i] = memory.read(I + i);
  }
}

// undefine macros to avoid conflicts
#undef memory
#undef display
#undef input
#undef V
#undef I
#undef pc
#undef sp
#undef stack
#undef delay_timer
#undef sound_timer