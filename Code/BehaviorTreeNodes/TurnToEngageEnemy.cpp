#include "TurnToEngageEnemy.h"
#include "../DevelopmentTools.h"

#include <CryAISystem/BehaviorTree/Decorator.h>
#include <CryAISystem/BehaviorTree/NodeFactory.h>
#include <CrySerialization/ClassFactory.h>

namespace BehaviorTree {
	LoadResult TurnToEngageEnemy::LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor)
	{
		xml->getAttr("rotationspeed", rotation_speed_);

		return Action::LoadFromXml(xml, context, isLoadingFromEditor);
	}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
	XmlNodeRef TurnToEngageEnemy::CreateXmlDescription()
	{
		auto xml = Node::CreateXmlDescription();
		xml->setTag("TurnToEngageEnemy");
		xml->setAttr("rotationspeed", rotation_speed_);

		return xml;
	}
#endif // USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
	void TurnToEngageEnemy::Serialize(Serialization::IArchive& archive)
	{
		Action::Serialize(archive);

		archive(rotation_speed_, "rotationspeed", "^RotationSpeed");
		archive.doc("Speed at which the entity will rotate to face the target");
	}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

	Status TurnToEngageEnemy::Update(const UpdateContext& context)
	{
		using namespace DevelopmentTools;

		if (const auto parent = get_parent(context.entity)) {
			const auto living_status = get_physics_status<pe_status_living>(*parent);
			auto direction = living_status.vel.normalized();
			direction.z = 0.0f;

			if (not direction.IsEquivalent(ZERO)) {
				const auto enemy_id = get_value_from_blackboard<EntityId>("enemy_id", context.blackboard);
				const auto enemy = search_entity_by_id(enemy_id);
				const auto distance = (enemy->GetWorldPos() - parent->GetWorldPos()).GetLength();

				const auto min_distance = 2.55f;
				const auto distance_difference = min_distance - distance;
				const auto rotation_speed_based_on_distance =
					CLAMP(rotation_speed_ - crymath::abs(distance_difference), 9.5f, 16.0f);

				const auto interpolation_speed = rotation_speed_based_on_distance * context.frameDeltaTime;
				const auto forward_vector = parent->GetForwardDir();
				const auto rotation_vector = Vec3::CreateSlerp(forward_vector, direction, interpolation_speed);

				auto pos_param = pe_params_pos{};
				pos_param.q = Quat::CreateRotationVDir(rotation_vector);
				update_physics_params(*parent, pos_param);
			}
		}

		return Success;
	}
} // namespace BehaviorTree