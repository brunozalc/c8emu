#include <iostream>

#include "memory.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
    return 1;
  }

  Memory memory;
  memory.load_rom(argv[1]);

  // simple test to check if the ROM is loaded correctly
  for (int i = 0x200; i < 0x210;
       ++i) {  // adjust range based on the size of your ROM
    std::cout << std::hex << "Memory at " << i << ": "
              << static_cast<int>(memory.read(i)) << std::endl;
  }

  return 0;
}
