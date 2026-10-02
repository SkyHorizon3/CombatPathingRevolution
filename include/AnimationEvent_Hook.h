#pragma once
#include "CPRHandler.h"
#include "Util.h"

namespace CombatPathing
{
	class hook_animationEvent
	{
	public:
		static void install()
		{
			REL::Relocation<uintptr_t> AnimEventVtbl_NPC{ RE::VTABLE_Character[2] };
			_ProcessEvent_NPC = AnimEventVtbl_NPC.write_vfunc(0x1, ProcessEvent_NPC);

			REX::INFO("{} Done!", __FUNCTION__);
		}

	private:
		static RE::BSEventNotifyControl ProcessEvent_NPC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
		{
			ProcessEvent(a_sink, a_event, a_eventSource);
			return _ProcessEvent_NPC(a_sink, a_event, a_eventSource);
		}

		static void ProcessEvent([[maybe_unused]] RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
		{
			if (a_event->tag != "CPR") {
				return;
			}

			auto holder = const_cast<RE::TESObjectREFR*>(a_event->holder);
			if (!holder) {
				return;
			}
			const auto asActor = holder->As<RE::Actor>();
			if (asActor) {
				const auto payload = std::string(a_event->payload.c_str());
				delegateNative(asActor, payload);
			}
		}

		static void delegateNative(RE::Actor* actor, const std::string& a_payload)
		{
			REX::DEBUG("CPR instruction triggered for {}-{:x}; instruction: {}", actor->GetName(), actor->GetFormID(), a_payload);

			const auto tokens = REX::STR::SPLIT(a_payload, "|");
			switch (REX::STR::CONST_HASH(tokens[0])) {
			case "EnableAdvance"_h:  // CPR.EnableAdvance|111|222|333|444|555|666
				CPRHandler::process(actor, tokens, CPRHandler::FUNCTION::EnableAdvance);
				break;
			case "EnableBackoff"_h:  // CPR.EnableBackoff|11|22
				CPRHandler::process(actor, tokens, CPRHandler::FUNCTION::EnableBackoff);
				break;
			case "EnableCircling"_h:  // CPR.EnableCircling|33|44
				CPRHandler::process(actor, tokens, CPRHandler::FUNCTION::EnableCircling);
				break;
			case "EnableSurround"_h:  // CPR.EnableSurround|...
				CPRHandler::process(actor, tokens, CPRHandler::FUNCTION::EnableSurround);
				break;
			case "EnableFallback"_h:  // CPR.EnableFallback|...
				CPRHandler::process(actor, tokens, CPRHandler::FUNCTION::EnableFallback);
				break;
			case "DisableAll"_h:  // CPR.DisableAll
				CPRHandler::process(actor, tokens, CPRHandler::FUNCTION::DisableAll);
				break;
			}
		}

		static inline REL::Relocation<decltype(ProcessEvent_NPC)> _ProcessEvent_NPC;
	};

}
