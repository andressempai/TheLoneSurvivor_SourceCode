#ifndef SEARCH_H
#define SEARCH_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class Search final : public Action {
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

		string target_key_{};
		Vec3 last_known_destination_{ ZERO };
	};
}

#endif // !SEARCH_H