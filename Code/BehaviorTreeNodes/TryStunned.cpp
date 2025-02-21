#include "TryStunned.h"
#include "../Components/Animation/ZombieAnimation.h"
#include "../DevelopmentTools.h"

namespace BehaviorTree {
	LoadResult TryStunned::LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor)
	{
		out_key_ = xml->getAttr("out_key");
		
		return Action::LoadFromXml(xml, context, isLoadingFromEditor);
	}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef TryStunned::CreateXmlDescription()
	{
		auto xml = Action::CreateXmlDescription();
		xml->setTag("TryStunned");
		xml->setAttr("out_key", out_key_);

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
	void TryStunned::Serialize(Serialization::IArchive& archive)
	{
		Action::Serialize(archive);

		archive(out_key_, "out_key", "^Out Key");
		archive.doc("Key to Update");
	}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

	Status TryStunned::Update(const UpdateContext& context)
	{
		using namespace DevelopmentTools;

		if (const auto animation_component = get_component_from_entity<ZombieAnimation>(context.entity)) {
			const auto blackboard = get_blackboard(context.entity.GetId());
			set_value_in_blackboard(out_key_.c_str(), *blackboard, animation_component->is_still_stunned());
		}

		return Success;
	}
}	// namespace BehaviorTree