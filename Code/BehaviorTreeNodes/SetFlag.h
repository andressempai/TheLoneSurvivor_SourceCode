#ifndef SET_FLAG_H
#define SET_FLAG_H

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class SetFlag final : public Action {
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

		string key_{};
		bool value_{};
	};
}

#endif // !SET_FLAG_H