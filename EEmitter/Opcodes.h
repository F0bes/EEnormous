#pragma once
#include <tamtypes.h>
#include <string_view>

namespace EEmitter
{
	namespace Opcodes
	{
		enum OpCode
		{
			SPECIAL, REGIMM, J,      JAL,      BEQ,   BNE,     BLEZ,  BGTZ,
			ADDI,    ADDIU,  SLTI,   SLTIU,    ANDI,  ORI,     XORI,  LUI,
			COP0,    COP1,   COP2,   _rsvoc13, BEQL,   BNEL,    BLEZL, BGTZL,
			DADDI,   DADDIU, LDL,    LDR,     MMI,    _rsvoc1d, LQ,    SQ,
			LB,      LH,     LWL,    LW,      LBU,    LHU,     LWR,   LWU,
			SB,      SH,     SWL,    SW,      SDL,    SDR,     SWR,   CACHE,
			_unsoc30,  LWC1,   _unsoc32, PREF,    _unsoc34, _unsoc35,  LWQC2, LD,
			_unsoc38,  SWC1,   _unsoc3a, _rsvoc3b, _unsoc3c, _unsoc3d,  SQC2,  SD
		};

		enum CLASS_SPECIAL
		{
			SLL,  _rsvsp1,  SRL,  SRA,  SLLV,    _rsvsp5,  SRLV,   SRAV,
			JR,   JALR,   MOVZ, MOVN, SYSCALL, BREAK,  _rsvspe,  SYNC,
			MFHI, MTHI,   MFLO, MTLO, DSLLV,   _rsvsp15, DSRLV,  DSRAV,
			MULT, MULTU,  DIV,  DIVU, _unssp1c,  _unssp1d, _unssp1e, _unssp1f,
			ADD,  ADDU,   SUB,  SUBU, AND,     OR,     XOR,    NOR,
			MFSA, MTSA,   SLT,  SLTU, DADD,    DADDU,  DSUB,   DSUBU,
			TGE,  TGEU,   TLT,  TLTU, TEQ,     _rsv2d, TNE,    _rsvsp2f,
			DSLL, _rsvsp31, DSRL, DSRA, DSLL32,  _rsvsp3D, DSRL32, DSRA32
		};

		enum CLASS_REGIMM
		{
			BLTZ,   BGEZ,   BLTZL,   BGEZL,   _rsvriri4,  _rsvri5,  _rsvri6,  _rsvri7,
			TGEI,   TGEIU,  TLTI,    TLTIU,   TEQI,   _rsvrid,  TNEI,   _rsvrif,
			BLTZAL, BGEZAL, BLTZALL, BGEZALL, _rsvri14, _rsvri15, _rsvri16, _rsvri17,
			MTSAB,  MTSAH,  _rsvri1a,  _rsvri1b,  _rsvri1c, _rsvri1d, _rsvri1e, _rsvri1f
		};

		#define GEN_SPECIAL(rs, rt, rd, sa, fn) \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((rd & 0x1F) << 11) | \
			((sa & 0x1F) << 6) | \
			((fn & 0x3F) << 0)


		#define GEN_SPECIAL_CODE(code, fn) \
			((code & 0xFFFFF) << 6) | \
			((fn & 0x3F) << 0)

		#define GEN_SPECIAL_CODE_TRAP(rs, rt, code, fn) \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((code & 0x3FF) << 6) | \
			((fn & 0x3F) << 0)

		#define GEN_REGIMM(rs, fn, imm) \
			(Opcodes::REGIMM << 26) | \
			((rs & 0x1F) << 21) | \
			((fn & 0x1F) << 16) | \
			((imm & 0xFFFF) << 0)

		#define GEN_OP_IMM(op, rs, rt, imm) \
			((op & 0x3F) << 26) | \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((imm & 0xFFFF) << 0)
		
		#define GEN_OP_OFFSET(op, imm) \
			((op & 0x3F) << 26) | \
			((imm & 0x3FFFFFF) << 0)
	}

	namespace Registers
	{
		struct xRegister
		{
			u8 id;
			std::string_view name;
			xRegister(u8 id, std::string_view name) : id(id), name(name) {}
			xRegister() = default;
		};

		#define GENXREG(name) \
			struct xRegister##name : xRegister \
			{ \
				xRegister##name(u8 id, const char* name) : xRegister(id, name){} \
			};

