#include "MeleeAttack.h"
#include "../EventSystem.h"

namespace BehaviorTree {
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef MeleeAttack::CreateXmlDescription()
	{
		auto xml = Action::CreateXmlDescription();
		xml->setTag("MeleeAttack");

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

	Status MeleeAttack::Update(const UpdateContext& context)
	{
		BoomerShooterEventSystem::instance().send(context.entity.GetId(), MelleeAttackEvent{});

		return Success;
	}
}	// namespace BehaviorTree