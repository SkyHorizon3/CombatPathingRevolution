#include "Settings.h"

namespace CombatPathing
{
	void CPRSettings::LoadSettings()
	{
		constexpr std::string_view header = "GameSettings";
		constexpr std::string_view fCombatFallbackChanceMin = "fCombatFallbackChanceMin";
		constexpr std::string_view fCombatFallbackChanceMax = "fCombatFallbackChanceMax";
		constexpr std::string_view fCombatCircleChanceMin = "fCombatCircleChanceMin";
		constexpr std::string_view fCombatCircleChanceMax = "fCombatCircleChanceMax";
		constexpr std::string_view fCombatCircleAnglePlayerMult = "fCombatCircleAnglePlayerMult";

		REX::TIniSetting<float> fallbackChanceMin{ header, fCombatFallbackChanceMin, 0.0f };
		REX::TIniSetting<float> fallbackChanceMax{ header, fCombatFallbackChanceMax, 0.75f };
		REX::TIniSetting<float> circleChanceMin{ header, fCombatCircleChanceMin, 0.15f };
		REX::TIniSetting<float> circleChanceMax{ header, fCombatCircleChanceMax, 0.95f };
		REX::TIniSetting<float> circleAnglePlayerMult{ header, fCombatCircleAnglePlayerMult, 1.0f };

		const auto store = REX::FIniSettingStore::GetSingleton();
		store->Init(path.data(), "");

		store->Load();
		store->Save();

		const auto setValue = [&](const std::string_view name, const float value) {
			auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(name.data());
			if (setting) {
				setting->data.f = value;

				if (enableDebugLog.GetValue()) {
					REX::INFO("Setting:\"{}\" is {}", name, value);
				}
			}
		};

		setValue(fCombatFallbackChanceMin, fallbackChanceMin.GetValue());
		setValue(fCombatFallbackChanceMax, fallbackChanceMax.GetValue());
		setValue(fCombatCircleChanceMin, circleChanceMin.GetValue());
		setValue(fCombatCircleChanceMax, circleChanceMax.GetValue());
		setValue(fCombatCircleAnglePlayerMult, circleAnglePlayerMult.GetValue());

		REX::INFO("{} Done!", __FUNCTION__);
	}
}
