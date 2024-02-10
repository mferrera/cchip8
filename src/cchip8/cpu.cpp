#include <cchip8/cpu.h>
#include <cchip8/display.h>
#include <cchip8/input.h>
#include <cchip8/instruction.h>
#include <cchip8/memory.h>

#include <iomanip>
#include <iostream>
#include <random>

namespace cchip8 {

void Cpu::Reset() {
  registers.fill(0);
  I = 0;
  pc = 0;
  sp = 0;
  t_delay = 0;
  t_sound = 0;
}

uint8_t Cpu::RandomByte() const {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> distribution(0, 0xFF);
  return distribution(rng);
}

/*
 * Descriptions from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

/* 0nnn - SYS addr
 * Jump to a machine code routine at nnn.
 *
 * This instruction is only used on the old computers on which Chip-8 was
 * originally implemented. It is ignored by modern interpreters.
 */
void Cpu::SYS(){};

/* 00E0 - CLS
 * Clear the display.
 */
void Cpu::CLS(Memory &memory) { memory.ClearVram(); };

/* 00EE - RET
 * Return from a subroutine.
 * The interpreter sets the program counter to the address at the top of the
 * stack, then subtracts 1 from the stack pointer.
 */
void Cpu::RET(Memory &memory) { pc = memory.stack.at(sp--); };

/*
 * 1nnn - JP addr
 * Jump to location nnn.
 *
 * The interpreter sets the program counter to nnn.
 */
void Cpu::JP(const Instruction &instruction) { pc = instruction.addr(); };

/* 2nnn - CALL addr
 * Call subroutine at nnn.
 *
 * The interpreter increments the stack pointer, then puts the current PC on
 * the top of the stack. The PC is then set to nnn.
 */
void Cpu::CALL(const Instruction &instruction, Memory &memory) {
  memory.stack.at(++sp) = pc;
  pc = instruction.addr();
};

/* 3xkk - SE Vx, byte
 * Skip next instruction if Vx = kk.
 *
 * The interpreter compares register Vx to kk, and if they are equal,
 * increments the program counter by 2.
 */
void Cpu::SE_VX_KK(const Instruction &instruction) {
  if (registers.at(instruction.x()) == instruction.kk()) {
    pc += 2;
  }
};

/* 4xkk - SNE Vx, byte
 * Skip next instruction if Vx != kk.
 *
 * The interpreter compares register Vx to kk, and if they are not equal,
 * increments the program counter by 2.
 */
void Cpu::SNE_VX_KK(const Instruction &instruction) {
  if (registers.at(instruction.x()) != instruction.kk()) {
    pc += 2;
  }
};

/* 5xy0 - SE Vx, Vy
 * Skip next instruction if Vx = Vy.
 *
 * The interpreter compares register Vx to register Vy, and if they are equal,
 * increments the program counter by 2.
 */
void Cpu::SE_VX_VY(const Instruction &instruction) {
  if (registers.at(instruction.x()) == registers.at(instruction.y())) {
    pc += 2;
  }
};

/* 6xkk - LD Vx, byte
 * Set Vx = kk.
 *
 * The interpreter puts the value kk into register Vx.
 */
void Cpu::LD_VX_KK(const Instruction &instruction) {
  registers.at(instruction.x()) = instruction.kk();
};

/* 7xkk - ADD Vx, byte
 * Set Vx = Vx + kk.
 *
 * Adds the value kk to the value of register Vx, then stores the result in Vx.
 */
void Cpu::ADD_VX_KK(const Instruction &instruction) {
  registers.at(instruction.x()) += instruction.kk();
};

/* 8xy0 - LD Vx, Vy
 * Set Vx = Vy.
 *
 * Stores the value of register Vy in register Vx.
 */
void Cpu::LD_VX_VY(const Instruction &instruction) {
  registers.at(instruction.x()) = registers.at(instruction.y());
};

/* 8xy1 - OR Vx, Vy
 * Set Vx = Vx OR Vy.
 *
 * Performs a bitwise OR on the values of Vx and Vy, then stores the result in
 * Vx.
 */
void Cpu::OR_VX_VY(const Instruction &instruction) {
  registers.at(instruction.x()) |= registers.at(instruction.y());
};

/* 8xy2 - AND Vx, Vy
 * Set Vx = Vx AND Vy.
 *
 * Performs a bitwise AND on the values of Vx and Vy, then stores the result
 * in Vx.
 */
void Cpu::AND_VX_VY(const Instruction &instruction) {
  registers.at(instruction.x()) &= registers.at(instruction.y());
};

/* 8xy3 - XOR Vx, Vy
 * Set Vx = Vx XOR Vy.
 *
 * Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the
 * result in Vx.
 */
void Cpu::XOR_VX_VY(const Instruction &instruction) {
  registers.at(instruction.x()) ^= registers.at(instruction.y());
};

/* 8xy4 - ADD Vx, Vy
 * Set Vx = Vx + Vy, set VF = carry.
 *
 * The values of Vx and Vy are added together. If the result is greater than
 * 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits
 * of the result are kept, and stored in Vx.
 */
void Cpu::ADD_VX_VY(const Instruction &instruction) {
  uint16_t sum = registers.at(instruction.x()) + registers.at(instruction.y());
  registers.at(instruction.x()) = sum & 0xFF;
  registers.at(Registers::VF) = (sum > 0xFF) ? 1 : 0;
};

/* 8xy5 - SUB Vx, Vy
 * Set Vx = Vx - Vy, set VF = NOT borrow.
 *
 * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from
 * Vx, and the results stored in Vx.
 */
void Cpu::SUB_VX_VY(const Instruction &instruction) {
  registers.at(Registers::VF) =
      (registers.at(instruction.x()) >= registers.at(instruction.y())) ? 1 : 0;
  registers.at(instruction.x()) -= registers.at(instruction.y());
};

/* 8xy6 - SHR Vx {, Vy}
 * Set Vx = Vx SHR 1.
 *
 * If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
 * Then Vx is divided by 2.
 */
void Cpu::SHR_VX(const Instruction &instruction) {
  registers.at(Registers::VF) = registers.at(instruction.x()) & 0x01;
  registers.at(instruction.x()) >>= 1;
};

/* 8xy7 - SUBN Vx, Vy
 * Set Vx = Vy - Vx, set VF = NOT borrow.
 *
 * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from
 * Vy, and the results stored in Vx.
 */
void Cpu::SUBN_VX_VY(const Instruction &instruction) {
  registers.at(Registers::VF) =
      (registers.at(instruction.y()) >= registers.at(instruction.x())) ? 1 : 0;
  registers.at(instruction.x()) -= registers.at(instruction.y());
};

/* 8xyE - SHL Vx {, Vy}
 * Set Vx = Vx SHL 1.
 *
 * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
 * Then Vx is multiplied by 2.
 */
void Cpu::SHL_VX(const Instruction &instruction) {
  registers.at(Registers::VF) = registers.at(instruction.x()) >> 7;
  registers.at(instruction.x()) <<= 1;
};

/* 9xy0 - SNE Vx, Vy
 * Skip next instruction if Vx != Vy.
 *
 * The values of Vx and Vy are compared, and if they are not equal, the program
 * counter is increased by 2.
 */
void Cpu::SNE_VX_VY(const Instruction &instruction) {
  if (registers.at(instruction.x()) != registers.at(instruction.y())) {
    pc += 2;
  }
};

/* Annn - LD I, addr
 * Set I = nnn.
 *
 * The value of register I is set to nnn.
 */
void Cpu::LD_I(const Instruction &instruction) { I = instruction.addr(); };

/* Bnnn - JP V0, addr
 * Jump to location nnn + V0.
 *
 * The program counter is set to nnn plus the value of V0.
 */
void Cpu::JP_V0(const Instruction &instruction) {
  pc = instruction.addr() + registers.at(Registers::V0);
};

/* Cxkk - RND Vx, byte
 * Set Vx = random byte AND kk.
 *
 * The interpreter generates a random number from 0 to 255, which is then ANDed
 * with the value kk. The results are stored in Vx.
 */
void Cpu::RND_VX_KK(const Instruction &instruction) {
  registers.at(instruction.x()) = RandomByte() & instruction.kk();
};

/* Dxyn - DRW Vx, Vy, nibble
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set
 * VF = collision.
 *
 * The interpreter reads n bytes from memory, starting at the address stored in
 * I. These bytes are then displayed as sprites on screen at coordinates
 * (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any
 * pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite
 * is positioned so part of it is outside the coordinates of the display, it
 * wraps around to the opposite side of the screen.
 */
void Cpu::DRW_VX_VY(const Instruction &instruction, Memory &memory) {
  registers.at(Registers::VF) = 0;
  for (auto y = 0; y < instruction.n(); ++y) {
    uint8_t byte = memory.ram.at(I + y);
    for (auto x = 0; x < 8; ++x) {
      if (byte & (0x80 >> x)) {
        uint16_t px = (registers.at(instruction.x()) + x) % DISPLAY_WIDTH;
        uint16_t py = (registers.at(instruction.y()) + y) % DISPLAY_HEIGHT;
        uint16_t xy = (px + (py * DISPLAY_WIDTH)) % DISPLAY_SIZE;
        registers.at(Registers::VF) = memory.vram.at(xy);
        memory.vram.at(xy) ^= 1;
      }
    }
  }
};

/* Ex9E - SKP Vx
 * Skip next instruction if key with the value of Vx is pressed.
 *
 * Checks the keyboard, and if the key corresponding to the value of Vx is
 * currently in the down position, PC is increased by 2.
 */
void Cpu::SKP_VX(const Instruction &instruction, Input &input) {
  if (input.IsDown(registers.at(instruction.x()))) {
    pc += 2;
  }
};

/* ExA1 - SKNP Vx
 * Skip next instruction if key with the value of Vx is not pressed.
 *
 * Checks the keyboard, and if the key corresponding to the value of Vx is
 * currently in the up position, PC is increased by 2.
 */
void Cpu::SKNP_VX(const Instruction &instruction, Input &input) {
  if (!input.IsDown(registers.at(instruction.x()))) {
    pc += 2;
  }
};

/* Fx07 - LD Vx, DT
 * Set Vx = delay timer value.
 *
 * The value of DT is placed into Vx.
 */
void Cpu::LD_VX_DT(const Instruction &instruction) {
  registers.at(instruction.x()) = t_delay;
};

/* Fx0A - LD Vx, K
 * Wait for a key press, store the value of the key in Vx.
 *
 * All execution stops until a key is pressed, then the value of that key is
 * stored in Vx.
 */
void Cpu::LD_VX_K(const Instruction &instruction, Input &input) {
  for (auto i = 0; i < 0x0F; ++i) {
    if (input.IsDown(i)) {
      registers.at(instruction.x()) = i;
    }
  }
};

/* Fx15 - LD DT, Vx
 * Set delay timer = Vx.
 *
 * DT is set equal to the value of Vx.
 */
void Cpu::LD_DT_VX(const Instruction &instruction) {
  t_delay = registers.at(instruction.x());
};

/* Fx18 - LD ST, Vx
 * Set sound timer = Vx.
 *
 * ST is set equal to the value of Vx.
 */
void Cpu::LD_ST_VX(const Instruction &instruction) {
  t_sound = registers.at(instruction.x());
};

/* Fx1E - ADD I, Vx
 * Set I = I + Vx.
 *
 * The values of I and Vx are added, and the results are stored in I.
 */
void Cpu::ADD_I_VX(const Instruction &instruction) {
  I += registers.at(instruction.x());
};

/* Fx29 - LD F, Vx
 * Set I = location of sprite for digit Vx.
 *
 * The value of I is set to the location for the hexadecimal sprite
 * corresponding to the value of Vx.
 */
void Cpu::LD_F_VX(const Instruction &instruction) {
  I = SPRITES_LOCATION + (registers.at(instruction.x()) * SPRITE_SIZE);
};

/* Fx33 - LD B, Vx
 * Store BCD representation of Vx in memory locations I, I+1, and I+2.
 *
 * The interpreter takes the decimal value of Vx, and places the hundreds digit
 * in memory at location in I, the tens digit at location I+1, and the ones
 * digit at location I+2.
 */
void Cpu::LD_B_VX(const Instruction &instruction, Memory &memory) {
  memory.ram.at(I) = registers.at(instruction.x()) / 100;
  memory.ram.at(I + 1) = (registers.at(instruction.x()) / 10) % 10;
  memory.ram.at(I + 2) = registers.at(instruction.x()) % 10;
};

/* Fx55 - LD [I], Vx
 * Store registers V0 through Vx in memory starting at location I.
 *
 * The interpreter copies the values of registers V0 through Vx into memory,
 * starting at the address in I.
 */
void Cpu::LD_I_VX(const Instruction &instruction, Memory &memory) {
  auto addr = I;
  for (auto i = 0; i <= instruction.x(); ++i) {
    memory.ram.at(addr + i) = registers.at(i);
  }
};

/* Fx65 - LD Vx, [I]
 * Read registers V0 through Vx from memory starting at location I.
 *
 * The interpreter reads values from memory starting at location I into
 * registers V0 through Vx.
 */
void Cpu::LD_VX_I(const Instruction &instruction, Memory &memory) {
  auto addr = I;
  for (auto i = 0; i <= instruction.x(); ++i) {
    registers.at(i) = memory.ram.at(addr + i);
  }
};

void Cpu::UNKNOWN(const Instruction &instruction) {
  std::cerr << "Unknown instruction " << std::hex << std::setw(4)
            << instruction.instruction() << std::endl;
};

};  // namespace cchip8
