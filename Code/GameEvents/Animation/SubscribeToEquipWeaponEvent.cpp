#include "SubscribeToEquipWeaponEvent.h"
#include "UserCode/Animation/PlayerAnimation.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Animation {
	SubscribeToEquipWeaponEvent::SubscribeToEquipWeaponEvent(user_code::animation::PlayerAnimation& player_animation)
		:	player_animation_{ player_animation }
	{}

	void SubscribeToEquipWeaponEvent::subcribe_to_event(
			EntityId id
		,	IActionController& action_controller
		,	const SControllerDef& controller_definition
	)
	{
		BoomerShooterEventSystem::instance().subscribe<EquipWeaponEvent>(id, [&](const EventBase& event)
		{
			using namespace DevelopmentTools;

			auto& player_animation = player_animation_.get();
			if (player_animation.current_weapon_animation_tag_ != TAG_ID_INVALID)
				set_animation_tag(&action_controller, player_animation.current_weapon_animation_tag_, false);

			const auto& equip_weapon_event = static_cast<const EquipWeaponEvent&>(event);
			player_animation.current_weapon_animation_tag_ = find_tag_id(
					controller_definition
				,	equip_weapon_event.animation_tag_name_
			);
			set_animation_tag(&action_controller, player_animation.current_weapon_animation_tag_, true);

			player_animation.is_switching_weapon_ = true;
		});
	}

	void SubscribeToEquipWeaponEvent::unscribe_to_event(EntityId id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<EquipWeaponEvent>(id);
	}

}	// namespace GameEvents::Animation