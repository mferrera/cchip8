#ifndef CCHIP8_MEMORY_H_
#define CCHIP8_MEMORY_H_

#include <cchip8/display.h>
#include <cchip8/rom.h>

#include <array>
#include <cstdint>

#define RAM_SIZE 4096
#define SPRITES_SIZE 80
#define STACK_SIZE 16

#define SPRITES_LOCATION 0x50
#define PROGRAM_START 0x200

static constexpr std::array<uint8_t, SPRITES_SIZE> SPRITES = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

namespace cchip8 {

/*
 * Memory Map
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.0
 *
 * +---------------+= 0xFFF (4095) End of Chip-8 RAM
 * |               |
 * |               |
 * |               |
 * |               |
 * |               |
 * | 0x200 to 0xFFF|
 * |     Chip-8    |
 * | Program / Data|
 * |     Space     |
 * |               |
 * |               |
 * |               |
 * +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
 * |               |
 * |               |
 * |               |
 * +---------------+= 0x200 (512) Start of most Chip-8 programs
 * | 0x000 to 0x1FF|
 * | Reserved for  |
 * |  interpreter  |
 * +---------------+= 0x000 (0) Start of Chip-8 RAM
 *
 */

class Memory {
 public:
  std::array<uint8_t, RAM_SIZE> ram{};
  std::array<bool, DISPLAY_SIZE> vram{};
  std::array<uint16_t, STACK_SIZE> stack{};

  void Reset();
  bool LoadProgram(const Rom& rom, const size_t location);

  void ClearVram() { vram.fill(false); }
  void ClearRam() { ram.fill(0); }

  void DumpMem() const;
  void DumpStack() const;
  void PrintByte(const uint8_t& byte) const;
  void PrintAddress(const uint16_t& address) const;
};

}  // namespace cchip8

#endif  // CCHIP8_MEMORY_H_
