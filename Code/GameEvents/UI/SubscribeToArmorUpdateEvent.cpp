#include "GameEvents/UI/SubscribeToArmorUpdateEvent.h"
#include "EventSystem.h"

#include <CrySystem/Scaleform/IFlashUI.h>

namespace GameEvents::UI {
	SubscribeToArmorUpdateEvent::SubscribeToArmorUpdateEvent(IUIElement& ui_element)
		:	ui_element_{ ui_element }
	{}

	void SubscribeToArmorUpdateEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<ArmorUpdateEvent>(entity_id, [&](const EventBase& event)
		{
			const auto& armor_update_event = static_cast<const ArmorUpdateEvent&>(event);
			SUIArguments args{};
			args.AddArgument(armor_update_event.hardness_);
			
			ui_element_.get().CallFunction("update_armor_hardness", args);
		});
	}

	void SubscribeToArmorUpdateEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<ArmorUpdateEvent>(entity_id);
	}
}	// namespace GameEvents::UI