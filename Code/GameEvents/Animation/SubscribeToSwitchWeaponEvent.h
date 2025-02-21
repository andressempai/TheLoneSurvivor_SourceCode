#ifndef SUBSCRIBE_TO_SWITCH_WEAPON_EVENT_H
#define SUBSCRIBE_TO_SWITCH_WEAPON_EVENT_H

#include <CryCore/BaseTypes.h>

#include <functional>

struct IActionController;
struct SControllerDef;

using EntityId = uint32;

namespace user_code::animation{
	struct PlayerAnimation;
}	// namespace user_code::animation

namespace GameEvents::Animation {
	struct SubscribeToSwitchWeaponEvent {
		explicit SubscribeToSwitchWeaponEvent(user_code::animation::PlayerAnimation& player_animation);
		void subcribe_to_event(EntityId id, IActionController& action_controller, const SControllerDef& controller_definition);
		void unscribe_to_event(EntityId id);
	
		std::reference_wrapper<user_code::animation::PlayerAnimation> player_animation_;
	};
}	// namespace GameEvents::Animation

#endif // !SUBSCRIBE_TO_EQUIP_WEAPON_EVENT_H