#ifndef COLDOWN_H
#define COLDOWN_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class Cooldown final : public Action {
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

		Status Update(const BehaviorTree::UpdateContext& context) override;
	
		string key_{};
		f32 time_{};
		f32 current_time_{};
	};
}	// namespace BehaviorTree

#endif // !COLDOWN_H