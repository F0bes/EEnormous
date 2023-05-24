#include "Block.h"
#include "Emitter.h"

#include <kernel.h> // Flushcache

namespace EEmitter
{
	Block::Block(u32* addr)
		: start(addr)
		, ptr(addr)
	{
	}

	Block::Block(u32* addr, std::string_view name)
		: start(addr)
		, ptr(addr)
		, name(name)
	{
	}

	const std::string_view Block::GetName()
	{
		return name;
	}

	u32 Block::GetCurrentPC()
	{
		return reinterpret_cast<u32>(ptr);
	}

	void Block::Finish()
	{
		*ptr++ = 0x03E00008; // jr $ra
		*ptr++ = 00000000; // nop

		for (auto& branch : blockBranches)
		{
			branch.Finish(); // Clean up any branches that need to be set
		}

		blockBranches.clear(); // Once the branch is set, we don't need to track it
		FlushCache(0);
	}

	void Block::Clear()
	{
		ptr = start;
		blockAsserts.clear();
		blockBranches.clear();
		FreeAllRegisters();
	}

	void Block::Emit(u32 opcode)
	{
		*ptr++ = opcode;
	}

	void Block::Execute()
	{
		EEmitter::state::lastBlockExecuted = this;
		asm volatile(
			"addiu $sp, $sp, -0x10\n"
			"sw $ra, 0x0($sp)\n"
			"jalr %0\n"
			"nop\n"
			"lw $ra, 0x0($sp)\n"
			"addiu $sp, $sp, 0x10\n" ::"r"(start)
			: "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7");
	}

	const xRegisterGPR& Block::AllocateRegister()
	{
		for (auto& reg : registerState)
		{
			if (!reg.second)
			{
				reg.second = true;
				return reg.first;
			}
		}
		// TODO: Write least used register to memory and give it to the caller?
		return zero;
	}

	bool Block::TryAllocateRegister(const xRegisterGPR& reg)
	{
		for (auto& regState : registerState)
		{
			if (regState.first == reg)
			{
				if (regState.second)
				{
					return false;
				}
				else
				{
					regState.second = true;
					return true;
				}
			}
		}
		return false;
	}

	void Block::FreeRegister(const xRegisterGPR& reg)
	{
		for (auto& regState : registerState)
		{
			if (regState.first == reg)
			{
				regState.second = false;
				return;
			}
		}
	}

	bool Block::IsRegisterFree(const xRegisterGPR& reg)
	{
		for (auto& regState : registerState)
		{
			if (regState.first == reg)
			{
				return !regState.second;
			}
		}
		return false;
	}

	void Block::FreeAllRegisters()
	{
		for (auto& regState : registerState)
		{
			regState.second = false;
		}
	}
} // namespace EEmitter
