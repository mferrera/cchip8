#ifndef CCHIP8_INSTRUCTION_H_
#define CCHIP8_INSTRUCTION_H_

#include <cstdint>

namespace cchip8 {

enum class Opcode {
  CLS,
  RET,
  SYS,
  JP,
  CALL,
  SE_VX_KK,
  SNE_VX_KK,
  SE_VX_VY,
  LD_VX_KK,
  ADD_VX_KK,
  LD_VX_VY,
  OR_VX_VY,
  AND_VX_VY,
  XOR_VX_VY,
  ADD_VX_VY,
  SUB_VX_VY,
  SHR_VX,
  SUBN_VX_VY,
  SHL_VX,
  SNE_VX_VY,
  LD_I,
  JP_V0,
  RND_VX_KK,
  DRW_VX_VY,
  SKP_VX,
  SKNP_VX,
  LD_VX_DT,
  LD_VX_K,
  LD_DT_VX,
  LD_ST_VX,
  ADD_I_VX,
  LD_F_VX,
  LD_B_VX,
  LD_I_VX,
  LD_VX_I,
  UNKNOWN,
};

class Instruction {
 public:
  Instruction(const uint16_t instruction) : m_instruction(instruction) {}

  /* A 12-bit value, the lowest 12 bits of the instruction */
  uint16_t addr() const { return m_instruction & 0x0FFF; }
  /* A 4-bit value, the lowest 4 bits of the instruction */
  uint8_t n() const { return m_instruction & 0x000F; }
  /* The most significant nibble */
  uint8_t msn() const { return m_instruction >> 12; }
  /* A 4-bit value, the lower 4 bits of the high byte of the instruction */
  uint8_t x() const { return (m_instruction & 0x0F00) >> 8; }
  /* A 4-bit value, the upper 4 bits of the low byte of the instruction */
  uint8_t y() const { return (m_instruction & 0x00F0) >> 4; }
  /* An 8-bit value, the lowest 8 bits of the instruction */
  uint8_t kk() const { return m_instruction & 0x00FF; }
  /* The full instruction as bytes */
  uint16_t instruction() const { return m_instruction; }

  Opcode Decode() const {
    switch (msn()) {
      case 0x00:
        switch (addr()) {
          case 0x0E0:
            return Opcode::CLS;
          case 0x0EE:
            return Opcode::RET;
          default:
            return Opcode::SYS;
        }
      case 0x01:
        return Opcode::JP;
      case 0x02:
        return Opcode::CALL;
      case 0x03:
        return Opcode::SE_VX_KK;
      case 0x04:
        return Opcode::SNE_VX_KK;
      case 0x05:
        return Opcode::SE_VX_VY;
      case 0x06:
        return Opcode::LD_VX_KK;
      case 0x07:
        return Opcode::ADD_VX_KK;
      case 0x08:
        switch (n()) {
          case 0x00:
            return Opcode::LD_VX_VY;
          case 0x01:
            return Opcode::OR_VX_VY;
          case 0x02:
            return Opcode::AND_VX_VY;
          case 0x03:
            return Opcode::XOR_VX_VY;
          case 0x04:
            return Opcode::ADD_VX_VY;
          case 0x05:
            return Opcode::SUB_VX_VY;
          case 0x06:
            return Opcode::SHR_VX;
          case 0x07:
            return Opcode::SUBN_VX_VY;
          case 0x0E:
            return Opcode::SHL_VX;
          default:
            return Opcode::UNKNOWN;
        }
      case 0x09:
        return Opcode::SNE_VX_VY;
      case 0x0A:
        return Opcode::LD_I;
      case 0x0B:
        return Opcode::JP_V0;
      case 0x0C:
        return Opcode::RND_VX_KK;
      case 0x0D:
        return Opcode::DRW_VX_VY;
      case 0x0E:
        switch (kk()) {
          case 0x9E:
            return Opcode::SKP_VX;
          case 0xA1:
            return Opcode::SKNP_VX;
          default:
            return Opcode::UNKNOWN;
        }
      case 0x0F:
        switch (kk()) {
          case 0x07:
            return Opcode::LD_VX_DT;
          case 0x0A:
            return Opcode::LD_VX_K;
          case 0x15:
            return Opcode::LD_DT_VX;
          case 0x18:
            return Opcode::LD_ST_VX;
          case 0x1E:
            return Opcode::ADD_I_VX;
          case 0x29:
            return Opcode::LD_F_VX;
          case 0x33:
            return Opcode::LD_B_VX;
          case 0x55:
            return Opcode::LD_I_VX;
          case 0x65:
            return Opcode::LD_VX_I;
          default:
            return Opcode::UNKNOWN;
        }
      default:
        return Opcode::UNKNOWN;
    }
  }

 private:
  const uint16_t m_instruction;
};

}  // namespace cchip8

#endif  // CCHIP8_INSTRUCTION_H_
