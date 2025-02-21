#include "DistanceCheck.h"
#include "../DevelopmentTools.h"

namespace BehaviorTree {
	LoadResult DistanceCheck::LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor)
	{
		result_key_ = xml->getAttr("resultkey");
		target_key_ = xml->getAttr("targetkey");
		xml->getAttr("min", min_);

		return Action::LoadFromXml(xml, context, isLoadingFromEditor);
	}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef DistanceCheck::CreateXmlDescription()
	{
		auto xml = Action::CreateXmlDescription();
		xml->setTag("DistanceCheck");
		xml->setAttr("resultkey", result_key_);
		xml->setAttr("targetkey", target_key_);
		xml->setAttr("min", min_);

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
	void DistanceCheck::Serialize(Serialization::IArchive& archive)
	{
		Action::Serialize(archive);

		archive(result_key_, "resultkey", "^Result Key");
		archive.doc("Key to store the result(True or False)");

		archive(target_key_, "targetkey", "^Target Key");
		archive.doc("Key to store the target");

		archive(min_, "min", "^Min");
		archive.doc("Min Distance to validate Key");
	}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

	Status DistanceCheck::Update(const BehaviorTree::UpdateContext& context)
	{
		using namespace DevelopmentTools;
		const auto target_entity_id = get_value_from_blackboard<EntityId>(target_key_.c_str(), context.blackboard);
		if (const auto target_entity = search_entity_by_id(target_entity_id)) {
			const auto target_position = target_entity->GetWorldPos();
			const auto current_position = context.entity.GetWorldPos();
			set_value_in_blackboard(
					result_key_.c_str()
				,	context.blackboard
				,	target_position.GetDistance(current_position) <= min_
			);
		}

		return Success;
	}
}	// namespace BehaviorTree