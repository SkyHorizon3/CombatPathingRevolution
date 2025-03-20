#include "Backoff_Hook.h"

namespace CombatPathing
{
	static constexpr char ENABLE_BACKOFF_GV[] = "CPR_EnableBackoff",
						  BACKOFF_MULT_GV[] = "CPR_BackoffMinDistMult", BACKOFF_CHANCE_GV[] = "CPR_BackoffChance";

	float BackoffStartHook::RescaleBackoffMinDistanceMult(RE::Actor* a_actor, RE::Actor*)
	{
		bool enablebackoff;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
			float backoffMult;
			if (a_actor->GetGraphVariableFloat(BACKOFF_MULT_GV, backoffMult))
				return backoffMult;
		}

		auto BackoffMinDistMultSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatBackoffMinDistanceMult");
		return BackoffMinDistMultSettings ? BackoffMinDistMultSettings->GetFloat() : 0.75f;
	}

	void BackoffChanceHookAE::thunk(RE::NodeArray& a_array, RE::CombatBehaviorTreeNode* a_node)
	{
		static auto RecalculateBackoffChance = +[](RE::Actor* a_actor) -> float {
			//SKSE::log::info("Hawk tuah run that thing!!!");

			bool enablebackoff;
			if (a_actor && a_actor->GetGraphVariableBool(ENABLE_BACKOFF_GV, enablebackoff) && enablebackoff) {
				float backoffChance;
				if (a_actor->GetGraphVariableFloat(BACKOFF_CHANCE_GV, backoffChance))
					return backoffChance;
			}

			auto BackoffChanceSettings = RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatBackoffChance");
			return BackoffChanceSettings ? BackoffChanceSettings->GetFloat() : 0.25f;
		};

		RE::TreeCtors_extradata extraData;
		extraData.func1 = RecalculateBackoffChance;
		extraData.func2 = nullptr;

		a_array = WrapToRandomNode(a_array, "Backoff", &extraData, a_node);
	}
}
