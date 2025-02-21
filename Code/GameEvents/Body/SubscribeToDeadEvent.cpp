#include "GameEvents/Body/SubscribeToDeadEvent.h"
#include "Components/Input/InputComponent.h"
#include "Types/UserInterfaceType.h"
#include "Utils/Body.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Body {
	void SubscribeToDeadEvent::subcribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().subscribe<DeadEvent>(entity_id, [this](const EventBase& event)
		{
			const auto& dead_event = static_cast<const DeadEvent&>(event);
			if (const auto parent_entity = DevelopmentTools::get_parent(dead_event.dead_entity_)) {
				if (DevelopmentTools::entity_has_component<InputComponent>(*parent_entity))
					parent_entity->RemoveComponent<InputComponent>();

				BoomerShooterEventSystem::instance().send(parent_entity->GetId(), DeadEvent{ ZERO, ZERO, *parent_entity, 0 });
			}

			const auto child_count = dead_event.dead_entity_.get().GetChildCount();
			for (uint32 i = 0; i < child_count; ++i)
				if (const auto child = dead_event.dead_entity_.get().GetChild(i))
					DevelopmentTools::destroy_entity_by_id(child->GetId());

			DevelopmentTools::destroy_entity_by_id(dead_event.dead_entity_.get().GetId());

			TimerManager::instance().make_timer(dead_timer_id_, 2.0f, false, []()
			{
				using namespace DevelopmentTools;
				if (const auto dead_menu_entity = search_entity_by_name("dead_menu_entity")) {
					const auto dead_menu_component = get_component_from_entity<Types::Components::DeadMenu>(*dead_menu_entity);
					dead_menu_component->show_ui_element();
				}
			});

			DevelopmentTools::play_2d_sound("player_dead");
		});
	}

	void SubscribeToDeadEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<DeadEvent>(entity_id);
	}
}	// namespace GameEvents::Body