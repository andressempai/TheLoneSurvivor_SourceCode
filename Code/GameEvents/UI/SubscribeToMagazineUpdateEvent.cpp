#include "GameEvents/UI/SubscribeToMagazineUpdateEvent.h"
#include "Types/WeaponTypes.h"

#include "EventSystem.h"

namespace GameEvents::UI {
	SubscribeToMagazineUpdateEvent::SubscribeToMagazineUpdateEvent(IUIElement& ui_element)
		:	ui_element_{ ui_element }
	{}

	void SubscribeToMagazineUpdateEvent::subcribe_to_event(EntityId id)
	{
		BoomerShooterEventSystem::instance().subscribe<MagazineUpdateEvent>(id, [&](const EventBase& event)
		{
			const auto& magazine_update_event = static_cast<const MagazineUpdateEvent&>(event);
			const auto& weapon = magazine_update_event.weapon_;
			invoke_flash_function(
					weapon
				,	ui_element_
				,	magazine_update_event.size_
				,	magazine_update_event.capacity_
				,	Types::Components::DynamicVectorWeapons{}
			);
		});
	}
	
	void SubscribeToMagazineUpdateEvent::unscribe_to_event(EntityId id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<MagazineUpdateEvent>(id);
	}
}	// namespace GameEvents::UI