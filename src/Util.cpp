#include "Util.h"

namespace CombatPathing
{
	bool IsMeleeOnly(RE::Actor* a_actor)
	{
		using TYPE = RE::CombatInventoryItem::TYPE;

		if (!a_actor)
			return false;

		const auto combatCtrl = a_actor->combatController;
		const auto CombatInv = combatCtrl ? combatCtrl->inventory : nullptr;
		if (CombatInv) {
			for (const auto item : CombatInv->equippedItems) {
				if (item.item) {
					switch (item.item->GetType()) {
					case TYPE::kMagic:
					case TYPE::kRanged:
					case TYPE::kScroll:
					case TYPE::kStaff:
						return false;

					default:
						break;
					}
				}
			}

			return true;
		}

		return false;
	}

	float GetEquippementRange(RE::CombatInventory* a_inv, bool a_full)
	{
		if (a_inv) {
			return a_full ? a_inv->maximumRange : a_inv->optimalRange;
		}

		return 0.f;
	}

	float RescaleValue(const float a_mult, const float a_min, const float a_max)
	{
		return a_min + a_mult * (a_max - a_min);
	}

	// used
	// inlined in this function on AE: 140816E40 - better example: 14083233B
	RE::CombatBehaviorTree::TreeBuilder* wrap_to_conditional_2(RE::CombatBehaviorTree::TreeBuilder* a, const char* name, void* extradata, RE::CombatBehaviorTreeNode* node)
	{
		// use the the function we modified to imitate the 1.5.97 function - was the first plan, changed it to a REed implementation
		//return _generic_foo<47845, NodeArray&, NodeArray&, const char*, void*, CombatBehaviorTreeNode*>(a, name, extradata, node);

		auto condNode = RE::CombatBehaviorTreeConditionalNodeImpl::Create();
		if (condNode) {
			condNode->expr = extradata;
			condNode->isSelector = true;

			char DstBuf[260];
			sprintf_s(DstBuf, 260, "ConditionalNode - %s", name);
			condNode->name = RE::BSFixedString(DstBuf);
			condNode->AddChild(node);

			return RE::CombatBehaviorTree::AddNode(a, name, node);
		}

		return a;
	}
}