#include "Backoff_Hook.h"

namespace CombatPathing
{
	static constexpr char ENABLE_BACKOFF_GV[] = "CPR_EnableBackoff",
						  BACKOFF_MULT_GV[] = "CPR_BackoffMinDistMult", BACKOFF_CHANCE_GV[] = "CPR_BackoffChance";

	float BackoffStartHook::RescaleBackoffMinDistanceMult(RE::Actor* a_actor, RE::Actor*)
	{
		bool enablebackoff = false;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
			float backoffMult;
			if (a_actor->GetGraphVariableFloat(BACKOFF_MULT_GV, backoffMult))
				return backoffMult;
		}

		auto BackoffMinDistMultSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatBackoffMinDistanceMult");
		return BackoffMinDistMultSettings ? BackoffMinDistMultSettings->GetFloat() : 0.75f;
	}

	void BackoffChanceHookAE::thunk(RE::CombatBehaviorTree::TreeBuilder* a_array, RE::CombatBehaviorTreeNode* a_node)
	{
		static auto RecalculateBackoffChance = +[](RE::Actor* a_actor, RE::Actor*) -> float {
			bool enablebackoff = false;
			if (a_actor && a_actor->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
				float backoffChance;
				if (a_actor->GetGraphVariableFloat(BACKOFF_CHANCE_GV, backoffChance))
					return backoffChance;
			}

			auto BackoffChanceSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatBackoffChance");
			return BackoffChanceSettings ? BackoffChanceSettings->GetFloat() : 0.25f;
		};

		Function chanceFunction{ RecalculateBackoffChance };
		a_array = AddRandomNode(a_array, "Backoff", chanceFunction, a_node);
	}
}
