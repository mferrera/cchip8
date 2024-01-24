#ifndef CCHIP8_CPU_H_
#define CCHIP8_CPU_H_

#include <cchip8/display.h>
#include <cchip8/input.h>
#include <cchip8/instruction.h>
#include <cchip8/memory.h>

#include <array>

#define NUM_REGISTERS 16

namespace cchip8 {

enum Registers {
  V0,
  V1,
  V2,
  V3,
  V4,
  V5,
  V6,
  V7,
  V8,
  V9,
  VA,
  VB,
  VC,
  VD,
  VE,
  VF,
};

class Cpu {
 public:
  std::array<uint8_t, NUM_REGISTERS> registers{};

  /* This register is generally used to store memory addresses,
   * so only the lowest (rightmost) 12 bits are usually used
   */
  uint16_t I{0};
  /* Program counter */
  uint16_t pc{0};
  /* Stack pointer */
  uint8_t sp{0};
  /* Delay timer register */
  uint8_t t_delay{0};
  /* Sound timer register */
  uint8_t t_sound{0};

  void Reset();
  uint8_t RandomByte();

  void CLS(Memory &memory);
  void RET(Memory &memory);
  void SYS();
  void JP(const Instruction &instruction);
  void CALL(const Instruction &instruction, Memory &memory);
  void SE_VX_KK(const Instruction &instruction);
  void SNE_VX_KK(const Instruction &instruction);
  void SE_VX_VY(const Instruction &instruction);
  void LD_VX_KK(const Instruction &instruction);
  void ADD_VX_KK(const Instruction &instruction);
  void LD_VX_VY(const Instruction &instruction);
  void OR_VX_VY(const Instruction &instruction);
  void AND_VX_VY(const Instruction &instruction);
  void XOR_VX_VY(const Instruction &instruction);
  void ADD_VX_VY(const Instruction &instruction);
  void SUB_VX_VY(const Instruction &instruction);
  void SHR_VX(const Instruction &instruction);
  void SUBN_VX_VY(const Instruction &instruction);
  void SHL_VX(const Instruction &instruction);
  void SNE_VX_VY(const Instruction &instruction);
  void LD_I(const Instruction &instruction);
  void JP_V0(const Instruction &instruction);
  void RND_VX_KK(const Instruction &instruction);
  void DRW_VX_VY(const Instruction &instruction, Memory &memory);
  void SKP_VX(const Instruction &instruction, Input &input);
  void SKNP_VX(const Instruction &instruction, Input &input);
  void LD_VX_DT(const Instruction &instruction);
  void LD_VX_K(const Instruction &instruction, Input &input);
  void LD_DT_VX(const Instruction &instruction);
  void LD_ST_VX(const Instruction &instruction);
  void ADD_I_VX(const Instruction &instruction);
  void LD_F_VX(const Instruction &instruction);
  void LD_B_VX(const Instruction &instruction, Memory &memory);
  void LD_I_VX(const Instruction &instruction, Memory &memory);
  void LD_VX_I(const Instruction &instruction, Memory &memory);
  void UNKNOWN(const Instruction &instruction);
};

}  // namespace cchip8

#endif  // CCHIP8_CPU_H_
