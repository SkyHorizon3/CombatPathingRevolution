#pragma once

namespace CombatPathing
{
	bool IsMeleeOnly(RE::Actor* a_actor);

	float GetEquippementRange(CombatInventory* a_inv, bool a_full = false);

	std::optional<float> GetGameSettingFloat(const std::string a_name);

	void SetGameSettingFloat(const std::string a_name, float a_value);

	const float RescaleValue(float a_mult, float a_min, float a_max);

	NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, void* extradata, CombatBehaviorTreeNode* node);

	NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node);
}