#ifndef MELEE_ATTACK_H
#define MELEE_ATTACK_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class MeleeAttack final : public Action {
	public:
		struct RuntimeData {};
	
	private:
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		XmlNodeRef CreateXmlDescription() override;
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

		Status Update(const UpdateContext& context) override;
	};
}

#endif // !MELEE_ATTACK_H