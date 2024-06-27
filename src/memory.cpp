#include "memory.h"

#include <stddef.h>

#include <fstream>
#include <iosfwd>
#include <iostream>

// initialize memory with zeroes
Memory::Memory() { memory.fill(0); }

// method to load ROMs (games) into memory
void Memory::load_rom(const char* filename) {
  // open file in binary mode and seek to the end
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (file.is_open()) {
    std::streampos size = file.tellg();  // get file size
    file.seekg(0, std::ios::beg);        // seek to beginning of file
    file.read(reinterpret_cast<char*>(&memory[0x200]),
              size);  // read file into memory starting at 0x200, which is the
                      // start of the ROM-destined space in memory
    file.close();
  } else {
    std::cerr << "Failed to load ROM file: " << filename << std::endl;
    exit(1);
  }
}

// method to load fontset into memory
void Memory::load_font() {
  for (size_t i = 0; i < fontset.size(); i++) {
    memory[i] = fontset[i];
  }
}

// method to read from memory
uint8_t Memory::read(uint16_t address) const { return memory[address]; }

// method to write to memory
void Memory::write(uint16_t address, uint8_t value) { memory[address] = value; }

// method to get a pointer to a memory address
const uint8_t* Memory::get_pointer(uint16_t address) const {
  return &memory[address];
}
