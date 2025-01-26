#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class FallbackDistanceHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(46712, 47908) };  // 7D73D0, 8149B0
			_GetFallbackDistance = trampoline.write_call<5>(Base.address() + REL::Relocate(0x1EB, 0x1B0), GetFallbackDistance);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetFallbackDistance(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackDistance)> _GetFallbackDistance;
	};

	class FallbackDistanceHook2
	{
		static float GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor* a_he);

		// 1-5-97-0 @ 0x7D7740
		static constexpr std::uintptr_t SE_FuncID = 46713;   //1407d7740
		static constexpr std::ptrdiff_t SE_OffsetL = 0x246;  //1407D7A00
		static constexpr std::ptrdiff_t SE_OffsetH = 0x24E;  //1407D7A08

		static constexpr std::uintptr_t AE_FuncID = 47909;  //140814CD0
		static constexpr std::ptrdiff_t AE_OffsetL = 0x2E7;
		static constexpr std::ptrdiff_t AE_OffsetH = 0x2EF;

		static constexpr Patch RelocateReturn{
			// addss xmm6, xmm0
			"\xF3\x0F\x58\xF0",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::RelocationID(SE_FuncID, AE_FuncID).address();
			Patch RelocatePointer{
				AsPointer(funcAddr + REL::Relocate(SE_OffsetL, AE_OffsetL) + 0x10),  //7D7996
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				REL::Relocate(std::make_pair(SE_OffsetL, SE_OffsetH), std::make_pair(AE_OffsetL, AE_OffsetH)),
				FUNC_INFO(GetMaxFallbackDistance),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};

	class FallbackWaitTimeHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(46713, 47909) };  //sub_1407D7740
			_GetFallbackWaitTime = trampoline.write_call<5>(Base.address() + REL::Relocate(0xEA, 0x12E), GetFallbackWaitTime);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetFallbackWaitTime(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackWaitTime)> _GetFallbackWaitTime;
	};

	class FallbackWaitTimeHook2
	{
		static float GetMinFallbackWaitTime(RE::Actor* a_me, RE::Actor* a_he);

		static constexpr std::uintptr_t AE_FuncID = 47909;  //140814CD0
		static constexpr std::ptrdiff_t AE_OffsetL = 0x362;
		static constexpr std::ptrdiff_t AE_OffsetH = 0x36A;

		static constexpr std::uintptr_t SE_FuncID = 46713;   //1407d7740
		static constexpr std::ptrdiff_t SE_OffsetL = 0x2C0;  //1407D7A00
		static constexpr std::ptrdiff_t SE_OffsetH = 0x2C8;  //1407D7A08

		static constexpr Patch RelocateReturn{
			// movss xmm1, xmm0
			"\xF3\x0F\x10\xC8",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::RelocationID(SE_FuncID, AE_FuncID).address();
			Patch RelocatePointer{
				AsPointer(funcAddr + REL::Relocate(0x256, 0x2F7)),  //7D7996
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				REL::Relocate(std::make_pair(SE_OffsetL, SE_OffsetH), std::make_pair(AE_OffsetL, AE_OffsetH)),
				FUNC_INFO(GetMinFallbackWaitTime),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};
}