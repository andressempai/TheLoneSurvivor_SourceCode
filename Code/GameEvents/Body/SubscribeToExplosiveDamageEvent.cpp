#include "GameEvents/Body/SubscribeToExplosiveDamageEvent.h"
#include "Utils/Body.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	void SubscribeToExplosiveDamageEvent::subcribe_to_event(EntityId entity_id)
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

			BoomerShooterEventSystem::instance().send(entity_id, HealthUpdateEvent{ body_->health_pack_.health() });

			if (body_->armor_)
				BoomerShooterEventSystem::instance().send(entity_id, ArmorUpdateEvent{ body_->armor_->hardness() });
			else
				BoomerShooterEventSystem::instance().send(entity_id, ArmorUpdateEvent{ 0 });

			if (not body_->health_pack_.is_empty())
				DevelopmentTools::play_2d_sound("player_injured");
		});
	}

	void SubscribeToExplosiveDamageEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<ExplosiveDamageEvent>(entity_id);
	}

	void SubscribeToExplosiveDamageEvent::set_body(TBody<PlayerSpineBodyData>* body)
	{
		body_ = body;
	}
}	// namespace GameEvents::Body