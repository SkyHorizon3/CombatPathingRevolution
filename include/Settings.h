#pragma once

namespace CombatPathing
{
	class CPRSettings : public REX::TSingleton<CPRSettings>
	{
	public:
		void LoadSettings();

		bool EnableDebugLog() const { return enableDebugLog.GetValue(); }

	private:
		static constexpr auto path = R"(Data\SKSE\Plugins\CombatPathingRevolution.ini)"sv;

		REX::TIniSetting<bool> enableDebugLog{ "Debug"sv, "EnableDebugLog"sv, false };
	};
}