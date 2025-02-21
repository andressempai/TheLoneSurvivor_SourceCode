#ifndef SUBSCRIBE_TO_BEFORE_SWITCH_WEAPON_EVENT_H
#define SUBSCRIBE_TO_BEFORE_SWITCH_WEAPON_EVENT_H

#include <functional>

struct IActionController;
struct SControllerDef;

using EntityId = unsigned int;

namespace user_code::animation {
	struct Weapon;
}	// namespace user_code::animation

namespace GameEvents::Animation {
	struct SubscribeToBeforeSwitchWeaponEvent {
		explicit SubscribeToBeforeSwitchWeaponEvent(user_code::animation::Weapon& weapon_animation);
		void subcribe_to_event(EntityId id, IActionController& action_controller, const SControllerDef& controller_definition);
		void unscribe_to_event(EntityId id);
	
		std::reference_wrapper<user_code::animation::Weapon> weapon_animation_;
	};
}	// namespace GameEvents::Animation

#endif // !SUBSCRIBE_TO_BEFORE_SWITCH_WEAPON_EVENT_H