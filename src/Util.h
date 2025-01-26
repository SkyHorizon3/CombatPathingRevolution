#pragma once
#include "RE/CombatBehaviorTreeNode.h"

namespace CombatPathing
{
	using namespace RE;

	bool IsMeleeOnly(RE::Actor* a_actor);

	float GetEquippementRange(CombatInventory* a_inv, bool a_full = false);

	std::optional<float> GetGameSettingFloat(const std::string a_name);

	void SetGameSettingFloat(const std::string a_name, float a_value);

	const float RescaleValue(float a_mult, float a_min, float a_max);

	NodeArray& wrap_to_valuenode_13(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node);

	NodeArray& wrap_to_conditional_2(NodeArray& a, const char* name, TreeCtors_extradata* extradata, CombatBehaviorTreeNode* node);

	NodeArray& init_withNode_withname(NodeArray& array, const char* name, CombatBehaviorTreeNode* node);

	NodeArray& pushback_parentof(NodeArray& array, NodeArray& cont_node);

	Character* CombatAI__get_he();

	Character* CombatAI__get_me();

	inline const float GetBoundRadius(RE::Actor* a_this)
	{
		using func_t = decltype(&GetBoundRadius);
		static REL::Relocation<func_t> func{ RELOCATION_ID(36444, 37439) };
		return func(a_this);
	}

	/*
	constexpr static const char* rNode = "RandomNode - %s";
	constexpr static const char* cNode = "ConditionalNode - %s";

	struct TrampolineCall : Xbyak::CodeGenerator
	{
		TrampolineCall(std::uintptr_t retn, std::uintptr_t func)
		{
			Xbyak::Label retnLabel;
			Xbyak::Label logMessage;
			Xbyak::Label funcLabel;
			Xbyak::Label endLabel;

			push(rcx);
			push(rax);

			push(rdx);
			mov(rdx, ptr[r8]);
			mov(rcx, rbp);

			sub(rsp, 0x20);
			call(ptr[rip + funcLabel]);
			add(rsp, 0x20);

			pop(rdx);

			test(rax, rax);  //(0 = false, !=0 = true)

			Xbyak::Label iffalse;
			je(iffalse);  // if same
			lea(edx, ptr[r9 + 0x38]);

			jmp(endLabel);

			L(iffalse);                // if not the same
			lea(edx, ptr[r9 + 0x48]);  // original code

			L(endLabel);

			//push(rbp);
			//mov(rbp, r8);
			xor_(r8d, r8d);  // do original code

			pop(rax);
			pop(rcx);

			jmp(ptr[rip + retnLabel]);

			L(funcLabel);
			dq(func);

			L(retnLabel);
			dq(retn);
		}
	};

	*/

	inline static std::uint8_t Modified = 0;
	inline static void* extra = nullptr;

	struct PrepareFunc00
	{
		static RE::NodeArray& thunk(RE::NodeArray& combatArray, const char* name, void* extraData, RE::CombatBehaviorTreeNode* node)
		{
			Modified = std::strcmp(name, "CPR Circle") == 0;

			SKSE::log::info("log: {}", name);

			if (Modified == 1) {
				SKSE::log::info(" == 1");
				extra = extraData;
				return func(combatArray, name, extraData, node);
			}

			return func(combatArray, name, extraData, node);
		};
		static inline REL::Relocation<decltype(thunk)> func;

		static void InstallHook()
		{
			SKSE::AllocTrampoline(35);

			REL::Relocation<std::uintptr_t> target1{ REL::ID(47845) };

			struct Patch : Xbyak::CodeGenerator
			{
				Patch(std::uintptr_t a_originalFuncAddr, std::size_t a_originalByteLength)
				{
					// Hook returns here. Execute the restored bytes and jump back to the original function.
					for (size_t i = 0; i < a_originalByteLength; ++i) {
						db(*reinterpret_cast<std::uint8_t*>(a_originalFuncAddr + i));
					}

					jmp(qword[rip]);
					dq(a_originalFuncAddr + a_originalByteLength);
				}
			};

			Patch p(target1.address(), 7);
			p.ready();

			auto& trampoline = SKSE::GetTrampoline();
			trampoline.write_branch<5>(target1.address(), thunk);

			auto alloc = trampoline.allocate(p.getSize());
			std::memcpy(alloc, p.getCode(), p.getSize());

			func = reinterpret_cast<std::uintptr_t>(alloc);

			INFO("{} Done!", __FUNCTION__);
		}
	};

