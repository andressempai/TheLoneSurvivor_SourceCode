#include "SetFlag.h"
#include "../DevelopmentTools.h"

namespace BehaviorTree {
	LoadResult SetFlag::LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor)
	{
		key_ = xml->getAttr("key");
		xml->getAttr("value", value_);

		return Action::LoadFromXml(xml, context, isLoadingFromEditor);
	}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef SetFlag::CreateXmlDescription()
	{
		auto xml = Action::CreateXmlDescription();
		xml->setTag("SetFlag");
		xml->setAttr("key", key_);
		xml->setAttr("value", value_);

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
	void SetFlag::Serialize(Serialization::IArchive& archive)
	{
		Action::Serialize(archive);

		archive(key_, "key", "^Key");
		archive.doc("Key to set");

		archive(value_, "value", "^Value");
		archive.doc("Value to set");
	}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

	Status SetFlag::Update(const UpdateContext& context)
	{
		DevelopmentTools::set_value_in_blackboard(key_.c_str(), context.blackboard, value_);

		return Success;
	}
}	// namespace BehaviorTree