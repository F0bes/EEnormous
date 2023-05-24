#pragma once
#include "Opcodes.h"
#include <tamtypes.h>
#include <cstdint>
#include <cstdio>
#include <any>

namespace EEmitter
{
	namespace Test
	{
		// Don't call this outside of a test block
		const u32 SYSCALL_ASSERT_CODE = 0x75;

		namespace internal
		{
			extern bool handlers_inited;
		} // namespace internal

		void Init();
		void InitHandlers();
		void DeInitHandlers();

		namespace Asserts
		{
			using namespace Registers;

			enum class CompSource
			{
				REG_REG,
				REG_IMM,
				REG_IMM_FP,
				REG_REG_FP,
			};

			enum class CompType
			{
				EQ,
				NE,
				LT,
				GT,
				LE,
				GE,
			};

			static constexpr std::string_view CompTypeToString(CompType type)
			{
				switch (type)
				{
					case CompType::EQ:
						return "==";
					case CompType::NE:
						return "!=";
					case CompType::LT:
						return "<";
					case CompType::GT:
						return ">";
					case CompType::LE:
						return "<=";
					case CompType::GE:
						return ">=";
				}
				return "";
			}

			struct ArithAssertResult
			{
				CompType comp_type;
				CompSource comp_source;

				// Used for immediate comparisons
				std::any expected_val;
				std::any actual_val;
				size_t val_size;
				float delta;

				xRegister regA;
				xRegister regB;

				ArithAssertResult(const CompType comp_type, const std::any& expected, size_t val_size, const xRegister& reg)
					: comp_type(comp_type)
					, comp_source(CompSource::REG_IMM)
					, expected_val(expected)
					, actual_val(0)
					, val_size(val_size)
					, regA(reg)
				{
				}
				ArithAssertResult(const CompType comp_type, const xRegister& a, const xRegister& b)
					: comp_type(comp_type)
					, comp_source(CompSource::REG_REG)
					, regA(a)
					, regB(b)
				{
				}
				ArithAssertResult(const CompType comp_type, const std::any& expected, float delta, const xRegister& reg)
					: comp_type(comp_type)
					, comp_source(CompSource::REG_IMM_FP)
					, expected_val(expected)
					, actual_val(0)
					, val_size(sizeof(float))
					, delta(delta)
					, regA(reg)
				{
				}
				ArithAssertResult(const CompType comp_type, float delta, const xRegister& a, const xRegister& b)
					: comp_type(comp_type)
					, comp_source(CompSource::REG_REG_FP)
					, delta(delta)
					, regA(a)
					, regB(b)
				{
				}
				~ArithAssertResult() = default;
			};

			class xASSERT
			{
			public:
				// GPR->GPR
				static void CMP(const xRegisterGPR& a, const xRegisterGPR& b, CompType comp_type);
				// GPR-imm
				static void CMP(const xRegisterGPR& a, const std::any& imm, CompType comp_type, size_t bytes);

				// helper for GPR->imm
				template <typename T>
				static void CMP(const xRegisterGPR& a, T imm, CompType comp_type)
				{
					CMP(a, std::make_any<T>(imm), comp_type, sizeof(T));
				}
			};
		} // namespace Asserts
	} // namespace Test
} // namespace EEmitter
