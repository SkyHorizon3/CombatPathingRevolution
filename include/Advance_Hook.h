#pragma once

namespace CombatPathing
{
	class AdvanceRadiusHook
	{
		static constexpr char ENABLE_RADIUS_GV[] = "CPR_EnableAdvanceRadius",
							  INNER_MIN_GV[] = "CPR_InnerRadiusMin", INNER_MID_GV[] = "CPR_InnerRadiusMid", INNER_MAX_GV[] = "CPR_InnerRadiusMax",
							  OUTER_MIN_GV[] = "CPR_OuterRadiusMin", OUTER_MID_GV[] = "CPR_OuterRadiusMid", OUTER_MAX_GV[] = "CPR_OuterRadiusMax";

		struct RadiusPatch : Xbyak::CodeGenerator
		{
			RadiusPatch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				xorps(xmm7, xmm7);      // 0x99; original code
				movss(ptr[rsi], xmm0);  // 0x9C; original code

				lahf();  // prologue
				push(rax);
				mov(rdx, rdi);
				movss(xmm2, xmm6);
				mov(r9, rbp);
				push(rbx);

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				pop(rbx);  // epilog
				pop(rax);
				sahf();

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

		struct MedianPatch : Xbyak::CodeGenerator
		{
			MedianPatch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;
				Xbyak::Label setting;

				movss(xmm1, ptr[rip + setting]);  // 0x139; original code
				movss(ptr[rsi], xmm1);            // 0x141; original code

				xor_(cl, cl);  // prologue

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				/*pop(rbx);  // epilog
				pop(rax);
				sahf();*/

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);

				L(setting);
				dq(REL::ID(371737).address());  // checked: 1.6.1170, 1.7.99
			}
		};

	public:
		static void InstallHook()
		{
			constexpr auto address = REL::ID(50643);
			REL::Relocation<std::uintptr_t> radiusTarget{ address, 0x99 };
			REL::WriteSafeFill(radiusTarget.address(), REL::NOP, 0x7);

			auto trampolineJmp = RadiusPatch(radiusTarget.address() + 0x7, REX::UNRESTRICTED_CAST<std::uintptr_t>(RecalculateAdvanceRadius));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(radiusTarget.address(), trampoline.allocate(trampolineJmp));

			REL::Relocation<std::uintptr_t> medianTarget{ address, 0x139 };
			REL::WriteSafeFill(medianTarget.address(), REL::NOP, 0xC);

			auto medianJmp = MedianPatch(medianTarget.address() + 0xC, REX::UNRESTRICTED_CAST<std::uintptr_t>(RecalculateAdvanceRadius));
			trampoline.write_jmp<5>(medianTarget.address(), trampoline.allocate(medianJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float RescaleRadius(float a_delta, float min, float mid, float max);

		// cl, rdx, xmm2, r9, rsp-0x8
		static void RecalculateAdvanceRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker);
	};

	class AdvanceInterruptHook
	{
	public:
		static void InstallHook()
		{
			auto& trampoline = REL::GetTrampoline();
			_Update = trampoline.write_jmp<5>(REL::ID(48092).address() + 0x9, Update);  // checked: 1.6.1170, 1.7.99
			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static constexpr char INTERRUPT_ACTION_GV[] = "CPR_InterruptAction";

		static void Update(RE::CombatBehaviorAdvance* context);
		static inline REL::Relocation<decltype(Update)> _Update;
	};
}