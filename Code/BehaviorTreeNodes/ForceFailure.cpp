#include "ForceFailure.h"

namespace BehaviorTree {
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef ForceFailure::CreateXmlDescription()
	{
		auto xml = Decorator::CreateXmlDescription();
		xml->setTag("ForceFailure");
	
		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

	Status ForceFailure::Update(const UpdateContext& context)
	{
		Decorator::Update(context);

		return Failure;
	}
}	// !BehaviorTree