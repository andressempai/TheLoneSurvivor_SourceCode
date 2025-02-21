#ifndef STOP_MOVEMENT_H
#define STOP_MOVEMENT_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class StopSearch final : public Action {
	public:
		struct RuntimeData {};
	
	private:
#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		XmlNodeRef CreateXmlDescription() override;
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

		Status Update(const UpdateContext& context) override;
	};
}

#endif // !STOP_MOVEMENT_H