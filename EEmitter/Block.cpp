#include "Block.h"
#include "Emitter.h"

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

	void Block::Finish()
	{
		// todo? block exit?
		*ptr++ = 0x03E00008;
		*ptr++ = 00000000;
	}

	void Block::Clear()
	{
		for (u32* i = start; i < ptr; i++)
			*i = 0x00000000;
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
			"addiu $sp, $sp, 0x10\n"
			:
			: "r"(start));
	}
} // namespace EEmitter
