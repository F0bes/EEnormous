#include "Branch.h"
#include "Emitter.h"

namespace EEmitter
{
	xFBRANCH::xForwardBranchBase::xForwardBranchBase(u32 opcode, bool nopDelaySlot)
		: branch(opcode)
	{
		branchAddr = GetBlock()->GetCurrentPC();
		xNOP();
		if (nopDelaySlot)
			xNOP();
	}

	void xFBRANCH::xForwardBranchBase::SetTarget()
	{
		this->target = GetBlock()->GetCurrentPC();
		GetBlock()->blockBranches.push_back(*this);
	}

	void xFBRANCH::xForwardBranchBase::Finish()
	{
		u32 offset = target - branchAddr;
		offset -= 4;
		offset /= 4;
		offset &= 0xFFFF;
		*reinterpret_cast<u32*>(branchAddr) = branch | offset;
	}
} // namespace EEmitter
