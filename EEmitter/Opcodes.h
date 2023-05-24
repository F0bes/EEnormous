#pragma once
#include <tamtypes.h>
#include <string_view>

namespace EEmitter
{
	namespace Opcodes
	{
		enum OpCode
		{
			SPECIAL,  REGIMM, J,        JAL,      BEQ,      BNE,      BLEZ,  BGTZ,
			ADDI,     ADDIU,  SLTI,     SLTIU,    ANDI,     ORI,      XORI,  LUI,
			COP0,     COP1,   COP2,     _rsvoc13, BEQL,     BNEL,     BLEZL, BGTZL,
			DADDI,    DADDIU, LDL,      LDR,      MMI,      _rsvoc1d, LQ,    SQ,
			LB,       LH,     LWL,      LW,       LBU,      LHU,      LWR,   LWU,
			SB,       SH,     SWL,      SW,       SDL,      SDR,      SWR,   CACHE,
			_unsoc30,  LWC1,  _unsoc32, PREF,     _unsoc34, _unsoc35, LWQC2, LD,
			_unsoc38,  SWC1,  _unsoc3a, _rsvoc3b, _unsoc3c, _unsoc3d, SQC2,  SD
		};

		enum CLASS_SPECIAL
		{
			SLL,  _rsvsp1,  SRL,  SRA,  SLLV,     _rsvsp5,  SRLV,     SRAV,
			JR,   JALR,     MOVZ, MOVN, SYSCALL,  BREAK,    _rsvspe,  SYNC,
			MFHI, MTHI,     MFLO, MTLO, DSLLV,    _rsvsp15, DSRLV,    DSRAV,
			MULT, MULTU,    DIV,  DIVU, _unssp1c, _unssp1d, _unssp1e, _unssp1f,
			ADD,  ADDU,     SUB,  SUBU, AND,      OR,       XOR,      NOR,
			MFSA, MTSA,     SLT,  SLTU, DADD,     DADDU,    DSUB,     DSUBU,
			TGE,  TGEU,     TLT,  TLTU, TEQ,      _rsv2d,   TNE,      _rsvsp2f,
			DSLL, _rsvsp31, DSRL, DSRA, DSLL32,   _rsvsp3D, DSRL32,   DSRA32
		};

		enum CLASS_REGIMM
		{
			BLTZ,   BGEZ,   BLTZL,    BGEZL,    _rsvriri4, _rsvri5,  _rsvri6,  _rsvri7,
			TGEI,   TGEIU,  TLTI,     TLTIU,    TEQI,      _rsvrid,  TNEI,     _rsvrif,
			BLTZAL, BGEZAL, BLTZALL,  BGEZALL,  _rsvri14,  _rsvri15, _rsvri16, _rsvri17,
			MTSAB,  MTSAH,  _rsvri1a, _rsvri1b, _rsvri1c,  _rsvri1d, _rsvri1e, _rsvri1f
		};

		enum CLASS_MMI
		{
			MADD,      MADDU,     _rsvmmi2,  _rsvmmi3,  PLZCW,     _rsvmmi5,  _rsvmmi6,  _rsvmmi7,
			MMI0,      MMI2,      _rsvmmia,  _rsvmmib,  _rsvmmic,  _rsvmmid,  _rsvmmie,  _rsvmmif,
			MFHI1,     MTHI1,     MFLO1,     MTLO1,     _rsvmmi14, _rsvmmi15, _rsvmmi16, _rsvmmi17,
			MULT1,     MULTU1,    DIV1,      DIVU1,     _rsvmmi1c, _rsvmmi1d, _rsvmmi1e, _rsvmmi1f,
			MADD1,     MADDU1,    _rsvmmi22, _rsvmmi23, _rsvmmi24, _rsvmmi25, _rsvmmi26, _rsvmmi27,
			MMI1,      MMI3,      _rsvmmi2a, _rsvmmi2b, _rsvmmi2c, _rsvmmi2d, _rsvmmi2e, _rsvmmi2f,
			PMFHL,     PMTHL,     _rsvmmi32, _rsvmmi33, PSLLH,     _rsvmmi35, PSRLH,     PSRAH,
			_rsvmmi38, _rsvmmi39, _rsvmmi3a, _rsvmmi3b, PSLLW,     _rsvmmi3d, PSRLW,     PSRAW
		};

		enum CLASS_MMI0
		{
			PADDW,  PSUBW,  PCGTW, PMAXW,     PADDH,      PSUBH,      PCGTH,     PMAXH,
			PADDB,  PSUBB,  PCGTB, _rsvmmi0b, _rsvmmi0c,  _rsvmmi0d,  _rsvmmi0e, _rsvmmi0f,
			PADDSW, PSUBSW, PEXTLW, PPACW,    PADDSH,     PSUBSH,     PEXTLH,    PPACH,
			PADDSB, PSUBSB, PEXTLB, PPACB,    _rsvmmi01c, _rsvmmi01d, PEXT5,     PPAC5
		};

		enum CLASS_MMI1
		{
			_rsv1mmi0, PABSW,     PCEQW,  PMINW,      PADSBH,     PABSH,      PCEQH,      PMINH,
			_rsv1mmi8, _rsv1mmi9, PCEQB,  _rsv1mmib,  _rsv1mmic,  _rsv1mmid,  _rsv1mmie,  _rsv1mmif,
			PADDUW,    PSUBUW,    PEXTUW, _rsv1mmi23, PADDSUH,    PSUBSUH,    PEXTUH,     _rsv1mmi27,
			PADDUB,    PSUBUB,    PEXTUB, QFSRV,      _rsv1mmi1c, _rsv1mmi1d, _rsv1mmi1e, _rsv1mmi1f
		};

