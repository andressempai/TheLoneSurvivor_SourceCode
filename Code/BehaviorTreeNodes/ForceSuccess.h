#ifndef FORCE_SUCCESS_H
#define FORCE_SUCCESS_H

#include <CryAISystem/BehaviorTree/Decorator.h>

namespace BehaviorTree {
	class ForceSuccess : public Decorator {
	public:
		struct RuntimeData {};
	
	private:
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		XmlNodeRef CreateXmlDescription() override;
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		Status Update(const BehaviorTree::UpdateContext& context) override;
	};
}	// namespace BehaviorTree

#endif // !FORCE_SUCCESS_H