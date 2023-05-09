#include <stdio.h>
#include <stdlib.h>
#include <kernel.h>
#include <tamtypes.h>
#include "EEmitter/Test.h"
#include "EEmitter/Emitter.h"

using namespace EEmitter;
using namespace EEmitter::Test::Asserts;

void Test_ADDI()
{
	uintptr_t* blockAddr = static_cast<uintptr_t*>(aligned_alloc(4, sizeof(uintptr_t) * 100));
	Block block(blockAddr, "Test_ADDI (Expected to fail)");

	SetBlock(&block);
	xADDI(t0, zero, 0x12);
	xASSERT::CMP<u32>(t0, 0x11, CompType::EQ);
	block.Finish();

	block.Execute();

	free(blockAddr);
}

// Overengineered and silly, but serves as an example
template <size_t N, u32 base>
struct add_truths
{
	constexpr add_truths()
		: results()
	{
		for (size_t i = 0; i < N; i++)
		{
			results[i] = i + base;
		}
	}
	u32 results[N];
};

void Test_ADDIU_Loop()
{
	uintptr_t* blockAddr = static_cast<uintptr_t*>(aligned_alloc(4, sizeof(uintptr_t) * 300));
	Block block(blockAddr, "Test_ADDIU_Looped");

	SetBlock(&block);

	// Emitter suppports pseudo instructions!
	xLI(t0, 0xdead);

	constexpr auto truths = add_truths<25, 0xdead>();
	for (size_t i = 0; i < 25; i++)
	{
		xADDIU(t1, t0, i);
		xASSERT::CMP<u32>(t1, truths.results[i], CompType::EQ);
	}

	block.Finish();
	block.Execute();

	free(blockAddr);
}

int main(void)
{
	// Setup for syscall testing and assert handling

	EEmitter::Test::Init();
	Test_ADDI();
	Test_ADDIU_Loop();

	SleepThread();
}
