#include "CPRHandler.h"
#include "Constant.h"
#include "Util.h"

void CPRHandler::process(RE::Actor* actor, const std::vector<std::string_view>& tokens, const FUNCTION func)
{
	switch (func) {
	case FUNCTION::EnableAdvance:
		enableAdvance(actor, tokens);
		break;
	case FUNCTION::EnableBackoff:
		enableBackoff(actor, tokens);
		break;
	case FUNCTION::EnableCircling:
		enableCircling(actor, tokens);
		break;
	case FUNCTION::EnableSurround:
		enableSurround(actor, tokens);
		break;
	case FUNCTION::EnableFallback:
		enableFallback(actor, tokens);
		break;
	case FUNCTION::DisableAll:
		disableAll(actor);
		break;
	}
}

static void SetCPRVariables(RE::Actor* a_actor, const std::string& actionName, std::span<const char* const> paramNames, const std::vector<std::string_view>& v)
{
	REX::DEBUG("Enable {} in actor :{}-{:x}", actionName, a_actor->GetName(), a_actor->GetFormID());
	a_actor->SetGraphVariableBool(actionName, true);

	for (size_t i = 0; i < paramNames.size(); i++) {
		const auto argIndex = i + 1;
		float value;
		if (argIndex < v.size() && CombatPathing::to_float(v.at(argIndex), value)) {
			a_actor->SetGraphVariableFloat(paramNames[i], value);
		} else {
			REX::DEBUG("Fail to parse argument \"{}\" for {} in actor :{}-{:x}", paramNames[i], actionName, a_actor->GetName(), a_actor->GetFormID());
			return;
		}
	}
}

void CPRHandler::enableAdvance(RE::Actor* a_actor, const std::vector<std::string_view>& v)
{
	static constexpr std::array paramName = {
		INNER_MIN_GV,
		INNER_MID_GV,
		INNER_MAX_GV,
		OUTER_MIN_GV,
		OUTER_MID_GV,
		OUTER_MAX_GV
	};

	SetCPRVariables(a_actor, ENABLE_RADIUS_GV, paramName, v);

	if (InterruptActiveAction<RE::NodeCloseMovementAdvance>(a_actor)) {
		REX::DEBUG("Interrupt NodeCloseMovementAdvance in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
}

void CPRHandler::enableBackoff(RE::Actor* a_actor, const std::vector<std::string_view>& v)
{
	static constexpr std::array paramNames = {
		BACKOFF_MULT_GV,
		BACKOFF_CHANCE_GV
	};

	SetCPRVariables(a_actor, ENABLE_BACKOFF_GV, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementBackoff>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementBackoff in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::enableCircling(RE::Actor* a_actor, const std::vector<std::string_view>& v)
{
	static constexpr std::array paramNames = {
		CIRCLING_MIN_DIST_GV,
		CIRCLING_MAX_DIST_GV,
		CIRCLING_MIN_ANG_GV,
		CIRCLING_MAX_ANG_GV,
		CIRCLING_VIEW_ANG_GV
	};

	SetCPRVariables(a_actor, ENABLE_CIRCLING_GV, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementCircle>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementCircle in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::enableSurround(RE::Actor* a_actor, const std::vector<std::string_view>& v)
{
	static constexpr std::array paramNames = {
		SURROUND_DIST_MIN_GV,
		SURROUND_DIST_MAX_GV
	};

	SetCPRVariables(a_actor, ENABLE_SURROUND_GV, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementSurround>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementSurround in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::enableFallback(RE::Actor* a_actor, const std::vector<std::string_view>& v)
{
	static constexpr std::array paramNames = {
		FALLBACK_DIST_MIN_GV,
		FALLBACK_DIST_MAX_GV,
		FALLBACK_TIME_MIN_GV,
		FALLBACK_TIME_MAX_GV,
	};

	SetCPRVariables(a_actor, ENABLE_FALLBACK_GV, paramNames, v);
	/*
	if (InterruptActiveAction<RE::NodeCloseMovementFallback>(a_actor)) {
		DEBUG("Interrupt NodeCloseMovementFallback in actor :{}-{:x}", a_actor->GetName(), a_actor->GetFormID());
	}
	*/
}

void CPRHandler::disableAll(RE::Actor* a_actor)
{
	REX::DEBUG("CPR:DisableAdvance for {} - {:x}", a_actor->GetName(), a_actor->formID);
	a_actor->SetGraphVariableBool("CPR_EnableAdvanceRadius", false);

	REX::DEBUG("CPR:DisableBackoff for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Backoff data.
	a_actor->SetGraphVariableBool("CPR_EnableBackoff", false);

	REX::DEBUG("CPR:DisableCircling for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Circling data.
	a_actor->SetGraphVariableBool("CPR_EnableCircling", false);

	REX::DEBUG("CPR:DisableSurround for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Surround data.
	a_actor->SetGraphVariableBool("CPR_EnableSurround", false);

	REX::DEBUG("CPR:DisableFallback for {} - {:x}", a_actor->GetName(), a_actor->formID);
	//Enable data override on vanilla Fallback data.
	a_actor->SetGraphVariableBool("CPR_EnableFallback", false);
}
