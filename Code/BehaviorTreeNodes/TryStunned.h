#ifndef TRY_STUNNED_H
#define TRY_STUNNED_H

#include "../TimerManager.h"

#include <CryAISystem/BehaviorTree/Action.h>

namespace BehaviorTree {
	class TryStunned final : public Action {
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

		TimerId timer_id_{};
		string out_key_{};
		bool is_initialized_{ false };
	};
}

#endif // !TRY_STUNNED_H