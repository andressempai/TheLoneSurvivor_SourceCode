#include "GameEvents/Body/SubscribeToHealEvent.h"
#include "Utils/Body.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	void SubscribeToHealEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<HealEvent>(entity_id, [entity_id, this](const EventBase& event)
		{
			const auto& heal_event = static_cast<const HealEvent&>(event);
			if (body_->heal(heal_event.health_pack_)) {
				DevelopmentTools::destroy_entity_by_id(heal_event.healed_entity_.get().GetId());

				BoomerShooterEventSystem::instance().send(entity_id, HealthUpdateEvent{ body_->health_pack_.health() });
			}
		});
	}

	void SubscribeToHealEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<HealEvent>(entity_id);
	}

	void SubscribeToHealEvent::set_body(TBody<PlayerSpineBodyData>* body)
	{
		body_ = body;
	}
}	// namespace GameEvents::Body