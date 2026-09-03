#include "Fallback_Hook.h"
#include "Constant.h"
#include "Util.h"

namespace CombatPathing
{
	float FallbackDistanceHook1::GetFallbackDistance(RE::Actor* a_actor)
	{
		bool enableFallback = false;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackDistMin, fallbackDistMax;

			if (a_actor->GetGraphVariableFloat(FALLBACK_DIST_MIN_GV, fallbackDistMin) && a_actor->GetGraphVariableFloat(FALLBACK_DIST_MAX_GV, fallbackDistMax)) {
				const auto combatCont = a_actor->combatController;
				const auto combatStyle = combatCont ? combatCont->combatStyle : nullptr;

				if (combatStyle) {
					auto fallbackMult = combatStyle->closeRangeData.fallbackMult;
					auto FallbackDistance = RescaleValue(fallbackMult, fallbackDistMin, fallbackDistMax);
					auto diameter = a_actor->GetBoundRadius() * 2.0f;

					return std::max(FallbackDistance, diameter);
				}
			}
		}

		return _GetFallbackDistance(a_actor);
	}

	float FallbackDistanceHook2::GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor*)
	{
		bool enableFallback = false;
		if (a_me && a_me->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackDistMax;
			if (a_me->GetGraphVariableFloat(FALLBACK_DIST_MAX_GV, fallbackDistMax))
				return fallbackDistMax;
		}

		const auto maxFallbackDistSettings = "fCombatFallbackDistanceMax"_gs;
		if (maxFallbackDistSettings.has_value())
			return maxFallbackDistSettings.value();

		return 256.f;
	}

	float FallbackWaitTimeHook1::GetFallbackWaitTime(RE::Actor* a_actor)
	{
		bool enableFallback = false;
		if (a_actor && a_actor->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackWaitTimeMin, fallbackWaitTimeMax;

			if (a_actor->GetGraphVariableFloat(FALLBACK_TIME_MIN_GV, fallbackWaitTimeMin) && a_actor->GetGraphVariableFloat(FALLBACK_TIME_MAX_GV, fallbackWaitTimeMax)) {
				const auto combatCont = a_actor->combatController;
				const auto combatStyle = combatCont ? combatCont->combatStyle : nullptr;
				if (combatStyle) {
					auto fallbackMult = combatStyle->closeRangeData.fallbackMult;
					return RescaleValue(fallbackMult, fallbackWaitTimeMin, fallbackWaitTimeMax);
				}
			}
		}

		return _GetFallbackWaitTime(a_actor);
	}

	float FallbackWaitTimeHook2::GetMinFallbackWaitTime(RE::Actor* a_me, RE::Actor*)
	{
		bool enableFallback = false;
		if (a_me && a_me->GetGraphVariableBool(ENABLE_FALLBACK_GV, enableFallback) && enableFallback) {
			float fallbackWaitTimeMin;
			if (a_me->GetGraphVariableFloat(FALLBACK_TIME_MIN_GV, fallbackWaitTimeMin))
				return fallbackWaitTimeMin;
		}

		const auto minFallbackTimeSettings = "fCombatFallbackWaitTimeMin"_gs;
		if (minFallbackTimeSettings.has_value())
			return minFallbackTimeSettings.value();

		return 0.75f;
	}

}