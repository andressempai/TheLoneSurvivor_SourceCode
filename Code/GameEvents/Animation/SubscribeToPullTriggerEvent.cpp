#include "GameEvents/Animation/SubscribeToPullTriggerEvent.h"
#include "UserCode/Animation/Weapon.h"
#include "Components/Magazine/Magazine.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Animation {
	SubscribeToPullTriggerEvent::SubscribeToPullTriggerEvent(user_code::animation::Weapon& weapon_animation)
		:	weapon_animation_{ weapon_animation }
	{}

	void SubscribeToPullTriggerEvent::subcribe_to_event(
			EntityId entity_id
		,	IActionController& action_controller
		,	const SControllerDef& controller_definition
	)
	{
		BoomerShooterEventSystem::instance().subscribe<PullTriggerEvent>(entity_id,
		[this, entity_id, &action_controller, &controller_definition](const EventBase&)
		{
			using namespace DevelopmentTools;

			auto& weapon_animation = weapon_animation_.get();
			if (TimerManager::instance().is_timer_active(weapon_animation.cooldown_timer_id_))
				return;

			const auto pistol_entity = search_entity_by_id(entity_id);
			const auto magazine = get_component_from_entity<Magazine>(*pistol_entity);
			if (not magazine or magazine->is_empty())
				return;
			
			if (weapon_animation.shoot_action_)
				weapon_animation.shoot_action_->ForceFinish();
			
			weapon_animation.shoot_action_ = make_animation_action(10, weapon_animation.shoot_fragment_id_);
			action_controller.Queue(*weapon_animation.shoot_action_);
			
			TimerManager::instance().make_timer(
					weapon_animation.cooldown_timer_id_
				,	weapon_animation.cooldown_time_
				,	false
				,	nullptr
			);
		});
	}

	void SubscribeToPullTriggerEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<PullTriggerEvent>(entity_id);
	}

}	// namespace GameEvents::Animation