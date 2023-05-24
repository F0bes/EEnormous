#pragma once
#include "Test.h"
#include "Branch.h"

#include <tamtypes.h>

#include <vector>
#include <string_view>
#include <array>

namespace EEmitter
{
	class Block
	{
	public:
		// Create a simple block at the given address
		Block(u32* addr);

		// Create a test block at the given address
		// The default success / fail handlers will be used
		Block(u32* addr, std::string_view name);

		// TODO: Implement custom handlers? How will you differentiate between
		// float vs int vs interrupts, etc?

		const std::string_view GetName();

		u32 GetCurrentPC();

		// Write a proper block exit, flush the cache
		void Finish();

		// Clear the contents of the block
		void Clear();

		// Emit an opcode to the current blockpc
		void Emit(u32 opcode);
		// Execute the block, this function will return
		void Execute();

		std::vector<Test::Asserts::ArithAssertResult> blockAsserts;
		std::vector<xFBRANCH::xForwardBranchBase> blockBranches;

		const Registers::xRegisterGPR& AllocateRegister();

		// Returns true if the register was allocated
		bool TryAllocateRegister(const Registers::xRegisterGPR& reg);
		void FreeRegister(const Registers::xRegisterGPR& reg);
		bool IsRegisterFree(const Registers::xRegisterGPR& reg);
		void FreeAllRegisters();

	private:
		u32* start; // Start of our block
		u32* ptr; // Current blockpc

		std::string_view name;
		void success_handler(const Test::Asserts::ArithAssertResult& result);
		void fail_handler(const Test::Asserts::ArithAssertResult& result);

		// Available scratch registers
		// Save a0,a1,v0,v1. Maybe for function calls?
		std::array<std::pair<const Registers::xRegisterGPR, bool>, 18> registerState{
			std::make_pair(Registers::a2, false),
			std::make_pair(Registers::a3, false),
			std::make_pair(Registers::t0, false),
			std::make_pair(Registers::t1, false),
			std::make_pair(Registers::t2, false),
			std::make_pair(Registers::t3, false),
			std::make_pair(Registers::t4, false),
			std::make_pair(Registers::t5, false),
			std::make_pair(Registers::t7, false),
			std::make_pair(Registers::s0, false),
			std::make_pair(Registers::s1, false),
			std::make_pair(Registers::s2, false),
			std::make_pair(Registers::s3, false),
			std::make_pair(Registers::s4, false),
			std::make_pair(Registers::s5, false),
			std::make_pair(Registers::s6, false),
			std::make_pair(Registers::s7, false),
			std::make_pair(Registers::s8, false)};

	};
} // namespace EEmitter
