#include "UserCode/Animation/Weapon.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace user_code::animation {
	void Weapon::initialize_animation(const SControllerDef& controller_definition, IActionController& action_controller)
	{
		using namespace DevelopmentTools;

		idle_fragment_id_ = find_fragment_id(controller_definition, "idle");
		shoot_fragment_id_ = find_fragment_id(controller_definition, "shoot");

		set_animation_tag(&action_controller, find_tag_id(controller_definition, tag_name_), true);

		idle_action_ = make_animation_action(0, idle_fragment_id_, IAction::Interruptable);
		action_controller.Queue(*idle_action_);
	}

	void Weapon::update(const IEntity& entity, IActionController& action_controller)
	{
	}
}	// namespace user_code::animation