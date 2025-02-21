#include "GameEvents/Body/SubscribeToPointDamageEvent.h"
#include "Utils/Body.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	void SubscribeToPointDamageEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<PointDamageEvent>(entity_id, [=](const EventBase& event)
		{
			const auto& point_damage_event = static_cast<const PointDamageEvent&>(event);
			body_->impact_damage(
					point_damage_event.damaged_entity_
				,	point_damage_event.impacts_points_[0]
				,	point_damage_event.impact_direction_
				,	point_damage_event.part_ids_[0]
				,	point_damage_event.damage_
				,	*body_
			);

			BoomerShooterEventSystem::instance().send(entity_id, HealthUpdateEvent{ body_->health_pack_.health() });

			if (body_->armor_)
				BoomerShooterEventSystem::instance().send(entity_id, ArmorUpdateEvent{ body_->armor_->hardness() });
			else
				BoomerShooterEventSystem::instance().send(entity_id, ArmorUpdateEvent{ 0 });
		});
	}

	void SubscribeToPointDamageEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<PointDamageEvent>(entity_id);
	}

	void SubscribeToPointDamageEvent::set_body(TBody<PlayerSpineBodyData>* body)
	{
		body_ = body;
	}
}	// namespace GameEvents::Body