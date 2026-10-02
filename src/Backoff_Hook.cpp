#include "Backoff_Hook.h"
#include "Constant.h"

namespace CombatPathing
{
	float BackoffStartHook::RescaleBackoffMinDistanceMult(RE::Actor* a_target, [[maybe_unused]] RE::Actor* a_attacker)  // TODO: verify
	{
		bool enablebackoff = false;
		if (a_target && a_target->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
			float backoffMult;
			if (a_target->GetGraphVariableFloat(BACKOFF_MULT_GV, backoffMult)) {
				return backoffMult;
			}
		}

		const auto multSetting = "fCombatBackoffMinDistanceMult"_gs;
		return multSetting.has_value() ? *multSetting : 0.75f;
	}

	void BackoffChanceHookAE::thunk(RE::CombatBehaviorTree::TreeBuilder* a_array, RE::CombatBehaviorTreeNode* a_node)
	{
		static auto RecalculateBackoffChance = +[](RE::Actor* a_target, [[maybe_unused]] RE::Actor* a_attacker) -> float {
			bool enablebackoff = false;
			if (a_target && a_target->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
				float backoffChance;
				if (a_target->GetGraphVariableFloat(BACKOFF_CHANCE_GV, backoffChance)) {
					return backoffChance;
				}
			}

			const auto chanceSetting = "fCombatBackoffChance"_gs;
			return chanceSetting.has_value() ? *chanceSetting : 0.25f;
		};

		Function chanceFunction{ RecalculateBackoffChance };
		a_array = AddRandomNode(a_array, "Backoff", chanceFunction, a_node);
	}
}
