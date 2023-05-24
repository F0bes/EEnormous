#include <stdio.h>
#include <stdlib.h>
#include <kernel.h>
#include <tamtypes.h>
#include "EEmitter/Test.h"
#include "EEmitter/Emitter.h"
#include "EEmitter/Branch.h"

using namespace EEmitter;
using namespace EEmitter::Test::Asserts;

void Test_ADDI()
{
	uintptr_t* blockAddr = static_cast<uintptr_t*>(aligned_alloc(4, sizeof(uintptr_t) * 100));
	printf("Block address: %p\n", blockAddr);
	Block block(blockAddr, "Test_ADDI (Expected to fail)");

	SetBlock(&block);
	xADDI(t0, zero, 0x11);
	xASSERT::CMP<u16>(t0, 0x10, CompType::LT);
	block.Finish();

	block.Execute();

	free(blockAddr);
}

void Test_Branch()
{
	uintptr_t* blockAddr = static_cast<uintptr_t*>(aligned_alloc(4, sizeof(uintptr_t) * 200));
	printf("Block address: %p\n", blockAddr);
	Block block(blockAddr, "Test_Branch");

	SetBlock(&block);

	auto temp = xAllocReg();
	auto temp2 = xAllocReg();

	xLI(temp, 0xdead);
	xLI(temp2, 0xbeef);
	xFBRANCH::BEQL branch(temp, temp2, false);

	xORI(temp, zero, 0xBAD);
	xNOP();
	branch.SetTarget();

	xASSERT::CMP<u32>(temp, 0xBAD, CompType::NE);

	block.Finish();
	block.Execute();
	free(blockAddr);
}

void main_thread(void* arg)
{
	printf("Hello from main thread!\n");
	EEmitter::Test::Init();
	Test_ADDI();
	Test_Branch();

	SleepThread();
}

int main(void)
{
	ee_thread_t thread;
	thread.func = reinterpret_cast<void*>(main_thread);
	thread.stack = aligned_alloc(4, 0x300000);
	thread.stack_size = 0x300000;
	thread.gp_reg = &_gp;
	thread.initial_priority = 0x60;

	auto threadID = CreateThread(&thread);

	StartThread(threadID, nullptr);

	printf("Complete\n");
	SleepThread();
}
