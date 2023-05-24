#pragma once
#include "Opcodes.h"

#include <tamtypes.h>

namespace EEmitter
{
	using namespace Registers;
	struct xFBRANCH
	{
		struct xForwardBranchBase
		{
			uintptr_t branchAddr;
			uintptr_t target;
			u32 branch;

			xForwardBranchBase(u32 op, bool nopDelaySlot = true);
			void SetTarget();

			friend class Block;

		protected:
			void Finish();
		};

		template <u32 op>
		struct xForwardBranch1 : xForwardBranchBase
		{
			xForwardBranch1(const xRegisterGPR& rs, bool nopDelaySlot = true)
				: xForwardBranchBase(GEN_OP_IMM(op, rs.id, 0x00, 0x00), nopDelaySlot)
			{
			}
		};

		template <u32 op>
		struct xForwardBranch2 : xForwardBranchBase
		{
			xForwardBranch2(const xRegisterGPR& rs, const xRegisterGPR& rt, bool nopDelaySlot = true)
				: xForwardBranchBase(GEN_OP_IMM(op, rs.id, rt.id, 0x00), nopDelaySlot)
			{
			}
		};

		template <u32 op>
		struct xForwardBranchIMM : xForwardBranchBase
		{
			xForwardBranchIMM(const xRegisterGPR& rs, const xRegisterGPR& rt, bool nopDelaySlot = true)
				: xForwardBranchBase(GEN_REGIMM(rs.id, op, 0x00), nopDelaySlot)
			{
			}
		};

		using BEQ = xForwardBranch2<Opcodes::BEQ>;
		using BEQL = xForwardBranch2<Opcodes::BEQL>;
		using BGEZ = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BGEZ>;
		using BGEZAL = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BGEZAL>;
		using BGEZALL = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BGEZALL>;
		using BGEZL = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BGEZL>;
		using BGTZ = xForwardBranch1<Opcodes::BGTZ>;
		using BGTZL = xForwardBranch1<Opcodes::BGTZL>;
		using BLEZ = xForwardBranch1<Opcodes::BLEZ>;
		using BLEZL = xForwardBranch1<Opcodes::BLEZL>;
		using BLTZ = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BLTZ>;
		using BLTZAL = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BLTZAL>;
		using BLTZALL = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BLTZALL>;
		using BLTZL = xForwardBranchIMM<Opcodes::CLASS_REGIMM::BLTZL>;
		using BNE = xForwardBranch2<Opcodes::BNE>;
		using BNEL = xForwardBranch2<Opcodes::BNEL>;
	};
} // namespace EEmitter
