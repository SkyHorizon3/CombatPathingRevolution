#pragma once

namespace CombatPathing
{
	void splitSV(std::vector<std::string_view>& ret, std::string_view strv, char delim);
	bool to_float(std::string_view input, float& out);
	bool IsMeleeOnly(RE::Actor* a_actor);
	float GetEquippementRange(RE::CombatInventory* a_inv, bool a_full = false);
	float RescaleValue(const float a_mult, const float a_min, const float a_max);
	RE::CombatBehaviorTree::TreeBuilder* wrap_to_conditional_2(RE::CombatBehaviorTree::TreeBuilder* a, const char* name, void* extradata, CombatBehaviorTreeNode* node);
}