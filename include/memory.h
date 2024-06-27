#pragma once

#include <array>
#include <cstdint>

#include "fonts.h"

class Memory {
 public:
  Memory();
  void load_rom(const char* filename);
  void load_font();
  uint8_t read(uint16_t address) const;
  void write(uint16_t address, uint8_t value);
  const uint8_t* get_pointer(uint16_t address) const;

 private:
  std::array<uint8_t, 4096> memory;  // CHIP-8 has 4KB of memory
};
