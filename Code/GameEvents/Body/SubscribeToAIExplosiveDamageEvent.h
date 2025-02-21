#ifndef SUBSCRIBE_TO_AI_EXPLOSIVE_DAMAGE_EVENT_H
#define SUBSCRIBE_TO_AI_EXPLOSIVE_DAMAGE_EVENT_H

#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	template <typename AIBody>
	struct SubscribeToAIExplosiveDamageEvent {
		void subcribe_to_event(EntityId entity_id)
		{
			BoomerShooterEventSystem::instance().subscribe<ExplosiveDamageEvent>(entity_id, [=](const EventBase& event)
			{
				const auto& damage_event = static_cast<const ExplosiveDamageEvent&>(event);
				body_->explosion_damage(
						damage_event.damaged_entity_
					,	damage_event.origin_
					,	damage_event.direction_
					,	damage_event.damage_
				);
			});
		}

		void unscribe_to_event(EntityId entity_id)
		{
			BoomerShooterEventSystem::instance().unsubscribe<ExplosiveDamageEvent>(entity_id);
		}

		void set_body(AIBody* body)
		{
			body_ = body;
		}

		AIBody* body_{};
	};
}	// namespace GameEvents::Body

#endif // !SUBSCRIBE_TO_AI_EXPLOSIVE_DAMAGE_EVENT_H