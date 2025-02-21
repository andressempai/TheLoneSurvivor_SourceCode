#include "StopSearch.h"
#include "../DevelopmentTools.h"

#include <CryAISystem/BehaviorTree/Decorator.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

namespace BehaviorTree {
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef StopSearch::CreateXmlDescription()
	{
		auto xml = Node::CreateXmlDescription();
		xml->setTag("StopSearch");

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

	Status StopSearch::Update(const UpdateContext& context)
	{
		using namespace DevelopmentTools;

		if (const auto parent = get_parent(context.entity)) {
			if (const auto navigation_component = get_component_from_entity<IEntityNavigationComponent>(*parent))
				navigation_component->StopMovement();
		}

		return Success;
	}
} // namespace BehaviorTree