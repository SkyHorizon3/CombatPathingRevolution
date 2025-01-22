#pragma once
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace RE;

	bool IsMeleeOnly(RE::Actor* a_actor);

	float GetEquippementRange(CombatInventory* a_inv, bool a_full = false);

	std::optional<float> GetGameSettingFloat(const std::string a_name);

	void SetGameSettingFloat(const std::string a_name, float a_value);

	const float RescaleValue(float a_mult, float a_min, float a_max);

	NodeArray& wrap_to_valuenode_13(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node);

	NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node);

	NodeArray& init_withNode_withname(NodeArray& array, const char* name, CombatBehaviorTreeNode* node);

	NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node);

	Character* CombatAI__get_he();

	Character* CombatAI__get_me();

	inline const float GetBoundRadius(RE::Actor* a_this)
	{
		using func_t = decltype(&GetBoundRadius);
		static REL::Relocation<func_t> func{ RELOCATION_ID(36444, 37439) };
		return func(a_this);
	}

	class PrepareFunc01 : public Xbyak::CodeGenerator
	{
	public:
		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn)  //, std::uintptr_t func
			{
				//Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				//sub(rsp, 0x20);              //allocate for call
				//call(ptr[rip + funcLabel]);  //call thunk
				//add(rsp, 0x20);

				xor_(r8d, r8d);  // original code
				lea(edx, ptr[r9 + 0x48]);

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				//L(funcLabel);
				//dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

		static void InstallHook()
		{
			constexpr auto targetAddress = REL::ID(47845);

			REL::safe_fill(targetAddress.address() + 0x61, REL::NOP, 0x7);

			REL::Relocation<std::uintptr_t> target{ targetAddress, 0x61 };
			auto trampolineJmp = TrampolineCall(target.address() + 0x7);  // reinterpret_cast<std::uintptr_t>(thunk)

			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(trampolineJmp.getSize());
			auto result = trampoline.allocate(trampolineJmp);
			SKSE::AllocTrampoline(14);
			trampoline.write_branch<5>(target.address(), (std::uintptr_t)result);

			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static void thunk()
		{
		}
	};
	// Goal: changing sub_140811F90 so we can call it with another parameter to switch to another usage
}