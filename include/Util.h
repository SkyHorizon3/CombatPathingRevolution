#pragma once

namespace RE
{
	class CombatBehaviorTreeConditionalNodeImpl : public CombatBehaviorTreeNode
	{
	public:
		inline static constexpr auto RTTI = RTTI_CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorContextCloseMovement_bool_CombatBehaviorContextCloseMovement____void____;
		inline static constexpr auto VTABLE = VTABLE_CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorContextCloseMovement_bool_CombatBehaviorContextCloseMovement____void____;

		~CombatBehaviorTreeConditionalNodeImpl() override = default;

		void Enter(CombatBehaviorThread* a_thread) override;           // 02
		void Update(CombatBehaviorThread* a_thread) override;          // 04
		void Abort(CombatBehaviorThread* a_thread) override;           // 05
		bool Validate(const CombatBehaviorTreeNode* a_node) override;  // 08
		const BSFixedString& GetType() override;                       // 09

		static CombatBehaviorTreeConditionalNodeImpl* Create()
		{
			auto node = malloc<CombatBehaviorTreeConditionalNodeImpl>();
			if (node) {
				std::memset(node, 0, sizeof(CombatBehaviorTreeConditionalNodeImpl));
				node->Ctor();
				REX::EMPLACE_VTABLE(node);
			}

			return node;
		}

		// members
		void* expr;
		bool isSelector;
	};
	static_assert(sizeof(CombatBehaviorTreeConditionalNodeImpl) == 0x38);

}

namespace CombatPathing
{
	void splitSV(std::vector<std::string_view>& ret, std::string_view strv, char delim);
	bool to_float(std::string_view input, float& out);
	bool IsMeleeOnly(RE::Actor* a_actor);
	float GetEquippementRange(RE::CombatInventory* a_inv, bool a_full = false);
	float RescaleValue(const float a_mult, const float a_min, const float a_max);
	RE::CombatBehaviorTree::TreeBuilder* wrap_to_conditional_2(RE::CombatBehaviorTree::TreeBuilder* a, const char* name, void* extradata, RE::CombatBehaviorTreeNode* node);
}