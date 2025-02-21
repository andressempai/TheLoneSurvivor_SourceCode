#ifndef DISTANCE_CHECK_H
#define DISTANCE_CHECK_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class DistanceCheck final : public Action {
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
	
		string result_key_{};
		string target_key_{};
		f32 min_{};
	};
}	// namespace BehaviorTree

#endif // !DISTANCE_CHECK_H