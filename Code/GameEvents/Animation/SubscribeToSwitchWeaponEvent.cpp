#include "SubscribeToSwitchWeaponEvent.h"
#include "UserCode/Animation/PlayerAnimation.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Animation {
	SubscribeToSwitchWeaponEvent::SubscribeToSwitchWeaponEvent(user_code::animation::PlayerAnimation& player_animation)
		:	player_animation_{ player_animation }
	{}

	void SubscribeToSwitchWeaponEvent::subcribe_to_event(
			EntityId id
		,	IActionController& action_controller
		,	const SControllerDef& controller_definition
	)
	{
		BoomerShooterEventSystem::instance().subscribe<SwitchWeaponEvent>(id, [&](const EventBase&)
		{
			auto& player_animation = player_animation_.get();
			if (player_animation.shoot_action_)
				player_animation.shoot_action_->ForceFinish();

			player_animation.lower_weapon_action_ = DevelopmentTools::make_animation_action(
					20
				,	player_animation.lower_weapon_fragment_id_
			);
			action_controller.Queue(*player_animation.lower_weapon_action_);
		});
	}

	void SubscribeToSwitchWeaponEvent::unscribe_to_event(EntityId id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<SwitchWeaponEvent>(id);
	}

}	// namespace GameEvents::Animation