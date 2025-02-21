#include "Search.h"
#include "../DevelopmentTools.h"

#include <CryAISystem/Components/IEntityNavigationComponent.h>

namespace BehaviorTree {
	LoadResult Search::LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor)
	{
		target_key_ = xml->getAttr("targetkey");

		return Action::LoadFromXml(xml, context, isLoadingFromEditor);
	}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef Search::CreateXmlDescription()
	{
		auto xml = Action::CreateXmlDescription();
		xml->setTag("Search");
		xml->setAttr("targetkey", target_key_);

		return xml;
	}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
	void Search::Serialize(Serialization::IArchive& archive)
	{
		Action::Serialize(archive);

		archive(target_key_, "targetkey", "^Target Key");
		archive.doc("Target Key");
	}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

	Status Search::Update(const UpdateContext& context)
	{
		using namespace DevelopmentTools;
		const auto player_id = get_value_from_blackboard<EntityId>(target_key_.c_str(), context.blackboard);
		const auto player = search_entity_by_id(player_id);
		if (not player)
			return Failure;

		if (const auto parent = get_parent(context.entity)) {
			if (const auto navigation_component = get_component_from_entity<IEntityNavigationComponent>(*parent)) {
				auto player_location = player->GetWorldPos();
				if (navigation_component->IsDestinationReachable(player_location))
					last_known_destination_ = player_location;

				navigation_component->NavigateTo(last_known_destination_);
				return Success;
			}
		}

		return Failure;
	}
} // namespace BehaviorTree