#include "ForceSuccess.h"

namespace BehaviorTree {
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef ForceSuccess::CreateXmlDescription()
	{
		auto xml = Decorator::CreateXmlDescription();
		xml->setTag("ForceSuccess");
	
		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

	Status ForceSuccess::Update(const UpdateContext& context)
	{
		Decorator::Update(context);

		return Success;
	}
}	// !BehaviorTree