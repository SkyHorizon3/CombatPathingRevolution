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

	NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, void* extradata, CombatBehaviorTreeNode* node);

	NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node);

	Character* CombatAI__get_he();

	Character* CombatAI__get_me();

	inline const float GetBoundRadius(RE::Actor* a_this)
	{
		using func_t = decltype(&GetBoundRadius);
		static REL::Relocation<func_t> func{ RELOCATION_ID(36444, 37439) };
		return func(a_this);
	}
}