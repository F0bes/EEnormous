#include "Emitter.h"

namespace EEmitter
{
	using namespace state;

	Block* state::block = nullptr;
	Block* state::lastBlockExecuted = nullptr;

	void SetBlock(Block* block)
	{
		state::block = block;
	}

	Block* GetBlock()
	{
		return state::block;
	}

	// Could this be done in a better way with some RAII?

	const xRegisterGPR& xAllocReg() { return state::block->AllocateRegister(); }
	bool xTryAllocReg(const xRegisterGPR& reg) { return state::block->TryAllocateRegister(reg); }
	void xFreeReg(const xRegisterGPR& reg) { state::block->FreeRegister(reg); }
	bool xIsRegFree(const xRegisterGPR& reg) { return state::block->IsRegisterFree(reg); }
	void xFreeAllRegisters() { state::block->FreeAllRegisters(); }

	// Pseudo instructions

	void xNOP()
	{
		block->Emit(0x00000000);
	}

	void xLI(const xRegisterGPR& rt, u32 imm)
	{
		xLUI(rt, imm >> 16);
		xORI(rt, rt, imm & 0xFFFF);
	}

	void xADD(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::ADD));
	}

	void xADD(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xADD(rs, rs, rt);
	}

	void xADDI(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::ADDI, rs.id, rt.id, imm));
	}

	void xADDIU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::ADDIU, rs.id, rt.id, imm));
	}

	void xADDU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::ADDU));
	}

	void xADDU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xADDU(rs, rs, rt);
	}

	void xAND(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::AND));
	}

	void xAND(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xAND(rs, rs, rt);
	}

	void xANDI(const xRegisterGPR& rt, const xRegisterGPR& rs, const u16& imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::ANDI, rs.id, rt.id, imm));
	}

	void xBEQ(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BEQ, rs.id, rt.id, offset));
	}

	void xBEQL(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BEQL, rs.id, rt.id, offset));
	}

	void xBGEZ(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BGEZ, offset));
	}

	void xBGEZAL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BGEZAL, offset));
	}

	void xBGEZALL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BGEZALL, offset));
	}

	void xBGEZL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BGEZL, offset));
	}

	void xBGTZ(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BGTZ, rs.id, 0x00, offset));
	}

	void xBGTZL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BGTZL, rs.id, 0x00, offset));
	}

	void xBLEZ(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BLEZ, rs.id, 0x00, offset));
	}

	void xBLEZL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BLEZL, rs.id, 0x00, offset));
	}

	void xBLTZ(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BLTZ, offset));
	}

	void xBLTZAL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BLTZAL, offset));
	}

	void xBLTZALL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BLTZALL, offset));
	}

	void xBLTZL(const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::BLTZL, offset));
	}

	void xBNE(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BNE, rs.id, rt.id, offset));
	}

	void xBNEL(const xRegisterGPR& rs, const xRegisterGPR& rt, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::BNEL, rs.id, rt.id, offset));
	}

	void xBREAK(u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE(code, Opcodes::CLASS_SPECIAL::BREAK));
	}

	void xDADD(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DADD));
	}

	void xDADD(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xDADD(rs, rs, rt);
	}

	void xDADDI(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::DADDI, rs.id, rt.id, imm));
	}

	void xDADDIU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::DADDIU, rs.id, rt.id, imm));
	}

	void xDADDU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DADDU));
	}

	void xDADDU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xDADDU(rs, rs, rt);
	}

	void xDIV(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, 0x00, 0x00, Opcodes::CLASS_SPECIAL::DIV));
	}

	void xDIVU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, 0x00, 0x00, Opcodes::CLASS_SPECIAL::DIVU));
	}

	void xDSLL(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::DSLL));
	}

	void xDSLL(const xRegisterGPR& rt, u8 sa)
	{
		xDSLL(rt, rt, sa);
	}

	void xDSLL32(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::DSLL32));
	}

	void xDSLL32(const xRegisterGPR& rt, u8 sa)
	{
		xDSLL32(rt, rt, sa);
	}

	void xDSLLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DSLLV));
	}

	void xDSLLV(const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		xDSLLV(rt, rt, rs);
	}

	void xDSRA(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::DSRA));
	}

	void xDSRA(const xRegisterGPR& rt, u8 sa)
	{
		xDSRA(rt, rt, sa);
	}

	void xDSRA32(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::DSRA32));
	}

	void xDSRA32(const xRegisterGPR& rt, u8 sa)
	{
		xDSRA32(rt, rt, sa);
	}

	void xDSRAV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DSRAV));
	}

	void xDSRAV(const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		xDSRAV(rt, rt, rs);
	}

	void xDSRL(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::DSRL));
	}

	void xDSRL(const xRegisterGPR& rt, u8 sa)
	{
		xDSRL(rt, rt, sa);
	}

	void xDSRL32(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::DSRL32));
	}

	void xDSRL32(const xRegisterGPR& rt, u8 sa)
	{
		xDSRL32(rt, rt, sa);
	}

	void xDSRLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DSRLV));
	}

	void xDSRLV(const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		xDSRLV(rt, rt, rs);
	}

	void xDSUB(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DSUB));
	}

	void xDSUB(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xDSUB(rs, rs, rt);
	}

	void xDSUBU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::DSUBU));
	}

	void xDSUBU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xDSUBU(rs, rs, rt);
	}

	void xJ(u32 target)
	{
		block->Emit(GEN_OP_OFFSET(Opcodes::J, target >> 2));
	}

	void xJAL(u32 target)
	{
		block->Emit(GEN_OP_OFFSET(Opcodes::JAL, target >> 2));
	}

	void xJALR(const xRegisterGPR& rd, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, 0x00, rd.id, 0x00, Opcodes::CLASS_SPECIAL::JALR));
	}

	void xJALR(const xRegisterGPR& rs)
	{
		xJALR(ra, rs);
	}

	void xJR(const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, 0x00, 0x00, 0x00, Opcodes::CLASS_SPECIAL::JR));
	}

	void xLB(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LB, rs.id, rt.id, offset));
	}

	void xLBU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LBU, rs.id, rt.id, offset));
	}

	void xLD(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LD, rs.id, rt.id, offset));
	}

	void xLDL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LDL, rs.id, rt.id, offset));
	}

	void xLDR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LDR, rs.id, rt.id, offset));
	}

	void xLH(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LH, rs.id, rt.id, offset));
	}

	void xLHU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LHU, rs.id, rt.id, offset));
	}

	void xLUI(const xRegisterGPR& rt, const u16& imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LUI, 0x00, rt.id, imm));
	}

	void xLW(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LW, rs.id, rt.id, offset));
	}

	void xLWL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LWL, rs.id, rt.id, offset));
	}

	void xLWR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LWR, rs.id, rt.id, offset));
	}

	void xLWU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::LWU, rs.id, rt.id, offset));
	}

	void xMFHI(const xRegisterGPR& rd)
	{
		block->Emit(GEN_SPECIAL(0x00, 0x00, rd.id, 0x00, Opcodes::CLASS_SPECIAL::MFHI));
	}

	void xMFLO(const xRegisterGPR& rd)
	{
		block->Emit(GEN_SPECIAL(0x00, 0x00, rd.id, 0x00, Opcodes::CLASS_SPECIAL::MFLO));
	}

	void xMOVN(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::MOVN));
	}

	void xMOVN(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xMOVN(rs, rs, rt);
	}

	void xMOVZ(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::MOVZ));
	}

	void xMOVZ(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xMOVZ(rs, rs, rt);
	}

	void xMTHI(const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, 0x00, 0x00, 0x00, Opcodes::CLASS_SPECIAL::MTHI));
	}

	void xMTLO(const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, 0x00, 0x00, 0x00, Opcodes::CLASS_SPECIAL::MTLO));
	}

	void xMULT(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, 0x00, 0x00, Opcodes::CLASS_SPECIAL::MULT));
	}

	void xMULTU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, 0x00, 0x00, Opcodes::CLASS_SPECIAL::MULTU));
	}

	void xNOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::NOR));
	}

	void xNOR(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xNOR(rs, rs, rt);
	}

	void xOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::OR));
	}

	void xOR(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xOR(rs, rs, rt);
	}

	void xORI(const xRegisterGPR& rt, const xRegisterGPR& rs, u16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::ORI, rs.id, rt.id, imm));
	}

	void xPREF(u8 hint, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::PREF, rs.id, hint, offset));
	}

	void xSB(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SB, rs.id, rt.id, offset));
	}

	void xSD(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SD, rs.id, rt.id, offset));
	}

	void xSDL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SDL, rs.id, rt.id, offset));
	}

	void xSDR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SDR, rs.id, rt.id, offset));
	}

	void xSH(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SH, rs.id, rt.id, offset));
	}

	void xSLL(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::SLL));
	}

	void xSLL(const xRegisterGPR& rt, u8 sa)
	{
		xSLL(rt, rt, sa);
	}

	void xSLLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SLLV));
	}

	void xSLLV(const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		xSLLV(rt, rt, rs);
	}

	void xSLT(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SLT));
	}

	void xSLT(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xSLT(rs, rs, rt);
	}

	void xSLTI(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SLTI, rs.id, rt.id, imm));
	}

	void xSLTIU(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SLTIU, rs.id, rt.id, imm));
	}

	void xSLTU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SLTU));
	}

	void xSLTU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xSLTU(rs, rs, rt);
	}

	void xSRA(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::SRA));
	}

	void xSRA(const xRegisterGPR& rt, u8 sa)
	{
		xSRA(rt, rt, sa);
	}

	void xSRAV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SRAV));
	}

	void xSRAV(const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		xSRAV(rt, rt, rs);
	}

	void xSRL(const xRegisterGPR& rd, const xRegisterGPR& rt, u8 sa)
	{
		block->Emit(GEN_SPECIAL(0x00, rt.id, rd.id, sa, Opcodes::CLASS_SPECIAL::SRL));
	}

	void xSRL(const xRegisterGPR& rt, u8 sa)
	{
		xSRL(rt, rt, sa);
	}

	void xSRLV(const xRegisterGPR& rd, const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SRLV));
	}

	void xSRLV(const xRegisterGPR& rt, const xRegisterGPR& rs)
	{
		xSRLV(rt, rt, rs);
	}

	void xSUB(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SUB));
	}

	void xSUB(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xSUB(rs, rs, rt);
	}

	void xSUBU(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x00, Opcodes::CLASS_SPECIAL::SUBU));
	}

	void xSUBU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xSUBU(rs, rs, rt);
	}

	void xSW(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SW, rs.id, rt.id, offset));
	}

	void xSWL(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SWL, rs.id, rt.id, offset));
	}

	void xSWR(const xRegisterGPR& rt, const xRegisterGPR& rs, s16 offset)
	{
		block->Emit(GEN_OP_IMM(Opcodes::SWR, rs.id, rt.id, offset));
	}

	void xSYNC(u8 stype)
	{
		block->Emit(GEN_SPECIAL(0x00, 0x00, 0x00, stype, Opcodes::CLASS_SPECIAL::SYNC));
	}

	void xSYNCL()
	{
		xSYNC(0x00);
	}

	void xSYNCP()
	{
		xSYNC(1 << 5);
	}

	void xSYSCALL(u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE(code, Opcodes::CLASS_SPECIAL::SYSCALL));
	}

	void xSYSCALL()
	{
		xSYSCALL(0x00);
	}

	void xTEQ(const xRegisterGPR& rs, const xRegisterGPR& rt, u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE_TRAP(rs.id, rt.id, code, Opcodes::CLASS_SPECIAL::TEQ));
	}

	void xTEQ(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xTEQ(rs, rt, 0x00);
	}

	void xTEQI(const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::TEQI, imm));
	}

	void xTGE(const xRegisterGPR& rs, const xRegisterGPR& rt, u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE_TRAP(rs.id, rt.id, code, Opcodes::CLASS_SPECIAL::TGE));
	}

	void xTGE(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xTGE(rs, rt, 0x00);
	}

	void xTGEI(const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::TGEI, imm));
	}

	void xTGEIU(const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::TGEIU, imm));
	}

	void xTGEU(const xRegisterGPR& rs, const xRegisterGPR& rt, u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE_TRAP(rs.id, rt.id, code, Opcodes::CLASS_SPECIAL::TGEU));
	}

	void xTGEU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xTGEU(rs, rt, 0x00);
	}

	void xTLT(const xRegisterGPR& rs, const xRegisterGPR& rt, u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE_TRAP(rs.id, rt.id, code, Opcodes::CLASS_SPECIAL::TLT));
	}

	void xTLT(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xTLT(rs, rt, 0x00);
	}

	void xTLTI(const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::TLTI, imm));
	}

	void xTLTIU(const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::TLTIU, imm));
	}

	void xTLTU(const xRegisterGPR& rs, const xRegisterGPR& rt, u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE_TRAP(rs.id, rt.id, code, Opcodes::CLASS_SPECIAL::TLTU));
	}

	void xTLTU(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xTLTU(rs, rt, 0x00);
	}

	void xTNE(const xRegisterGPR& rs, const xRegisterGPR& rt, u32 code)
	{
		block->Emit(GEN_SPECIAL_CODE_TRAP(rs.id, rt.id, code, Opcodes::CLASS_SPECIAL::TNE));
	}

	void xTNE(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xTNE(rs, rt, 0x00);
	}

	void xTNEI(const xRegisterGPR& rs, s16 imm)
	{
		block->Emit(GEN_REGIMM(rs.id, Opcodes::CLASS_REGIMM::TNEI, imm));
	}

	void xXOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_SPECIAL(rs.id, rt.id, rd.id, 0x26, Opcodes::CLASS_SPECIAL::XOR));
	}

	void xXOR(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xXOR(rs, rs, rt);
	}

	void xXORI(const xRegisterGPR& rt, const xRegisterGPR& rs, const u16& imm)
	{
		block->Emit(GEN_OP_IMM(Opcodes::XORI, rs.id, rt.id, imm));
	}

	// MMI

	void xPCPYLD(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI2(rs.id, rt.id, rd.id, Opcodes::PCPYLD));
	}

	void xPCPYLD(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPCPYLD(rs, rs, rt);
	}

	void xPEXTLW(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI0(rs.id, rt.id, rd.id, Opcodes::PEXTLW));
	}

	void xPEXTLW(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPEXTLW(rs, rs, rt);
	}

	void xPOR(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI3(rs.id, rt.id, rd.id, Opcodes::POR));
	}

	void xPOR(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPOR(rs, rs, rt);
	}

	void xPAND(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI2(rs.id, rt.id, rd.id, Opcodes::PAND));
	}

	void xPAND(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPAND(rs, rs, rt);
	}

	void xPCEQW(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI1(rs.id, rt.id, rd.id, Opcodes::PCEQW));
	}

	void xPCEQW(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPCEQW(rs, rs, rt);
	}

	void xPCGTW(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI0(rs.id, rt.id, rd.id, Opcodes::PCGTW));
	}

	void xPCGTW(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPCGTW(rs, rs, rt);
	}

	void xPPACB(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI0(rs.id, rt.id, rd.id, Opcodes::PPACB));
	}

	void xPPACB(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPPACB(rs, rs, rt);
	}

	void xPPACH(const xRegisterGPR& rd, const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI0(rs.id, rt.id, rd.id, Opcodes::PPACH));
	}

	void xPPACH(const xRegisterGPR& rs, const xRegisterGPR& rt)
	{
		xPPACH(rs, rs, rt);
	}

	void xPREVH(const xRegisterGPR& rd, const xRegisterGPR& rt)
	{
		block->Emit(GEN_MMI2(0, rt.id, rd.id, Opcodes::PREVH));
	}

} // namespace EEmitter
