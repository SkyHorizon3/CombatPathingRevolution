#include "CombatBehaviorNodesMovement.h"

namespace RE
{
	/*
	static CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_* RunCreate(char* a_mem)
	{
		return _generic_foo<47545, CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_*>(a_mem);
	}

	CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_* Test()
	{
		char* mem = (char*)RE::MemoryManager::GetSingleton()->Allocate(0x28, 0, 0);

		if (mem) {
			auto* tree = RunCreate(mem);

			stl::emplace_vtable<CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_>(tree);

			return tree;
		}
	}
	*/

	//DEFINE_CombatBehaviorTree_XXX__createnew(46753, CombatBehaviorTreeCreateContextNode_CombatBehaviorContextCloseMovement_);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorFallback, 46762);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorBackoff, 46759);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorSurround, 46765);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorReposition, 46764);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorAdvance, 46758);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorFallbackToRanged, 46763);
	DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorCircle, 49470);
	//DEFINE_CombatBehaviorTreeNodeObject_createnew(CombatBehaviorCircleDistant, 46761);

}