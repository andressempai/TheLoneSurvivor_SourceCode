#include "GameEvents/Body/SubscribeToRepairArmorEvent.h"
#include "Utils/Body.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	void SubscribeToRepairArmorEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<RepairArmorEvent>(entity_id, [=](const EventBase& event)
		{
			const auto& repair_armor_event = static_cast<const RepairArmorEvent&>(event);
			if (body_->repair_armor(repair_armor_event.armor_)) {
				DevelopmentTools::destroy_entity_by_id(repair_armor_event.armor_entity_.get().GetId());

				if (body_->armor_)
					BoomerShooterEventSystem::instance().send(entity_id, ArmorUpdateEvent{ body_->armor_->hardness() });
				else
					BoomerShooterEventSystem::instance().send(entity_id, ArmorUpdateEvent{ 0 });
			}
		});
	}

	void SubscribeToRepairArmorEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<RepairArmorEvent>(entity_id);
	}

	void SubscribeToRepairArmorEvent::set_body(TBody<PlayerSpineBodyData>* body)
	{
		body_ = body;
	}
}	// namespace GameEvents::Body