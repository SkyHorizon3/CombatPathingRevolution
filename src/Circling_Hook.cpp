#include "Circling_Hook.h"
#include "Constant.h"
#include "Util.h"

namespace CombatPathing
{
	static bool WithinCricleRange(RE::Actor* me, RE::Actor* he)
	{
		if (me && he) {
			const auto combatCtr = me->combatController;

			if (combatCtr && combatCtr->combatStyle) {
				bool enableCircling = false;
				if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling) {
					float circlingDistMin{}, circlingDistMax{};
					if (me->GetGraphVariableFloat(CIRCLING_MIN_DIST_GV, circlingDistMin) && me->GetGraphVariableFloat(CIRCLING_MAX_DIST_GV, circlingDistMax)) {
						const auto inv = combatCtr->inventory;
						const auto optimalWeapRange = GetEquippementRange(inv);
						const auto maxWeapRange = GetEquippementRange(inv, true);
						const auto distance = me->GetPosition().GetDistance(he->GetPosition()) - he->GetBoundRadius();
						circlingDistMin += circlingDistMin > 0.f ? optimalWeapRange : 0.f;
						circlingDistMax += maxWeapRange;

						return distance >= circlingDistMin && distance <= circlingDistMax;
					}
				}
			}
		}

		return false;
	}

	float CirclingChanceHook::GetCirclingChance(const float a_circleMult, const float a_minChance, const float a_maxChance)
	{
		auto me = RE::CombatBehaviorTree::GetAttacker();
		auto he = RE::CombatBehaviorTree::GetTarget();

		if (!WithinCricleRange(me, he))
			return std::max(0.1f, a_minChance);  //The chance must be a bit greater than zero, ohterwise NPC would be stucked by barriers.

		return _GetCirclingChance(a_circleMult, a_minChance, a_maxChance);
	}

	RE::CombatBehaviorTree::TreeBuilder* AdvanceToCircleHook::PushBackNode(RE::CombatBehaviorTree::TreeBuilder* a_master, RE::CombatBehaviorTree::TreeBuilder* a_target)
	{
		auto nodeCirlce = RE::CombatBehaviorTree::CreateObject<RE::CombatBehaviorCircle>();
		if (nodeCirlce) {
			RE::CombatBehaviorTree::TreeBuilder array;

			auto arr = wrap_to_conditional_2(&array, "CPR Circle", &ShouldCircle, nodeCirlce);
			a_master = a_master->AppendLastNode(*arr);
		}

		return _PushBackNode(a_master, a_target);
	}

	bool AdvanceToCircleHook::ShouldCircle([[maybe_unused]] void* a_context)
	{
		auto me = RE::CombatBehaviorTree::GetAttacker();
		auto he = RE::CombatBehaviorTree::GetTarget();

		if (me && he) {
			if (WithinCricleRange(me, he)) {
				auto chance = GetCircleChance(me);
				return REX::TRandom<float>().Generate(0.f, 1.0f) <= chance ? true : false;
			}
		}

		return false;
	}

	float CircleAngleHook1::RescaleCircleAngle(const float a_circleMult, const float a_minAnlge, const float a_maxAngle)
	{
		auto me = RE::CombatBehaviorTree::GetAttacker();
		if (me) {
			const auto combatCont = me->combatController;
			if (combatCont && combatCont->combatStyle) {
				bool enableCircling = false;
				if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling && IsMeleeOnly(me)) {
					float circlingAngleMin, circlingAngleMax;
					if (me->GetGraphVariableFloat(CIRCLING_MIN_ANG_GV, circlingAngleMin) && me->GetGraphVariableFloat(CIRCLING_MAX_ANG_GV, circlingAngleMax)) {
						return _RescaleCircleAngle(a_circleMult, circlingAngleMin, circlingAngleMax);
					}
				}
			}
		}

		return _RescaleCircleAngle(a_circleMult, a_minAnlge, a_maxAngle);
	}

	float CircleAngleHook2::GetMinCircleAngle()
	{
		auto me = RE::CombatBehaviorTree::GetAttacker();
		if (me) {
			const auto combatCont = me->combatController;

			if (combatCont && combatCont->combatStyle) {
				bool enableCircling = false;
				if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling && IsMeleeOnly(me)) {
					float circlingAngleMin;
					if (me->GetGraphVariableFloat(CIRCLING_MIN_ANG_GV, circlingAngleMin))
						return circlingAngleMin;
				}
			}
		}

		const auto angleMinSetting = "fCombatCircleAngleMin"_gs;
		if (angleMinSetting.has_value())
			return *angleMinSetting;

		return 30.f;
	}

	float CircleAngleHook3::GetMaxCircleAngle()
	{
		auto me = RE::CombatBehaviorTree::GetAttacker();
		if (me) {
			const auto combatCont = me->combatController;
			if (combatCont && combatCont->combatStyle) {
				bool enableCircling = false;
				if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling && IsMeleeOnly(me)) {
					float circlingAngleMax;
					if (me->GetGraphVariableFloat(CIRCLING_MAX_ANG_GV, circlingAngleMax))
						return circlingAngleMax;
				}
			}
		}

		const auto angleMaxSetting = "fCombatCircleAngleMax"_gs;
		if (angleMaxSetting.has_value())
			return *angleMaxSetting;

		return 90.f;
	}

	bool CircleViewConeHook::WithinHeadingAngle(RE::Actor* he, RE::NiPoint3* a_pos, float a_angle)
	{
		auto me = RE::CombatBehaviorTree::GetAttacker();
		if (me) {
			bool enableCircling = false;
			if (me->GetGraphVariableBool(ENABLE_CIRCLING_GV, enableCircling) && enableCircling && IsMeleeOnly(me)) {
				float circlingViewConeAngle{};
				if (me->GetGraphVariableFloat(CIRCLING_VIEW_ANG_GV, circlingViewConeAngle)) {
					return _WithinHeadingAngle(he, a_pos, circlingViewConeAngle * 0.017453292);
				}
			}
		}

		return _WithinHeadingAngle(he, a_pos, a_angle);
	}

}