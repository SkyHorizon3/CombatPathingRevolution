#pragma once

namespace CombatPathing
{
	//Disable circling when the combat target not within circling distance range.
	class CirclingChanceHook
	{
	public:
		static void InstallHook()
		{
			// checked: 1.6.1170, 1.7.99
			REL::Relocation<std::uintptr_t> Base{ REL::ID(50647) };

			auto& trampoline = REL::GetTrampoline();
			_GetCirclingChance = trampoline.write_jmp<5>(Base.address() + 0x22, GetCirclingChance);

			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance);

		static inline REL::Relocation<decltype(GetCirclingChance)> _GetCirclingChance;
	};

	//Insert a sibling circle node before the advance node, to allow advance action switch to circle.
	class AdvanceToCircleHook
	{
	public:
		static void InstallHook()
		{
			// checked: 1.6.1170, 1.7.99
			REL::Relocation<std::uintptr_t> Base{ REL::ID(47928) };

			auto& trampoline = REL::GetTrampoline();
			_PushBackNode = trampoline.write_call<5>(Base.address() + 0xE1C, PushBackNode);

			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static RE::CombatBehaviorTree::TreeBuilder* PushBackNode(RE::CombatBehaviorTree::TreeBuilder* a_master, RE::CombatBehaviorTree::TreeBuilder* a_target);

		static float GetCircleChance(RE::Actor* a_actor)
		{
			using func_t = decltype(&AdvanceToCircleHook::GetCircleChance);
			static REL::Relocation<func_t> func{ REL::ID(50647) };
			return func(a_actor);
		}

		static inline REL::Relocation<decltype(PushBackNode)> _PushBackNode;

		static bool ShouldCircle(void* a_context);
	};

	class CircleAngleHook1
	{
	public:
		static void InstallHook()
		{
			auto& trampoline = REL::GetTrampoline();

			// checked: 1.6.1170, 1.7.99
			REL::Relocation<std::uintptr_t> Base{ REL::ID(50648) };
			_RescaleCircleAngle = trampoline.write_call<5>(Base.address() + 0x44, RescaleCircleAngle);
			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float RescaleCircleAngle(const float a_circleMult, const float a_minAnlge, const float a_maxAngle);

		static inline REL::Relocation<decltype(RescaleCircleAngle)> _RescaleCircleAngle;
	};

	class CircleAngleHook2
	{
		static float GetMinCircleAngle();

		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				movss(xmm8, xmm0);

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
			REL::Relocation<std::uintptr_t> target{ REL::ID(50648), 0x4C };
			REL::WriteSafeFill(target.address(), REL::NOP, 0x9);

			auto trampolineJmp = Patch(target.address() + 0x9, REX::UNRESTRICTED_CAST<std::uintptr_t>(GetMinCircleAngle));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(target.address(), trampoline.allocate(trampolineJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}
	};

	class CircleAngleHook3
	{
		static float GetMaxCircleAngle();

		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				comiss(xmm11, xmm0);

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
			REL::Relocation<std::uintptr_t> target{ REL::ID(47916), 0x234 };
			REL::WriteSafeFill(target.address(), REL::NOP, 0x8);

			auto trampolineJmp = Patch(target.address() + 0x8, REX::UNRESTRICTED_CAST<std::uintptr_t>(GetMaxCircleAngle));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(target.address(), trampoline.allocate(trampolineJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}
	};

	class CircleViewConeHook
	{
	public:
		static void InstallHook()
		{
			// checked: 1.6.1170, 1.7.99
			REL::Relocation<std::uintptr_t> WithinHeadingAngleBase{ REL::ID(47916) };

			auto& trampoline = REL::GetTrampoline();
			_WithinHeadingAngle = trampoline.write_call<5>(WithinHeadingAngleBase.address() + 0x2F2, WithinHeadingAngle);

			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static bool WithinHeadingAngle(RE::Actor* he, RE::NiPoint3* a_pos, float a_angle);

		static inline REL::Relocation<decltype(WithinHeadingAngle)> _WithinHeadingAngle;
	};
}