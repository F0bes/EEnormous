#pragma once
#include "Block.h"
#include "Opcodes.h"

namespace EEmitter
{
	namespace state
	{
		extern Block* block;
		extern Block* lastBlockExecuted;
	} // namespace state
	void SetBlock(Block* block);
	void GetBlock(Block* block);

	using namespace Registers;


	// Psuedo instructions

	void xNOP();
	void xLI(const xRegisterGPR& rt, u32 imm);

	void xADD(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xADD(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xADDI(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm);
	void xADDIU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm);
	void xADDU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xADDU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xAND(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xAND(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xANDI(const xRegisterGPR& rt, const xRegisterGPR& rs, const u16& imm);

	void xBEQ(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset);
	void xBEQL(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset);
	void xBGEZ(const xRegisterGPR& rs, s16 offset);
	void xBGEZAL(const xRegisterGPR& rs, s16 offset);
	void xBGEZALL(const xRegisterGPR& rs, s16 offset);
	void xBGEZL(const xRegisterGPR& rs, s16 offset);
	void xBGTZ(const xRegisterGPR& rs, s16 offset);
	void xBGTZL(const xRegisterGPR& rs, s16 offset);
	void xBLEZ(const xRegisterGPR& rs, s16 offset);
	void xBLEZL(const xRegisterGPR& rs, s16 offset);
	void xBLTZ(const xRegisterGPR& rs, s16 offset);
	void xBLTZAL(const xRegisterGPR& rs, s16 offset);
	void xBLTZALL(const xRegisterGPR& rs, s16 offset);
	void xBLTZL(const xRegisterGPR& rs, s16 offset);
	void xBNE(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset);
	void xBNEL(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset);

	void xBREAK(const u32& code);

	void xDADD(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDADD(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDADDI(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm); // OwO
	void xDADDIU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm);
	void xDADDU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDADDU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xDIV(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDIVU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xDSLL(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xDSLL(const xRegisterGPR& rt, const u8& sa);
	void xDSLL32(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xDSLL32(const xRegisterGPR& rt, const u8& sa);
	void xDSLLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xDSLLV(const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xDSRA(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xDSRA(const xRegisterGPR& rt, const u8& sa);
	void xDSRA32(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xDSRA32(const xRegisterGPR& rt, const u8& sa);
	void xDSRAV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xDSRAV(const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xDSRL(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xDSRL(const xRegisterGPR& rt, const u8& sa);
	void xDSRL32(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xDSRL32(const xRegisterGPR& rt, const u8& sa);
	void xDSRLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xDSRLV(const xRegisterGPR& rt, const xRegisterGPR& rs);

	void xDSUB(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDSUB(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDSUBU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xDSUBU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xJ(u32 target);
	void xJAL(u32 target);
	void xJALR(const xRegisterGPR& rs);
	void xJALR(const xRegisterGPR& rd, const xRegisterGPR& rs);
	void xJR(const xRegisterGPR& rs);

	void xLB(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLBU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLD(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLDL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLDR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLH(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLHU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLUI(const xRegisterGPR& rt, const u16& imm);
	void xLW(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLWL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLWR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xLWU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);

	void xMFHI(const xRegisterGPR& rd);
	void xMFLO(const xRegisterGPR& rd);

	// todo psuedo-mov (ori $rd,$rs,0)
	void xMOVN(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xMOVN(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xMOVZ(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xMOVZ(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xMTHI(const xRegisterGPR& rs);
	void xMTLO(const xRegisterGPR& rs);

	void xMULT(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xMULTU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xNOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xNOR(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xOR(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xORI(const xRegisterGPR& rt, const xRegisterGPR& rs, const u16& imm);

	void xPREF(const u8& hint, const xRegisterGPR& rs, s16 offset);

	void xSB(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSD(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSDL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSDR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSH(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSLL(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xSLL(const xRegisterGPR& rt, const u8& sa);
	void xSLLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xSLLV(const xRegisterGPR& rt, const xRegisterGPR& rs);

	void xSLT(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xSLT(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xSLTI(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm);
	void xSLTIU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm);
	void xSLTU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xSLTU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xSRA(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xSRA(const xRegisterGPR& rt, const u8& sa);
	void xSRAV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xSRAV(const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xSRL(const xRegisterGPR& rd, const xRegisterGPR& rt, const u8& sa);
	void xSRL(const xRegisterGPR& rt, const u8& sa);
	void xSRLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs);
	void xSRLV(const xRegisterGPR& rt, const xRegisterGPR& rs);

	void xSUB(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xSUB(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xSUBU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xSUBU(const xRegisterGPR& rs, const xRegisterGPR& rt);

	void xSW(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSWL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);
	void xSWR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset);

	void xSYNC(const u8& stype);
	void xSYNCL();
	void xSYNCP();

	void xSYSCALL();
	void xSYSCALL(const u32& code);

	void xTEQ(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xTEQ(const xRegisterGPR& rs, const xRegisterGPR& rt, const u32& code);
	void xTEQI(const xRegisterGPR& rs, s16 imm);
	void xTGE(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xTGE(const xRegisterGPR& rs, const xRegisterGPR& rt, const u32& code);
	void xTGEI(const xRegisterGPR& rs, s16 imm);
	void xTGEIU(const xRegisterGPR& rs, s16 imm);
	void xTGEU(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xTGEU(const xRegisterGPR& rs, const xRegisterGPR& rt, const u32& code);
	void xTLT(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xTLT(const xRegisterGPR& rs, const xRegisterGPR& rt, const u32& code);
	void xTLTI(const xRegisterGPR& rs, s16 imm);
	void xTLTIU(const xRegisterGPR& rs, s16 imm);
	void xTLTU(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xTLTU(const xRegisterGPR& rs, const xRegisterGPR& rt, const u32& code);
	void xTNE(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xTNE(const xRegisterGPR& rs, const xRegisterGPR& rt, const u32& code);
	void xTNEI(const xRegisterGPR& rs, s16 imm);

	void xXOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xXOR(const xRegisterGPR& rs, const xRegisterGPR& rt);
	void xXORI(const xRegisterGPR& rt, const xRegisterGPR& rs, const u16& imm);
} // namespace EEmitter
