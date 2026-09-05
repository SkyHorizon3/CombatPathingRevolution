#pragma once

class CPRHandler
{
public:
	enum class FUNCTION : std::uint8_t
	{
		EnableAdvance,
		EnableBackoff,
		EnableCircling,
		EnableSurround,
		EnableFallback,
		DisableAll
	};

	static void process(RE::Actor* actor, const std::vector<std::string_view>& v, const FUNCTION f);

private:
	static void enableAdvance(RE::Actor* actor, const std::vector<std::string_view>& v);
	static void enableBackoff(RE::Actor* actor, const std::vector<std::string_view>& v);
	static void enableCircling(RE::Actor* actor, const std::vector<std::string_view>& v);
	static void enableSurround(RE::Actor* actor, const std::vector<std::string_view>& v);
	static void enableFallback(RE::Actor* actor, const std::vector<std::string_view>& v);
	static void disableAll(RE::Actor* actor);

	template <class T>
	static bool InterruptActiveAction(RE::Actor* a_actor)
	{
		using NodeState = RE::CombatBehaviorThread::State;

		if (a_actor) {
			auto combatCtrl = a_actor->combatController;
			auto behaviorCtrl = combatCtrl ? combatCtrl->behaviorController : nullptr;
			if (behaviorCtrl) {
				for (auto nodeCtrl : behaviorCtrl->activeThreads) {
					if (nodeCtrl && nodeCtrl->currentNode && nodeCtrl->state == NodeState::kUpdating) {
						auto activeNode = skyrim_cast<const T*>(nodeCtrl->currentNode);
						if (activeNode) {
							return a_actor->SetGraphVariableBool("CPR_InterruptAction", true);
						}
					}
				}
			}
		}

		return false;
	}
};