		#define _nr(name, id) \
			name(id, #name)

		// c++ strong typedef when
		GENXREG(GPR);
		GENXREG(COP0);
		GENXREG(FPR);
		GENXREG(FCR);

		const xRegisterGPR
		r0(0, "zero"), zero(0, "zero"),
		_nr(at, 1),  _nr(v0, 2),  _nr(v1, 3),  _nr(a0, 4),  _nr(a1, 5),  _nr(a2, 6),  _nr(a3, 7),
		_nr(t0, 8),  _nr(t1, 9),  _nr(t2, 10), _nr(t3, 11), _nr(t4, 12), _nr(t5, 13), _nr(t6, 14), _nr(t7, 15),
		_nr(s0, 16), _nr(s1, 17), _nr(s2, 18), _nr(s3, 19), _nr(s4, 20), _nr(s5, 21), _nr(s6, 22), _nr(s7, 23),
		_nr(t8, 24), _nr(t9, 25), _nr(k0, 26), _nr(k1, 27), _nr(gp, 28), _nr(sp, 29), _nr(s8, 30), _nr(ra, 31);
		
		const xRegisterCOP0
			_nr(Index, 0),      _nr(Random, 1),     _nr(EntryLo0, 2),   _nr(EntryLo1, 3),
			_nr(Context, 4),    _nr(PageMask, 5),   _nr(Wired, 6),      _nr(_rsv7, 7),
			_nr(BadVAddr, 8),   _nr(Count, 9),      _nr(EntryHi, 10),   _nr(Compare, 11),
			_nr(Status, 12),    _nr(Cause, 13),     _nr(EPC, 14),       _nr(PRId, 15),
			_nr(Config, 16),    _nr(_cp0rsv11, 17), _nr(_cp0rsv12, 18), _nr(_cp0rsv13, 19),
			_nr(_cp0rsv14, 20), _nr(_cp0rsv15, 21), _nr(_cp0rsv16, 22), _nr(BadPAddr, 23),
			_nr(Debug, 24),     _nr(Perf, 25),      _nr(_cp0rsv1a, 26), _nr(_cp0rsv1b, 27),
			_nr(TagLo, 28),     _nr(TagHi, 29),     _nr(ErrorEPC, 30),  _nr(_cp0rsv1f, 31);

		const xRegisterFPR
			_nr(f0, 0),   _nr(f1, 1),   _nr(f2, 2),   _nr(f3, 3),   _nr(f4, 4),   _nr(f5, 5),   _nr(f6, 6),   _nr(f7, 7),
			_nr(f8, 8),   _nr(f9, 9),   _nr(f10, 10), _nr(f11, 11), _nr(f12, 12), _nr(f13, 13), _nr(f14, 14), _nr(f15, 15),
			_nr(f16, 16), _nr(f17, 17), _nr(f18, 18), _nr(f19, 19), _nr(f20, 20), _nr(f21, 21), _nr(f22, 22), _nr(f23, 23),
			_nr(f24, 24), _nr(f25, 25), _nr(f26, 26), _nr(f27, 27), _nr(f28, 28), _nr(f29, 29), _nr(f30, 30), _nr(f31, 31);

		const xRegisterFCR
			_nr(fcr0, 0),   _nr(fcr1, 1),   _nr(fcr2, 2),   _nr(fcr3, 3),   _nr(fcr4, 4),   _nr(fcr5, 5),   _nr(fcr6, 6),   _nr(fcr7, 7),
			_nr(fcr8, 8),   _nr(fcr9, 9),   _nr(fcr10, 10), _nr(fcr11, 11), _nr(fcr12, 12), _nr(fcr13, 13), _nr(fcr14, 14), _nr(fcr15, 15),
			_nr(fcr16, 16), _nr(fcr17, 17), _nr(fcr18, 18), _nr(fcr19, 19), _nr(fcr20, 20), _nr(fcr21, 21), _nr(fcr22, 22), _nr(fcr23, 23),
			_nr(fcr24, 24), _nr(fcr25, 25), _nr(fcr26, 26), _nr(fcr27, 27), _nr(fcr28, 28), _nr(fcr29, 29), _nr(fcr30, 30), _nr(fcr31, 31);

		#undef _nr
		#undef GENXREG
	} // namespace Registers
} // namespace EEmitter

