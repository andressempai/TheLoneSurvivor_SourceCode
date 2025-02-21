#include "Cooldown.h"
#include "../DevelopmentTools.h"

namespace BehaviorTree {
	LoadResult Cooldown::LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor)
	{
		key_ = xml->getAttr("key");
		xml->getAttr("time", time_);

		current_time_ = time_;

		return Action::LoadFromXml(xml, context, isLoadingFromEditor);
	}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef Cooldown::CreateXmlDescription()
	{
		auto xml = Action::CreateXmlDescription();
		xml->setTag("Cooldown");
		xml->setAttr("key", key_);
		xml->setAttr("time", time_);

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
	void Cooldown::Serialize(Serialization::IArchive& archive)
	{
		Action::Serialize(archive);

		archive(key_, "key", "^Key");
		archive.doc("Key to use for the cooldown");

		archive(time_, "time", "^Time");
		archive.doc("Time to wait before the next action");
	}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

	Status Cooldown::Update(const BehaviorTree::UpdateContext& context)
	{
		if (not DevelopmentTools::get_value_from_blackboard<bool>(key_.c_str(), context.blackboard)) {
			current_time_ -= context.frameDeltaTime;
			if (current_time_ <= 0.0f) {
				current_time_ = time_;
				context.blackboard.SetVariable(key_.c_str(), true);
			}
		}

		return Success;
	}
}	// namespace BehaviorTree