#include "Advance_Hook.h"
#include "Constant.h"
#include "Util.h"

namespace CombatPathing
{
	float AdvanceRadiusHook::RescaleRadius(float a_delta, float min, float mid, float max)
	{
		return a_delta <= 0.0f ? min + (mid - min) * (a_delta + 1.0f) : mid + (max - mid) * a_delta;
	}

	void AdvanceRadiusHook::RecalculateAdvanceRadius(bool a_fullRadius, float* a_radius, float a_delta, RE::Actor* a_target, RE::Actor* a_attacker)
	{
		if (!a_radius || !a_target || !IsMeleeOnly(a_attacker))
			return;

		bool enableAdvanceRadius = false;
		if (a_attacker->GetGraphVariableBool(ENABLE_RADIUS_GV, enableAdvanceRadius) && enableAdvanceRadius) {
			float InnerMin{}, InnerMid{}, InnerMax{}, OuterMin{}, OuterMid{}, OuterMax{};
			if (a_attacker->GetGraphVariableFloat(INNER_MIN_GV, InnerMin) && a_attacker->GetGraphVariableFloat(INNER_MID_GV, InnerMid) && a_attacker->GetGraphVariableFloat(INNER_MAX_GV, InnerMax) &&
				a_attacker->GetGraphVariableFloat(OUTER_MIN_GV, OuterMin) && a_attacker->GetGraphVariableFloat(OUTER_MID_GV, OuterMid) && a_attacker->GetGraphVariableFloat(OUTER_MAX_GV, OuterMax)) {
				auto& inner = a_radius[0];
				auto& outer = a_radius[2];

				if (a_fullRadius) {
					inner = RescaleRadius(a_delta, InnerMin, InnerMid, InnerMax);
					outer = RescaleRadius(a_delta, OuterMin, OuterMid, OuterMax);
				} else {
					inner = InnerMid;
					outer = OuterMid;
				}
			}
		}
	}

	void AdvanceInterruptHook::Update(RE::CombatBehaviorAdvance* context)
	{
		auto attacker = RE::CombatBehaviorTree::GetAttacker();
		if (attacker && context) {
			bool enableAdvanceRadius = false, interruptAction = false;
			if (attacker->GetGraphVariableBool(INTERRUPT_ACTION_GV, interruptAction) && interruptAction && attacker->GetGraphVariableBool(ENABLE_RADIUS_GV, enableAdvanceRadius) && enableAdvanceRadius) {
				auto combatPath = context->path.get();
				if (combatPath) {
					combatPath->state = RE::CombatPath::STATE::kFailed;
				}
			}

			if (interruptAction) {
				attacker->SetGraphVariableBool(INTERRUPT_ACTION_GV, false);
			}
		}

		_Update(context);
	}

}