#ifndef TURN_TO_ENGAGE_ENEMY_H
#define TURN_TO_ENGAGE_ENEMY_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class TurnToEngageEnemy final : public Action {
	public:
		struct RuntimeData {};
	
	private:
		LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor) override;

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		XmlNodeRef CreateXmlDescription() override;
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
		void Serialize(Serialization::IArchive& archive) override;
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

		Status Update(const UpdateContext& context) override;

		f32 rotation_speed_{};
	};
}

#endif // !TURN_TO_ENGAGE_ENEMY_H