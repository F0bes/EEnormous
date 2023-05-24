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
				auto scratch = xAllocReg();
				switch (comp_type)
				{
					case CompType::EQ:
						xXOR(scratch, b, a);
						xBEQ(scratch, zero, offset);
						xNOP();
						break;
					case CompType::NE:
						xXOR(scratch, b, a);
						xBNE(scratch, zero, offset);
						xNOP();
						break;
					case CompType::LT:
						xSUB(scratch, b, a);
						xBGTZ(scratch, offset + 1);
						xNOP();
						break;
					case CompType::LE:
						xSUB(scratch, b, a);
						xBGEZ(scratch, offset + 1);
						xNOP();
						break;
					case CompType::GT:
						xSUB(scratch, b, a);
						xBLTZ(scratch, offset + 1);
						xNOP();
						break;
					case CompType::GE:
						xSUB(scratch, b, a);
						xBLEZ(scratch, offset + 1);
						xNOP();
						break;
				}
				xFreeReg(scratch);
			}

			void genMaskFromSize(const xRegisterGPR& value, size_t size)
			{
				auto scratch = xAllocReg();
				switch (size)
				{
					case sizeof(u8):
						xLI(scratch, 0xFF);
						xAND(value, scratch);
						break;
					case sizeof(u16):
						xLI(scratch, 0xFFFF);
						xAND(value, scratch);
						break;
					case sizeof(u32):
						xLI(scratch, 0xFFFFFFFF);
						// Clear upper 32 bits
						xPEXTLW(scratch, zero, scratch);
						xAND(value, scratch);
						break;
					case sizeof(u64):
						// Sign extention will set the top 64 bits
						xLI(scratch, 0xFFFFFFFF);
						xAND(value, scratch);
						break;
				}
				xFreeReg(scratch);
			}

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
					// A better runtime typing system needs to be implemented!
					switch (assert.val_size)
					{
						case sizeof(u8):
						{
							std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s 0x%x (%s actual 0x%x)\n",
								state::lastBlockExecuted->GetName().cbegin(),
								assert.regA.name.cbegin(),
								CompTypeToString(assert.comp_type).cbegin(),
								std::any_cast<u8>(assert.expected_val),
								assert.regA.name.cbegin(),
								std::any_cast<u8>(assert.actual_val));
							break;
						}
						case sizeof(u16):
						{
							std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s 0x%x (%s actual 0x%x)\n",
								state::lastBlockExecuted->GetName().cbegin(),
								assert.regA.name.cbegin(),
								CompTypeToString(assert.comp_type).cbegin(),
								std::any_cast<u16>(assert.expected_val),
								assert.regA.name.cbegin(),
								std::any_cast<u16>(assert.actual_val));
							break;
						}
						case sizeof(u32):
						{
							std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s 0x%x (%s actual 0x%x)\n",
								state::lastBlockExecuted->GetName().cbegin(),
								assert.regA.name.cbegin(),
								CompTypeToString(assert.comp_type).cbegin(),
								std::any_cast<u32>(assert.expected_val),
								assert.regA.name.cbegin(),
								std::any_cast<u32>(assert.actual_val));
							break;
						}
						case sizeof(u64):
						{
							std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s 0x%llx (%s actual 0x%llx)\n",
								state::lastBlockExecuted->GetName().cbegin(),
								assert.regA.name.cbegin(),
								CompTypeToString(assert.comp_type).cbegin(),
								std::any_cast<u64>(assert.expected_val),
								assert.regA.name.cbegin(),
								std::any_cast<u64>(assert.actual_val));
							break;
						}
						case sizeof(u128):
						{
							std::snprintf(msg, sizeof(msg), "<%s> Assertion failed: %s %s (128-bit imm view not implemented)\n",
								state::lastBlockExecuted->GetName().cbegin(),
								assert.regA.name.cbegin(),
								CompTypeToString(assert.comp_type).cbegin());
							break;
						}
					}
				}

				puts(msg);
			}

			void xASSERT::CMP(const xRegisterGPR& a, const xRegisterGPR& b, CompType comp_type)
			{
				state::block->blockAsserts.emplace_back(ArithAssertResult(comp_type, a, b));
				const size_t assertID = state::block->blockAsserts.size() - 1;

				genBranchByComptype(a, b, comp_type, 7);
				xORI(v0, zero, 9);
				xLD(v0, v0, 0);
				// TODO: Preserve v0, v1 and a0
				xORI(v0, zero, assertID);
				xORI(v1, zero, SYSCALL_ASSERT_CODE);
				xSYSCALL();
				xJ((uintptr_t)_AssertionUsermodeHandler);
				xNOP();
				xNOP();
			}

			void xASSERT::CMP(const xRegisterGPR& a, const std::any& imm, CompType comp_type, size_t bytes)
			{
				if (bytes >= sizeof(u128))
				{
					printf("128-bit compare is not available!\n");
					return;
				}
				GetBlock()->blockAsserts.emplace_back(ArithAssertResult(comp_type, imm, bytes, a));
				const size_t assertID = state::block->blockAsserts.size() - 1;

				auto immReg = xAllocReg();
				switch (bytes)
				{
					case sizeof(u8):
					{
						xLI(immReg, std::any_cast<u8>(imm));
						break;
					}
					case sizeof(u16):
					{
						xLI(immReg, std::any_cast<u16>(imm));
						break;
					}
					case sizeof(u32):
					{
						xLI(immReg, std::any_cast<u32>(imm));
						// Clear the top 32 bits of the imm reg
						// Our mask will have the top 32 bits cleared
						// (This is becoming my favourite instruction ^_^)
						xPEXTLW(immReg, zero, immReg);
						break;
					}
					case sizeof(u64):
					{
						xLI(immReg, std::any_cast<u64>(imm) >> 32);
						auto tmp = xAllocReg();
						xLI(tmp, std::any_cast<u64>(imm));
						xPEXTLW(immReg, immReg, tmp);
						xFreeReg(tmp);
						break;
					}
				}

				genMaskFromSize(a, bytes);
				genBranchByComptype(a, immReg, comp_type, 7);

				// TODO: Preserve v0, v1 and a0
				xORI(v0, zero, assertID);
				xORI(v1, zero, SYSCALL_ASSERT_CODE);
				if (bytes > sizeof(u32))
					xPOR(a0, a, zero);
				else
					xADD(a0, a, zero);
				xSYSCALL();

				xJ((uintptr_t)_AssertionUsermodeHandler);

				xNOP();
				xNOP();
				xFreeReg(immReg);
			}
		} // namespace Asserts
	} // namespace Test
} // namespace EEmitter
