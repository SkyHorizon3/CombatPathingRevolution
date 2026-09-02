#include "Advance_Hook.h"
#include "Backoff_Hook.h"
#include "Circling_Hook.h"
#include "Fallback_Hook.h"
#include "hooks.h"

namespace CombatPathing
{
	void OnInit(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kPostLoad:
			{
				AdvanceRadiusHook::InstallHook();
				AdvanceInterruptHook::InstallHook();

				BackoffStartHook::InstallHook();
				BackoffChanceHookAE::InstallHook();

				CirclingChanceHook::InstallHook();
				AdvanceToCircleHook::InstallHook();
				CircleAngleHook1::InstallHook();
				CircleAngleHook2::InstallHook();
				CircleAngleHook3::InstallHook();
				CircleViewConeHook::InstallHook();

				FallbackDistanceHook1::InstallHook();
				FallbackDistanceHook2::InstallHook();
				FallbackWaitTimeHook1::InstallHook();
				FallbackWaitTimeHook2::InstallHook();

				Hooks::hook_animationEvent::install();
			}
			break;
		default:
			break;
		}
	}
}

#ifdef SKYRIM_SUPPORT_AE
SKSE_PLUGIN_VERSION = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);
	v.AuthorName("SkyHorizon");
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });

	if constexpr (SKSE::RUNTIME_SSE_LATEST < SKSE::RUNTIME_SSE_1_7_99) {
		v.MinimumRequiredXSEVersion(REL::Version{ 2, 2, 5 });
	} else {
		// address library v5 support
		v.MinimumRequiredXSEVersion(REL::Version{ 2, 3, 0 });
	}

	return v;
}();
#endif

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse, SKSE::InitInfo{
						 .log = true,
						 .logName = Plugin::NAME,
						 //.trampoline = true,
						 // .trampolineSize = 100,
					 });

	// TODO: Add debug log setting
	const auto runtimeVer = skse->RuntimeVersion();
	REX::INFO("Game version: {}", runtimeVer);

#ifdef SKYRIM_SUPPORT_AE
	if constexpr (SKSE::RUNTIME_SSE_LATEST < SKSE::RUNTIME_SSE_1_7_99) {
		if (runtimeVer >= SKSE::RUNTIME_SSE_1_7_99) {
			REX::FAIL(
				"You are using a newer version of Skyrim than this version of {0} supports.\n"
				"Install the correct version of {0} for your game version.\n"
				"Runtime: {1}\n"
				"Supported: 1.6.1170 (Steam) / 1.6.1179 (GOG)",
				Plugin::NAME, runtimeVer);
		}
	}
#endif

	auto messaging = SKSE::GetMessagingInterface();
	messaging->RegisterListener(CombatPathing::OnInit);

	return true;
}