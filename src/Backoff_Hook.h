#pragma once

#include "DKUtil/Hook.hpp"
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class BackoffStartHook
	{
		static float RescaleBackoffMinDistanceMult(RE::Actor* a_me, RE::Actor* a_he);

		// 1-6-640-0 @ 0x816350
		static constexpr std::uintptr_t AE_FuncID = 47920;   // 140816350
		static constexpr std::ptrdiff_t AE_OffsetL = 0x1BE;  // 14081650E
		static constexpr std::ptrdiff_t AE_OffsetH = 0x1C6;  // 140816516
		// 1-5-97-0 @ 0x7D8C90
		static constexpr std::uintptr_t SE_FuncID = 46724;   // 1407D8C90
		static constexpr std::ptrdiff_t SE_OffsetL = 0x201;  // 1407D8E91
		static constexpr std::ptrdiff_t SE_OffsetH = 0x209;  // 1407D8E99

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::RelocationID(SE_FuncID, AE_FuncID).address();

			Patch RelocatePointer{
				AsPointer(funcAddr + REL::Relocate(0x1B1, 0x173)),  //SE: 1407D8E41, AE: 1408164C3
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ AE_OffsetL, AE_OffsetH },
				FUNC_INFO(RescaleBackoffMinDistanceMult),
				&RelocatePointer,
				nullptr);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};

	class BackoffChanceHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(46731, 47928) };                                      // SE: 1407D97D0, AE: 140816E40
			_WrapToRandomNode = trampoline.write_call<5>(Base.address() + REL::Relocate(0xD7, 0x1F0), WrapToRandomNode);  // AE difficult
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static RE::NodeArray& WrapToRandomNode(RE::NodeArray& a_array, const char* a_name, RE::TreeCtors_extradata* a_extradata, RE::CombatBehaviorTreeNode* a_node);

		static inline REL::Relocation<decltype(WrapToRandomNode)> _WrapToRandomNode;
	};

	class BackoffChanceHookAE : public Xbyak::CodeGenerator
	{
	public:
		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;
				//Xbyak::Label originalFunc;

				//sub(rsp, 0x20);
				//call(ptr[rip + originalFunc]);  //call original function
				//add(rsp, 0x20);

				push(rcx);
				push(rdx);
				//push(r8);
				//push(r9);

				//mov(r8, rdi);               //RE::TreeCtors_extradata* a_extradata
				mov(rdx, rbx);              // RE::CombatBehaviorTreeNode* a_node
				mov(rcx, ptr[rbp - 0x28]);  // RE::NodeArray& a_array

				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);  //call thunk
				add(rsp, 0x20);

				//mov(ptr[rbp - 0x28], rax);

				//pop(r9);
				//pop(r8);
				pop(rdx);
				pop(rcx);

				jmp(ptr[rip + retnLabel]);  //jump back to original code

				//L(originalFunc);
				//dq(REL::ID(47516).address());

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

		static void InstallHook()
		{
			constexpr auto targetAddress = REL::ID(47928);
			// +0x10F -> 140817030

			REL::safe_fill(targetAddress.address() + 0x10F, REL::NOP, 0x7);

			REL::Relocation<std::uintptr_t> target{ targetAddress, 0x10F };
			auto trampolineJmp = TrampolineCall(target.address() + 0xE6, reinterpret_cast<std::uintptr_t>(thunk));

			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(trampolineJmp.getSize());
			auto result = trampoline.allocate(trampolineJmp);
			SKSE::AllocTrampoline(14);
			trampoline.write_branch<5>(target.address(), (std::uintptr_t)result);

			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static RE::NodeArray& WrapToRandomNode(RE::NodeArray& a_array, const char* a_name, RE::TreeCtors_extradata* a_extradata, RE::CombatBehaviorTreeNode* a_node)
		{
			using func_t = decltype(&WrapToRandomNode);
			static REL::Relocation<func_t> func{ REL::ID(47845) };
			return func(a_array, a_name, a_extradata, a_node);
		}

		static void thunk(RE::NodeArray& a_array, RE::CombatBehaviorTreeNode* a_node);
	};

}