#pragma once
#include "Test.h"
#include <tamtypes.h>

#include <vector>
#include <string_view>
#include <functional>

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

		// TODO: Implement custom handlers? How will you differenciante between
		// float vs int vs interrupts, etc?

		const std::string_view GetName();

		// Write a proper block exit
		void Finish();

		// Clear the contents of the block
		void Clear();

		// Emit an opcode to the current blockpc
		void Emit(u32 opcode);
		// Execute the block, this function will return
		void Execute();


		std::vector<Test::Asserts::ArithAssertResult> blockAsserts;

	private:
		u32* start; // Start of our block
		u32* ptr; // Current blockpc

		std::string_view name;
		void success_handler(const Test::Asserts::ArithAssertResult& result);
		void fail_handler(const Test::Asserts::ArithAssertResult& result);
	};
} // namespace EEmitter