		enum CLASS_MMI2
		{
			PMADDW,    _rsv2mmi1,  PSLLVW, PSRLVW,    PMSUBW,_rsv2mmi5, _rsv2mmi6,  _rsv2mmi7,
			PMFHI,      PMFLO,     PINTH,  _rsv2mmib, PMULTW, PDIVW,    PCPYLD,     _rsv2mmif,
			PMADDH,     PHMADH,    PAND,   PXOR,      PMUSBH, PHMSBH,   _rsv2mmi16, _rsv2mmi17,
			_rsvmmi218,_rsv2mmi19, PEXEH,  PREVH,     PMULTH, PDIVBW,   PEXEW,      PROT3W
		};

		enum CLASS_MMI3
		{
			PMADDUW,    _rsv3mmi1, _rsv3mmi2, PSRAVW,    _rsv3mmi4, _rsv3mmi5,   _rsv3mmi6,  _rsv3mmi7,
			PMTHI,      PMTLO,      PINTEH,   _rsv3mmib, PMULTUW,    PDIVUW,     PCPYUD,     _rsv3mmif,
			_rsv3mmi10, _rsv3mmi11, POR,      PNOR,      _rsv3mmi14, _rsv3mmi15, _rsv3mmi16, _rsv3mmi17,
			_rsv3mmi18, _rsv3mmi19, PEXCH,    PCPYH,     _rsv3mmi1c, _rsv3mmi1d, PEXUW,      _rsv3mmi1f
		};

		enum CLASS_COP0
		{
			MFC0,       _rsvcop01,  _rsvcop02,  _rsvcop03,  MT0,        _rsvcop05,  _rsvcop06,  _rsvcop07,
			BC0,        _rsvcop09,  _rsvcop0a,  _rsvcop0b,  _rsvcop0c,  _rsvcop0d,  _rsvcop0e,  _rsvcop0f,
			C0,         _rsvcop011, _rsvcop012, _rsvcop013, _rsvcop014, _rsvcop015, _rsvcop016, _rsvcop017,
			_rsvcop018, _rsvcop019, _rsvcop01a, _rsvcop01b, _rsvcop01c, _rsvcop01d, _rsvcop01e, _rsvcop01f
		};
		
		enum CLASS_BC0
		{
			BC0F,      BC0T,      BC0FL,     BC0TL,     _rsvbc04,  _rsvbc05,  _rsvbc06,  _rsvbc07,
			_rsvbc08,  _rsvbc09,  _rsvbc0a,  _rsvbc0b,  _rsvbc0c,  _rsvbc0d,  _rsvbc0e,  _rsvbc0f,
			_rsvbc010, _rsvbc011, _rsvbc012, _rsvbc013, _rsvbc014, _rsvbc015, _rsvbc016, _rsvbc017,
			_rsvbc018, _rsvbc019, _rsvbc01a, _rsvbc01b, _rsvbc01c, _rsvbc01d, _rsvbc01e, _rsvbc01f
		};

		enum CLASS_C0
		{
			_undc00,  TLBR,     TLBWI,    _undc03,  _undc04,  _undc05,  TLBWR ,   _undc07,
			TLBP,     _undc09,  _undc0a,  _undc0b,  _undc0c,  _undc0d,  _undc0e,  _undc0f,
			_undc010, _undc011, _undc012, _undc013, _undc014, _undc015, _undc016, _undc017,
			ERET,     _undc019, _undc01a, _undc01b, _undc01c, _undc01d, _undc01e, _undc01f,
			_undc020, _undc021, _undc022, _undc023, _undc024, _undc025, _undc026, _undc027,
			_undc028, _undc029, _undc02a, _undc02b, _undc02c, _undc02d, _undc02e, _undc02f,
			_undc030, _undc031, _undc032, _undc033, _undc034, _undc035, _undc036, _undc037,
			EI,       DI,       _undc03a, _undc03b, _undc03c, _undc03d, _undc03e, _undc03f,
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

		#define GEN_MMI0(rs, rt,rd, fun) \
			((Opcodes::MMI & 0x3F) << 26) | \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((rd & 0x1F) << 11) | \
			((fun & 0x1F) << 6) | \
			((Opcodes::MMI0 & 0x3F) << 0)

		#define GEN_MMI1(rs, rt,rd, fun) \
			((Opcodes::MMI & 0x3F) << 26) | \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((rd & 0x1F) << 11) | \
			((fun & 0x1F) << 6) | \
			((Opcodes::MMI1 & 0x3F) << 0)

		#define GEN_MMI2(rs, rt,rd, fun) \
			((Opcodes::MMI & 0x3F) << 26) | \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((rd & 0x1F) << 11) | \
			((fun & 0x1F) << 6) | \
			((Opcodes::MMI2 & 0x3F) << 0)

		#define GEN_MMI3(rs, rt,rd, fun) \
			((Opcodes::MMI & 0x3F) << 26) | \
			((rs & 0x1F) << 21) | \
			((rt & 0x1F) << 16) | \
			((rd & 0x1F) << 11) | \
			((fun & 0x1F) << 6) | \
			((Opcodes::MMI3 & 0x3F) << 0)

		#define GEN_COP0_BC0(rt, offset) \
			((op & COP0) << 26) | \
			((rs & BC0) << 21) | \
			((rt & 0x1F) << 16) | \
			((offset & 0xFFFF) << 0)

		#define GEN_COP0_C0(op, rs, fun) \
			((op & COP0) << 26) | \
			((rs & C0) << 21) | \
			((fun & 0x3F) << 0)
	}

	namespace Registers
	{
		struct xRegister
		{
			u8 id;
			std::string_view name;
			xRegister(u8 id, std::string_view name) : id(id), name(name) {}
			xRegister() = default;
			bool operator==(const xRegister& other) const { return id == other.id; } \
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

