#include "ForceRunning.h"

namespace BehaviorTree {
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef ForceRunning::CreateXmlDescription()
	{
		auto xml = Decorator::CreateXmlDescription();
		xml->setTag("ForceRunning");
	
		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

	Status ForceRunning::Update(const UpdateContext& context)
	{
		Decorator::Update(context);

		return Running;
	}
}	// !BehaviorTree