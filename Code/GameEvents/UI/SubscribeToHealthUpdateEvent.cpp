#include "GameEvents/UI/SubscribeToHealthUpdateEvent.h"
#include "EventSystem.h"

#include <CrySystem/Scaleform/IFlashUI.h>

namespace GameEvents::UI {
	SubscribeToHealthUpdateEvent::SubscribeToHealthUpdateEvent(IUIElement& ui_element)
		:	ui_element_{ ui_element }
	{}

	void SubscribeToHealthUpdateEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<HealthUpdateEvent>(entity_id, [&](const EventBase& event)
		{
			const auto& health_update_event = static_cast<const HealthUpdateEvent&>(event);
			SUIArguments args{};
			args.AddArgument(health_update_event.health_);

			ui_element_.get().CallFunction("update_health_pack", args);
		});
	}

	void SubscribeToHealthUpdateEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<HealthUpdateEvent>(entity_id);
	}
}	// namespace GameEvents::UI