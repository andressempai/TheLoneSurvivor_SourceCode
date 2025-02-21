#ifndef SUBSCRIBE_TO_AI_POINT_DAMAGE_EVENT_H
#define SUBSCRIBE_TO_AI_POINT_DAMAGE_EVENT_H

#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	template <typename AIBody>
	struct SubscribeToAIPointDamageEvent {
		void subcribe_to_event(EntityId entity_id)
		{
			BoomerShooterEventSystem::instance().subscribe<PointDamageEvent>(entity_id, [this](const EventBase& event)
			{
				const auto& damage_event = static_cast<const PointDamageEvent&>(event);
				body_->impact_damage(
						damage_event.damaged_entity_
					,	damage_event.impacts_points_[0]
					,	damage_event.impact_direction_
					,	damage_event.part_ids_[0]
					,	damage_event.damage_
					,	*body_
				);
			});
		}

		void unscribe_to_event(EntityId entity_id)
		{
			BoomerShooterEventSystem::instance().unsubscribe<PointDamageEvent>(entity_id);
		}

		void set_body(AIBody* body)
		{
			body_ = body;
		}

		AIBody* body_{};
	};
}	// namespace GameEvents::Body

#endif // !SUBSCRIBE_TO_AI_POINT_DAMAGE_EVENT_H