#include "LoadGame.h"

DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;

	v.PluginName(Plugin::NAME);
	v.AuthorName(Plugin::AUTHOR);
	v.PluginVersion(Plugin::Version);
	v.UsesAddressLibrary();
	v.UsesNoStructs();
	return v;
}();

DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;

	return true;
}

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	/*
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {
		Sleep(100);
	}
#endif
	*/

	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	REL::Module::reset();
	SKSE::Init(a_skse, false);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	// do stuff
	auto g_message = SKSE::GetMessagingInterface();
	if (!g_message) {
		ERROR("Messaging Interface Not Found!");
		return false;
	}

	g_message->RegisterListener(CombatPathing::EventCallback);

	return true;
}
