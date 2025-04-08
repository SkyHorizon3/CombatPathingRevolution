#pragma once

namespace RE
{
	class CombatBehaviorTreeNode;
	class CombatBehaviorTreeControl;

	struct TreeCtors_extradata
	{
		void* func1;
		void* func2;
	};

	using NodeArray = RE::BSTArray<CombatBehaviorTreeNode*>;
	class CombatBehaviorTreeNode
	{
	public:
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNode;  // 141692510

		// add
		virtual void* destroy(char need_freeself);
		virtual char* get_name();
		virtual CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control);
		virtual void pop(CombatBehaviorTreeControl* control);
		virtual void on_childfailed(CombatBehaviorTreeControl* control);
		virtual void on_interrupted(CombatBehaviorTreeControl* control);
		virtual void* SaveGame(CombatBehaviorTreeControl* control, RE::BGSSaveGameBuffer* buffer);
		virtual void* LoadGame(CombatBehaviorTreeControl* control, RE::BGSLoadGameBuffer* buffer);
		virtual bool __unk_8(CombatBehaviorTreeControl* control);
		virtual RE::BSFixedString* __unk_9();

		void AddChild(CombatBehaviorTreeNode* node)
		{
			using func_t = decltype(&CombatBehaviorTreeNode::AddChild);
			static REL::Relocation<func_t> func{ RELOCATION_ID(46304, 47548) };
			return func(this, node);
		}

		// members
		RE::BSFixedString name;
		CombatBehaviorTreeNode* parent;
		CombatBehaviorTreeNode** childs;
		int childs_count;
		char __pad_24[4];

	protected:
		CombatBehaviorTreeNode* Ctor()
		{
			using func_t = decltype(&CombatBehaviorTreeNode::Ctor);
			static REL::Relocation<func_t> func{ RELOCATION_ID(46301, 47545) };
			return func(this);
		}
	};
	static_assert(sizeof(CombatBehaviorTreeNode) == 0x28);

	class CombatBehaviorTreeConditionalNodeImpl : public CombatBehaviorTreeNode
	{
	public:
		~CombatBehaviorTreeConditionalNodeImpl() = default;

		static CombatBehaviorTreeConditionalNodeImpl* Create()
		{
			auto node = malloc<CombatBehaviorTreeConditionalNodeImpl>();
			std::memset(node, 0, sizeof(CombatBehaviorTreeConditionalNodeImpl));
			if (node) {
				node->Ctor();

				*(uint64_t*)node = VTABLE_CombatBehaviorTreeConditionalNode_CombatBehaviorExpression_CombatBehaviorMemberFunc_CombatBehaviorContextCloseMovement_bool__CombatBehaviorContextCloseMovement_____void____[0].address();  // set vtable
			}

			return node;
		}

		// members
		void* expr;
		bool isSelector;
	};
	static_assert(sizeof(CombatBehaviorTreeConditionalNodeImpl) == 0x38);

#define DECLARE_CombatBehaviorTreeNodeObjectBase(T)                                                 \
	class CombatBehaviorTreeNodeObjectBase_##T##_ : public CombatBehaviorTreeNode                   \
	{                                                                                               \
	public:                                                                                         \
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObjectBase_##T##_;       \
                                                                                                    \
		void* destroy(char need_freeself) override;                                                 \
		void pop(CombatBehaviorTreeControl* control) override;                                      \
		void on_childfailed(CombatBehaviorTreeControl* control) override;                           \
		void on_interrupted(CombatBehaviorTreeControl* control) override;                           \
		void* SaveGame(CombatBehaviorTreeControl* control, RE::BGSSaveGameBuffer* buffer) override; \
		void* LoadGame(CombatBehaviorTreeControl* control, RE::BGSLoadGameBuffer* buffer) override; \
		bool __unk_8(CombatBehaviorTreeControl* control) override;                                  \
		RE::BSFixedString* __unk_9() override;                                                      \
	};                                                                                              \
	static_assert(sizeof(CombatBehaviorTreeNodeObjectBase_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject_(T)                                               \
	class CombatBehaviorTreeNodeObject_##T##_ : public CombatBehaviorTreeNodeObjectBase_##T##_ \
	{                                                                                          \
	public:                                                                                    \
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObject_##T##_;      \
		inline static constexpr auto VTABLE = RE::RTTI_CombatBehaviorTreeNodeObject_##T##_;    \
		void* destroy(char need_freeself) override;                                            \
		CombatBehaviorTreeControl* act(CombatBehaviorTreeControl* control) override;           \
                                                                                               \
		static CombatBehaviorTreeNodeObject_##T##_* createnew();                               \
	};                                                                                         \
	static_assert(sizeof(CombatBehaviorTreeNodeObject_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject(T)  \
	DECLARE_CombatBehaviorTreeNodeObjectBase(T); \
	DECLARE_CombatBehaviorTreeNodeObject_(T);

#define DEFINE_CombatBehaviorTree_XXX__createnew(id, T) \
	T* T::createnew()                                   \
	{                                                   \
		return _generic_foo<id, T*>();                  \
	}

#define DEFINE_CombatBehaviorTreeNodeObject_createnew(T, id) \
	DEFINE_CombatBehaviorTree_XXX__createnew(id, CombatBehaviorTreeNodeObject_##T##_)

}