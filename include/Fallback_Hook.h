#pragma once

namespace CombatPathing
{
	class FallbackDistanceHook1
	{
	public:
		static void InstallHook()
		{
			// checked: 1.6.1170, 1.7.99

			auto& trampoline = REL::GetTrampoline();
			REL::Relocation<std::uintptr_t> Base{ REL::ID(47908) };
			_GetFallbackDistance = trampoline.write_call<5>(Base.address() + 0x1B0, GetFallbackDistance);
			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetFallbackDistance(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackDistance)> _GetFallbackDistance;
	};

	class FallbackDistanceHook2
	{
		static float GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor* a_he);

		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				mov(rdx, r13);
				mov(rcx, r15);

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				addss(xmm6, xmm0);

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

	public:
		static void InstallHook()
		{
			// checked: 1.6.1170, 1.7.99
			REL::Relocation<std::uintptr_t> target{ REL::ID(47909), 0x2E7 };
			REL::WriteSafeFill(target.address(), REL::NOP, 0x8);

			auto trampolineJmp = Patch(target.address() + 0x8, REX::UNRESTRICTED_CAST<std::uintptr_t>(GetMaxFallbackDistance));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(target.address(), trampoline.allocate(trampolineJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}
	};

	class FallbackWaitTimeHook1
	{
	public:
		static void InstallHook()
		{
			auto& trampoline = REL::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::ID(47909) };
			_GetFallbackWaitTime = trampoline.write_call<5>(Base.address() + 0x12E, GetFallbackWaitTime);
			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetFallbackWaitTime(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackWaitTime)> _GetFallbackWaitTime;
	};

	class FallbackWaitTimeHook2
	{
		static float GetMinFallbackWaitTime(RE::Actor* a_me, RE::Actor* a_he);

		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				mov(rdx, r13);
				mov(rcx, r15);

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				movss(xmm1, xmm0);

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

	public:
		static void InstallHook()
		{
			// checked: 1.6.1170, 1.7.99
			REL::Relocation<std::uintptr_t> target{ REL::ID(47909), 0x362 };
			REL::WriteSafeFill(target.address(), REL::NOP, 0x8);

			auto trampolineJmp = Patch(target.address() + 0x8, REX::UNRESTRICTED_CAST<std::uintptr_t>(GetMinFallbackWaitTime));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(target.address(), trampoline.allocate(trampolineJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}
	};
}