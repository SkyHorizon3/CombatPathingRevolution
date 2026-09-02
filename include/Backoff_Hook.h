#pragma once

namespace CombatPathing
{
	class BackoffStartHook
	{
		static float RescaleBackoffMinDistanceMult(RE::Actor* a_me, RE::Actor* a_he);

		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				// don't execute original code, we return our float in xmm0
				// rcx, rdx still populated with Actor*

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

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
			REL::Relocation<std::uintptr_t> target{ REL::ID(47920), 0x1BE };
			REL::WriteSafeFill(target.address(), REL::NOP, 0x8);

			auto trampolineJmp = Patch(target.address() + 0x8, REX::UNRESTRICTED_CAST<std::uintptr_t>(RescaleBackoffMinDistanceMult));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(target.address(), trampoline.allocate(trampolineJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}
	};

	class BackoffChanceHookAE : public Xbyak::CodeGenerator
	{
	public:
		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				push(rcx);
				push(rdx);

				mov(rdx, rbx);              // RE::CombatBehaviorTreeNode* a_node
				lea(rcx, ptr[rbp - 0x28]);  // RE::NodeArray& a_array

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				pop(rdx);
				pop(rcx);

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> target{ REL::ID(47928), 0x127 };
			REL::WriteSafeFill(target.address(), REL::NOP, 0x7);

			// jump with the return over inlined code that should not be executed since we replaced it with our thunk
			auto trampolineJmp = TrampolineCall(target.address() + 0xCE, reinterpret_cast<std::uintptr_t>(thunk));
			auto& trampoline = REL::GetTrampoline();
			trampoline.write_jmp<5>(target.address(), trampoline.allocate(trampolineJmp));

			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		struct Function  // just simulate the 500 templates Bethesda used here, REing this would be a lifetime task
		{
			float (*function)(RE::Actor*, RE::Actor*);
		};

		// CombatBehaviorTree::AddRandomNode<CombatBehaviorExpression<CombatBehaviorFunc2<float (*)(Actor *,Actor *),CombatBehaviorTree::CombatBehaviorAttacker,CombatBehaviorTree::CombatBehaviorTarget>>>
		static RE::CombatBehaviorTree::TreeBuilder* AddRandomNode(RE::CombatBehaviorTree::TreeBuilder* a_out, const char* a_name, const Function& a_chance, RE::CombatBehaviorTreeNode* a_node)
		{
			using func_t = decltype(&AddRandomNode);
			static REL::Relocation<func_t> func{ REL::ID(47845) };
			return func(a_out, a_name, a_chance, a_node);
		}

		static void thunk(RE::CombatBehaviorTree::TreeBuilder* a_array, RE::CombatBehaviorTreeNode* a_node);
	};
}