#pragma once

namespace RE
{
	class CombatPath : public CombatObject
	{
	public:
		inline static constexpr auto RTTI = RTTI_CombatPath;
		inline static constexpr auto VTABLE = VTABLE_CombatPath;

		enum class SPEED : std::uint32_t
		{
			kUnk00,
			kUnk01,
			kUnk02,
			kUnk03,
			kUnk04
		};

		enum class STATE : std::uint32_t
		{
			kNone,
			kBuilding,
			kFollowing,
			kRetrying,
			kComplete,
			kFailed,
			kUnk06
		};

		~CombatPath() override;  // 00

		// override (CombatObject)
		void SaveGame(BGSSaveGameBuffer* a_buf) override;  // 03
		void LoadGame(BGSLoadGameBuffer* a_buf) override;  // 04

		// add
		virtual void Unk_05(void) = 0;  // 05
		virtual void Unk_06(void) = 0;  // 06
		virtual void Unk_07(void) = 0;  // 07
		virtual void Unk_08(void) = 0;  // 08
		virtual void Unk_09(void) = 0;  // 09
		virtual void Unk_0A(void) = 0;  // 0A
		virtual void Unk_0B(void) = 0;  // 0B
		virtual void Unk_0C(void) = 0;  // 0C

		void SetAccelerationDecelerationMult(float a_accelerationMult, float a_decelerationMult)
		{
			using func_t = decltype(&CombatPath::SetAccelerationDecelerationMult);
			static REL::Relocation<func_t> func{ RELOCATION_ID(49022, 49953) };
			func(this, a_accelerationMult, a_decelerationMult);
		}

		void Start()
		{
			using func_t = decltype(&CombatPath::Start);
			static REL::Relocation<func_t> func{ RELOCATION_ID(49013, 49944) };
			func(this);
		}

		void Update()
		{
			using func_t = decltype(&CombatPath::Start);
			static REL::Relocation<func_t> func{ RELOCATION_ID(49011, 47899) };
			func(this);
		}

		// members
		ActorHandle actor;   // 10
		STATE state;         // 14
		STATE lastState;     // 18
		SPEED speed;         // 1C
		AITimer retryTimer;  // 20
		AITimer waitTimer;   // 28
		void* unk30;         // 30 - smart ptr
	};
	static_assert(sizeof(CombatPath) == 0x38);

	class CombatAimController;
	class CombatTargetLocationSearch;
	class CombatTargetLocationSearchResult;

	class CombatBehaviorAccessors
	{
	public:
	};
	static_assert(std::is_empty_v<CombatBehaviorAccessors>);

	class CombatBehavior : public CombatBehaviorAccessors
	{
	public:
		bool CheckTargetChanged()
		{
			using func_t = decltype(&CombatBehavior::CheckTargetChanged);
			static REL::Relocation<func_t> func{ RELOCATION_ID(46089, 47353) };
			return func(this);
		}

		CombatBehaviorThread* CreateChildThread(std::uint32_t a_childIndex, bool a_addThread)
		{
			using func_t = decltype(&CombatBehavior::CreateChildThread);
			static REL::Relocation<func_t> func{ RELOCATION_ID(46090, 47354) };
			return func(this, a_childIndex, a_addThread);
		}

		void StartChildThread(CombatBehaviorThread* a_thread, std::uint32_t a_childIndex, bool a_addThread)
		{
			using func_t = decltype(&CombatBehavior::StartChildThread);
			static REL::Relocation<func_t> func{ RELOCATION_ID(46091, 47355) };
			func(this, a_thread, a_childIndex, a_addThread);
		}
	};
	static_assert(std::is_empty_v<CombatBehavior>);

	class CombatBehaviorAction : public CombatBehavior
	{
	public:
	};
	static_assert(std::is_empty_v<CombatBehaviorAction>);

	class CombatBehaviorAdvance : public CombatBehaviorAction
	{
	public:
		// members
		NiPointer<CombatPath> path;                                // 00
		NiPointer<CombatAimController> aimController;              // 08
		NiPointer<CombatTargetLocationSearch> locationSearch;      // 10
		NiPointer<CombatTargetLocationSearchResult> searchResult;  // 18
	};
	static_assert(sizeof(CombatBehaviorAdvance) == 0x20);

#define DECLARE_CombatBehaviorTreeNodeObjectBase(T)                                                  \
	class CombatBehaviorTreeNodeObjectBase_##T##_ : public CombatBehaviorTreeNode                    \
	{                                                                                                \
	public:                                                                                          \
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObjectBase_##T##_;        \
                                                                                                     \
		void* destroy(char need_freeself) override;                                                  \
		void Exit(CombatBehaviorThread* a_thread) override;                                          \
		void Update(CombatBehaviorThread* a_thread) override;                                        \
		void Abort(CombatBehaviorThread* a_thread) override;                                         \
		void SaveGame(CombatBehaviorThread* a_thread, BGSSaveFormBuffer* a_saveGameBuffer) override; \
		void LoadGame(CombatBehaviorThread* a_thread, BGSLoadFormBuffer* a_loadGameBuffer) override; \
		bool Validate(const CombatBehaviorTreeNode* a_node) override;                                \
		const BSFixedString& GetType() override;                                                     \
	};                                                                                               \
	static_assert(sizeof(CombatBehaviorTreeNodeObjectBase_##T##_) == 0x28);

#define DECLARE_CombatBehaviorTreeNodeObject_(T)                                               \
	class CombatBehaviorTreeNodeObject_##T##_ : public CombatBehaviorTreeNodeObjectBase_##T##_ \
	{                                                                                          \
	public:                                                                                    \
		inline static constexpr auto RTTI = RE::RTTI_CombatBehaviorTreeNodeObject_##T##_;      \
		inline static constexpr auto VTABLE = RE::RTTI_CombatBehaviorTreeNodeObject_##T##_;    \
                                                                                               \
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

	DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorAdvance);  // 1416960B0
	using NodeCloseMovementAdvance = CombatBehaviorTreeNodeObject_CombatBehaviorAdvance_;

	DECLARE_CombatBehaviorTreeNodeObject(CombatBehaviorCircle);  // 1416964D0
	using NodeCloseMovementCircle = CombatBehaviorTreeNodeObject_CombatBehaviorCircle_;

	RE::RTTI_CombatBehaviorTreeNodeObjectBase_CombatBehaviorAdvance_;
}