#ifndef _c64instructions_h_
#define _c64instructions_h_

#include <stdint.h>

#define LDI (uint16_t)0x0001 // LDI r, imm (r = imm)
#define LDM (uint16_t)0x0002 // LDM r, addr (r = addr)
#define ST (uint16_t)0x0003 // ST r, addr (r -> addr)
#define TF (uint16_t)0x0004 // TF r1, r2 (r1 = r2)

#define ADDI (uint16_t)0x0011 // ADDI r, imm (r += imm) ( C/Z/N/V )
#define SUBI (uint16_t)0x0012 // SUBI r, imm (r -= imm) ( C/Z/N/V )
#define MULI (uint16_t)0x0013 // MULI r, imm (r *= imm) ( unsigned ) ( C/Z/N/V )
#define DIVI (uint16_t)0x0014 // DIVI r, imm (r /= imm) ( unsigned ) ( C/Z/N/V )
#define MODI (uint16_t)0x0015 // MODI r, imm (r %= imm) ( unsigned ) ( C/Z/N/V )
#define MULIS (uint16_t)0x0016 // MULIS r, imm (r *= imm) ( signed ) ( C/Z/N/V )
#define DIVIS (uint16_t)0x0017 // DIVIS r, imm (r /= imm) ( signed ) ( C/Z/N/V )

#define ADD (uint16_t)0x0021 // ADD r1, r2 (r1 += r2) ( C/Z/N )
#define SUB (uint16_t)0x0022 // SUB r1, r2 (r1 -= r2) ( C/Z/N )
#define MUL (uint16_t)0x0023 // MUL r1, r2 (r1 *= r2) ( unsigned ) ( C/Z/N/V )
#define DIV (uint16_t)0x0024 // DIV r1, r2 (r1 /= r2) ( unsigned ) ( C/Z/N/V )
#define MOD (uint16_t)0x0025 // MOD r1, r2 (r1 %= r2) ( unsigned ) ( C/Z/N/V )
#define MULS (uint16_t)0x0026 // MULS r1, r2 (r1 *= r2) ( signed ) ( C/Z/N/V )
#define DIVS (uint16_t)0x0027 // DIVS r1, r2 (r1 /= r2) ( signed ) ( C/Z/N/V )

#define ANDI (uint16_t)0x0031 // ANDI r, imm (r &= imm) ( C/Z/N )
#define ORI (uint16_t)0x0032 // ORI r, imm (r |= imm) ( C/Z/N )
#define XORI (uint16_t)0x0033 // XORI r, imm (r ^= imm) ( C/Z/N )
#define NOTI (uint16_t)0x0034 // NOTI r, imm (r = ~imm) ( C/Z/N )
#define SHLI (uint16_t)0x0035 // SHLI r, imm (r <<= imm) ( C/Z/N )
#define SHRI (uint16_t)0x0036 // SHRI r, imm (r >>= imm) ( C/Z/N )
#define RORI (uint16_t)0x0037 // RORI r, imm (r = ror(r, imm)) ( C/Z/N )

#define AND (uint16_t)0x0041 // AND r1, r2 (r1 &= r2) ( C/Z/N )
#define OR (uint16_t)0x0042 // OR r1, r2 (r1 |= r2) ( C/Z/N )
#define XOR (uint16_t)0x0043 // XOR r1, r2 (r1 ^= r2) ( C/Z/N )
#define NOT (uint16_t)0x0044 // NOT r1, r2 (r1 = ~r2) ( C/Z/N )
#define SHL (uint16_t)0x0045 // SHL r1, r2 (r1 <<= r2) ( C/Z/N )
#define SHR (uint16_t)0x0046 // SHR r1, r2 (r1 >>= r2) ( C/Z/N )

#define CMP (uint16_t)0x0051 // CMP r1, r2 ( C/Z/N )
#define CMPI (uint16_t)0x0052 // CMPI r, imm ( C/Z/N )

#define JMP (uint16_t)0x0061 // JMP addr ( PC = addr )
#define JEQ (uint16_t)0x0062 // JEQ addr ( PC = addr ) ( Z )
#define JNE (uint16_t)0x0063 // JNE addr ( PC = addr ) ( !Z )
#define JGT (uint16_t)0x0064 // JGT addr ( PC = addr ) ( !Z && (N == V) )
#define JLT (uint16_t)0x0065 // JLT addr ( PC = addr ) ( N != V )
#define JGE (uint16_t)0x0066 // JGE addr ( PC = addr ) ( N == V )
#define JLE (uint16_t)0x0067 // JLE addr ( PC = addr ) ( Z || (N != V) )

#define BRA (uint16_t)0x0071 // BRA addr ( PC = addr ) ( always ) ( PC => SP-- )
#define BEQ (uint16_t)0x0072 // BEQ addr ( PC = addr ) ( Z ) ( PC => SP-- )
#define BNE (uint16_t)0x0073 // BNE addr ( PC = addr ) ( !Z ) ( PC => SP-- )
#define BGT (uint16_t)0x0074 // BGT addr ( PC = addr ) ( !Z && (N == V) ) ( PC => SP-- )
#define BLT (uint16_t)0x0075 // BLT addr ( PC = addr ) ( N != V ) ( PC => SP-- )
#define BGE (uint16_t)0x0076 // BGE addr ( PC = addr ) ( N == V ) ( PC => SP-- )
#define BLE (uint16_t)0x0077 // BLE addr ( PC = addr ) ( Z || (N != V) ) ( PC => SP-- )

#define RET (uint16_t)0x0081 // RET ( PC = SP++ )

#define PUSH (uint16_t)0x0091 // PUSH r ( SP-- => *SP = r )
#define POP (uint16_t)0x0092 // POP r ( *SP => r => SP++ )

#define CALL (uint16_t)0x00A1 // CALL addr ( PC => SP-- => *SP = PC => PC = addr )
#define CALLR (uint16_t)0x00A2 // CALLR r ( PC => SP-- => *SP = PC => PC = r )

#define CLC (uint16_t)0x00B1 // CLC ( C = 0 )
#define SEC (uint16_t)0x00B2 // SEC ( C = 1 )
#define CLZ (uint16_t)0x00B3 // CLZ ( Z = 0 )
#define SEZ (uint16_t)0x00B4 // SEZ ( Z = 1 )
#define CLN (uint16_t)0x00B5 // CLN ( N = 0 )
#define SEN (uint16_t)0x00B6 // SEN ( N = 1 )
#define CLV (uint16_t)0x00B7 // CLV ( V = 0 )
#define SEV (uint16_t)0x00B8 // SEV ( V = 1 )

#define NOP (uint16_t)0x0000 // NOP ( no operation )
#define HLT (uint16_t)0xFFFF // HLT ( halt )

#endif // _c64instructions_h_