#include "GameEvents/Animation/SubscribeToBeforeSwitchWeaponEvent.h"
#include "UserCode/Animation/Weapon.h"
#include "EventSystem.h"

#include <CryAnimation/ICryMannequin.h>

namespace GameEvents::Animation {
	SubscribeToBeforeSwitchWeaponEvent::SubscribeToBeforeSwitchWeaponEvent(user_code::animation::Weapon& weapon_animation)
		:	weapon_animation_{ weapon_animation }
	{}

	void SubscribeToBeforeSwitchWeaponEvent::subcribe_to_event(
			EntityId entity_id
		,	IActionController& action_controller
		,	const SControllerDef& controller_definition
	)
	{
		BoomerShooterEventSystem::instance().subscribe<BeforeSwitchWeaponEvent>(entity_id, [this](const EventBase&)
		{
			auto& weapon_animation = weapon_animation_.get();
			if (weapon_animation.shoot_action_)
				weapon_animation.shoot_action_->ForceFinish();
		});
	}

	void SubscribeToBeforeSwitchWeaponEvent::unscribe_to_event(EntityId entity_id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<BeforeSwitchWeaponEvent>(entity_id);
	}
}	// namespace GameEvents::Animation