	struct PrepareFunc01
	{
		static void* thunk(RE::MemoryManager* manager, std::size_t a_size, std::int32_t a_alignment, bool a_alignmentRequired)
		{
			if (Modified == 1) {
				SKSE::log::info("Allocate 0x38 instead of: {}", a_size);

				return func(manager, 0x38, a_alignment, a_alignmentRequired);
			}

			return func(manager, a_size, a_alignment, a_alignmentRequired);
		};
		static inline REL::Relocation<decltype(thunk)> func;

		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);

			REL::Relocation<std::uintptr_t> target1{ REL::ID(47845) };
			auto& trampoline = SKSE::GetTrampoline();
			func = trampoline.write_call<5>(target1.address() + 0x6F, thunk);

			INFO("{} Done!", __FUNCTION__);
		}
	};

	class PrepareFunc02 : public Xbyak::CodeGenerator
	{
	public:
		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn01, std::uintptr_t log)
			{
				Xbyak::Label retnLabel01;
				Xbyak::Label retnLabel02;
				Xbyak::Label originalFunc;
				Xbyak::Label logMessage;
				Xbyak::Label vtable;
				Xbyak::Label oldVtable;

				sub(rsp, 0x20);
				call(ptr[rip + originalFunc]);
				add(rsp, 0x20);

				push(rax);

				mov(rax, (std::uintptr_t)&Modified);
				cmp(byte[rax], 0);

				pop(rax);

				Xbyak::Label iffalse;
				je(iffalse);  // if same Modified == 0
				mov(rcx, reinterpret_cast<std::uintptr_t>("Strings sind gleich!"));
				sub(rsp, 0x20);
				call(ptr[rip + logMessage]);
				add(rsp, 0x20);

				lea(rax, ptr[rip + vtable]);
				mov(ptr[rbx], rax);
				mov(rcx, (std::uintptr_t)extra);
				mov(ptr[rbx + 0x28], rcx);
				mov(byte[rbx + 0x30], 0x1);

				jmp(ptr[rip + retnLabel02]);

				L(iffalse);  // if not the same
				mov(rcx, reinterpret_cast<std::uintptr_t>("Strings sind NICHT gleich!"));
				sub(rsp, 0x20);
				call(ptr[rip + logMessage]);
				add(rsp, 0x20);

				/*
				lea(rax, ptr[rip + oldVtable]);
				mov(ptr[rbx], rax);
				movups(xmm0, ptr[rsp + 0x20]);
				movups(ptr[rbx + 0x28], xmm0);
				movsd(xmm1, ptr[rsp + 0x30]);
				movsd(ptr[rbx + 0x38], xmm1);
				mov(byte[rbx + 0x40], 0x1);
				*/

				jmp(ptr[rip + retnLabel01]);

				L(originalFunc);
				dq(REL::ID(47545).address());

				L(logMessage);
				dq(log);

				L(vtable);
				dq(REL::VariantID(266106, 212791, 0x1719760).address());

				L(oldVtable);
				dq(REL::ID(212610).address());

				L(retnLabel01);
				dq(retn01 + 0x5);

				L(retnLabel02);
				dq(retn01 + 0x2B);
			}
		};

		static void log(const char* message)
		{
			SKSE::log::info("log: {}", message);
		}

		static void InstallHook()
		{
			constexpr auto targetAddress = REL::ID(47845);

			REL::safe_fill(targetAddress.address() + 0x87, REL::NOP, 0x5);

			REL::Relocation<std::uintptr_t> target{ targetAddress, 0x87 };
			auto trampolineJmp = TrampolineCall(target.address(), reinterpret_cast<std::uintptr_t>(log));

			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(trampolineJmp.getSize());
			auto result = trampoline.allocate(trampolineJmp);
			SKSE::AllocTrampoline(14);
			trampoline.write_branch<5>(target.address(), (std::uintptr_t)result);

			INFO("{} Done!", __FUNCTION__);

			//REL::safe_write(target.address(), REL::NOP5, sizeof(REL::NOP5));

			//RE::VTABLE_CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorContextCloseMovement_bool__CombatBehaviorContextCloseMovement_____void____[0].address();
		}
	};

	class PrepareFunc03  //: public Xbyak::CodeGenerator
	{
	public:
		/*
		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn, bool* modified, std::uintptr_t log)
			{
				Xbyak::Label retnLabel01;
				Xbyak::Label logMessage;
				Xbyak::Label endLabel;
				Xbyak::Label randomNode;
				Xbyak::Label conditionalNode;

				push(rax);

				mov(rax, reinterpret_cast<std::uintptr_t>(modified));  // Zeiger in rax
				movzx(rax, byte[rax]);                                 // Dereferenzierung: Wert von modified in rax
				test(rax, rax);                                        // test, ob der Wert 0 oder ungleich 0 ist  //(0 = false, !=0 = true)

				pop(rax);

				Xbyak::Label iffalse;
				je(iffalse);  // if same
				lea(r8, ptr[rip + conditionalNode]);

				//mov(rcx, r8);
				//sub(rsp, 0x20);
				//call(ptr[rip + logMessage]);
				//add(rsp, 0x20);

				jmp(endLabel);

				L(iffalse);  // if not the same
				lea(r8, ptr[rip + randomNode]);

				L(endLabel);

				jmp(ptr[rip + retnLabel01]);

				L(logMessage);
				dq(log);

				L(randomNode);
				dq(REL::ID(205305).address());

				L(conditionalNode);
				dq(REL::ID(212315).address());

				L(retnLabel01);
				dq(retn);
			}
		};
		*/

		static std::int32_t thunk(char* Buffer, size_t BufferCount, const char* Format, const char* arg)
		{
			SKSE::log::info("BufferCount: {}", BufferCount);
			SKSE::log::info("Format: {}", Format);
			SKSE::log::info("arg: {}", arg);

			if (Modified == 1) {
				//constexpr static const char* node = "ConditionalNode - %s";

				return func(Buffer, BufferCount, "ConditionalNode - %s", arg);
			}

			auto result = func(Buffer, BufferCount, Format, arg);

			SKSE::log::info("Result: {}", result);

			return result;
		};
		static inline REL::Relocation<decltype(thunk)> func;

		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);

			REL::Relocation<std::uintptr_t> target1{ REL::ID(47845) };
			auto& trampoline = SKSE::GetTrampoline();
			func = trampoline.write_call<5>(target1.address() + 0xC6, thunk);

			INFO("{} Done!", __FUNCTION__);
		}
	};

	struct TestHook
	{
#pragma pack(push, 1)
		struct SrcAssembly
		{
			// lea r64, [rip + disp]
			std::uint8_t rex;     // 0 - REX.W
			std::uint8_t opcode;  // 1 - 0x8D
			std::uint8_t modrm;   // 2 - mod = 0b00, reg = r64, rm = 0b101
			std::int32_t disp;    // 3
		};
		static_assert(offsetof(SrcAssembly, rex) == 0x0);
		static_assert(offsetof(SrcAssembly, opcode) == 0x1);
		static_assert(offsetof(SrcAssembly, modrm) == 0x2);
		static_assert(offsetof(SrcAssembly, disp) == 0x3);
		static_assert(sizeof(SrcAssembly) == 0x7);
#pragma pack(pop)

		struct MoveParent : Xbyak::CodeGenerator
		{
		};

		static void Install()
		{
			constexpr auto targetAddress = REL::ID(47845);

			REL::Relocation<std::uintptr_t> swapTarget{ targetAddress, 0x8C };
			REL::safe_fill(swapTarget.address(), REL::NOP, 0x7);

			const auto var = REL::VariantID(265988, 212610, 0x1717fe0).address();
			const auto rip = swapTarget.address() + sizeof(SrcAssembly);
			const auto disp = reinterpret_cast<const std::byte*>(var) -
			                  reinterpret_cast<const std::byte*>(rip);

			//INFO("DISP: 0x{:X}", disp);

			SrcAssembly mem{
				.rex = 0x48,                              // REX.W Prefix
				.opcode = 0x8D,                           // Opcode für LEA
				.modrm = 0x05,                            // ModR/M: mod = 0b00, reg = 0b000 (RAX), rm = 0b101
				.disp = static_cast<std::int32_t>(disp),  // Displacement
			};

			REL::safe_write(swapTarget.address(), &mem, sizeof(SrcAssembly));
		}
	};

}