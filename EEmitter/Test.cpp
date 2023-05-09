#include "Test.h"
#include "Emitter.h"
#include <kernel.h>
#include <bit>
#include <cstdio>

namespace EEmitter
{
	namespace Test
	{
		namespace internal
		{
			u32 syscall_handler_raised = 0;
			bool handlers_inited = false;
			// Don't you dare call this directly!
			void _SyscallHandler()
			{
				asm volatile(
					"add $sp, $sp, -0x4\n"
					"sw $t0, 0x0($sp)\n"
					"li $t0, 0x7A\n"
					"sw $t0, 0x00(%0)\n"
					"lw $t0, 0x0($sp)\n"
					"add $sp, $sp, 0x4\n"
					"sync\n"
					"ei\n"
					"nop\n" ::"r"(&syscall_handler_raised));
			}

			static size_t assertion_failed_id = 0;
			void _AssertionSyscallHandler()
			{
				using namespace Asserts;
				// Expect v0 to be our assert index that failed
				register size_t assertID asm("v0");
				assertion_failed_id = assertID;
				// Expect a0 to be our 'actual' value (if the assert was with an immediate)
				register u128 actualValueReg asm("a0");
				const u128 actualValue = actualValueReg; // Save here, because a0 is about to be clobbered
				ArithAssertResult& assert = state::lastBlockExecuted->blockAsserts[assertID];

				if (assert.comp_source == CompSource::REG_IMM)
				{
					switch (assert.val_size)
					{
						case sizeof(u8):
						{
							assert.actual_val = std::make_any<u8>(actualValue);
							break;
						}
						case sizeof(u16):
						{
							assert.actual_val = std::make_any<u16>(actualValue);
							break;
						}
						case sizeof(u32):
						{
							assert.actual_val = std::make_any<u32>(actualValue);
							break;
						}
						case sizeof(u64):
						{
							assert.actual_val = std::make_any<u64>(actualValue);
							break;
						}
						case sizeof(u128):
						{
							assert.actual_val = std::make_any<u128>(actualValue);
							break;
						}
					}
				}
				else if (assert.comp_source == CompSource::REG_IMM_FP)
				{
					u32 typedValue = 0;
					asm volatile("sw %1, 0(%0)"
								 : "=r"(typedValue)
								 : "r"(actualValue));
					assert.actual_val = std::bit_cast<float>(typedValue);
				}
				ExitHandler();
			}
		} // namespace internal

		void Init()
		{
			InitHandlers();
		}

		void InitHandlers()
		{
			if (!internal::handlers_inited)
			{
				SetSyscall(SYSCALL_ASSERT_CODE, (void*)internal::_AssertionSyscallHandler);

				internal::handlers_inited = 1;
			}
		}

		void DeInitHandlers()
		{
			// We can leave the syscall handler set
			internal::handlers_inited = 0;
		}

		namespace Asserts
		{
			void genBranchByComptype(const xRegisterGPR& a, const xRegisterGPR& b, CompType comp_type, u16 offset)
			{
				switch (comp_type)
				{
					case CompType::EQ:
						xBEQ(a, b, offset);
						xNOP();
						break;
					case CompType::NE:
						xBNE(a, b, offset);
						xNOP();
						break;
					case CompType::LT:
						xSUB(a, b);
						xBLTZ(a, offset + 1);
						xNOP();
						break;
					case CompType::LE:
						xSUB(a, b);
						xBLEZ(a, offset + 1);
						xNOP();
						break;
					case CompType::GT:
						xSUB(a, b);
						xBGTZ(a, offset + 1);
						xNOP();
						break;
					case CompType::GE:
						xSUB(a, b);
						xBGEZ(a, offset + 1);
						xNOP();
						break;
				}
			}

			// Jumped to directly from a block when we hit an assertion
			// We need to return to ret_ptr!
			void _AssertionUsermodeHandler()
			{
				const ArithAssertResult& assert = state::lastBlockExecuted->blockAsserts[internal::assertion_failed_id];
				char msg[512];

				if (assert.comp_source == CompSource::REG_REG)
				{
					std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s %s\n",
						state::lastBlockExecuted->GetName().cbegin(),
						assert.regA.name.cbegin(),
						CompTypeToString(assert.comp_type).cbegin(),
						assert.regB.name.cbegin());
				}
				else if (assert.comp_source == CompSource::REG_IMM)
				{
					std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s 0x%X (%s actual 0x%X)\n",
						state::lastBlockExecuted->GetName().cbegin(),
						assert.regA.name.cbegin(),
						CompTypeToString(assert.comp_type).cbegin(),
						std::any_cast<u32>(assert.expected_val),
						assert.regA.name.cbegin(),
						std::any_cast<u32>(assert.actual_val));
				}

				puts(msg);
			}

			void xASSERT::CMP(const xRegisterGPR& a, const xRegisterGPR& b, CompType comp_type)
			{
				state::block->blockAsserts.emplace_back(ArithAssertResult(comp_type, a, b));
				const size_t assertID = state::block->blockAsserts.size() - 1;

				genBranchByComptype(a, b, comp_type, 6);
				xORI(v0, zero, assertID);
				xORI(v1, zero, SYSCALL_ASSERT_CODE);
				xSYSCALL();
				xJ((uintptr_t)_AssertionUsermodeHandler);
				xNOP();
				xNOP();
			}

			void xASSERT::CMP(const xRegisterGPR& a, const std::any& imm, CompType comp_type, size_t bytes)
			{
				if (a.id == v0.id)
				{
					printf("xASSERT::CMP: Cannot compare v0 with immediate\n");
					return;
				}

				state::block->blockAsserts.emplace_back(ArithAssertResult(comp_type, imm, bytes, a));
				const size_t assertID = state::block->blockAsserts.size() - 1;

				switch (bytes)
				{
					case sizeof(u8):
					{
						xLI(v0, std::any_cast<u8>(imm));
						break;
					}
					case sizeof(u16):
					{
						xLI(v0, std::any_cast<u16>(imm));
						break;
					}
					case sizeof(u32):
					{
						xLI(v0, std::any_cast<u32>(imm));
						break;
					}
					case sizeof(u64):
					{
						// do something, dunno yet
						break;
					}
					case sizeof(u128):
					{
						// await MMI instruction implementation
						break;
					}
				}

				genBranchByComptype(a, v0, comp_type, 7);
				xORI(v0, zero, assertID);
				xORI(v1, zero, SYSCALL_ASSERT_CODE);
				xADD(a0, a, zero);
				xSYSCALL();
				xJ((uintptr_t)_AssertionUsermodeHandler);
				xNOP();
				xNOP();
			}
		} // namespace Asserts
	} // namespace Test
} // namespace EEmitter
