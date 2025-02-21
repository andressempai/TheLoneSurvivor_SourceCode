#include "PlayerAnimation.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace user_code::animation {
	void PlayerAnimation::initialize_animation(
			const SControllerDef& controller_definition
		,	IActionController& action_controller
	)
	{
		idle_fragmend_id_ = DevelopmentTools::find_fragment_id(controller_definition, "idle");
		walk_fragmend_id_ = DevelopmentTools::find_fragment_id(controller_definition, "walk");
		shoot_fragment_id_ = DevelopmentTools::find_fragment_id(controller_definition, "shoot");
		lower_weapon_fragment_id_ = DevelopmentTools::find_fragment_id(controller_definition, "lower");
		raise_weapon_fragment_id_ = DevelopmentTools::find_fragment_id(controller_definition, "raise");

		current_weapon_animation_tag_ = DevelopmentTools::find_tag_id(controller_definition, "AmericanShotgun");
		DevelopmentTools::set_animation_tag(&action_controller, current_weapon_animation_tag_, true);

		idle_action_ = DevelopmentTools::make_animation_action(0, idle_fragmend_id_, IAction::Interruptable);
		current_action_ = idle_action_;
		action_controller.Queue(*current_action_);
	}

	void PlayerAnimation::update(const IEntity& entity, IActionController& action_controller)
	{
		using namespace DevelopmentTools;

		const auto parent = get_parent(entity);
		if (not parent)
			return;

		if (get_physics_status<pe_status_living>(*parent).vel.len2() >= 5.0f) {
			if (is_idle_) {
				is_idle_ = false;

				walk_action_ = make_animation_action(10, walk_fragmend_id_, IAction::Interruptable);
				current_action_ = walk_action_;
				action_controller.Queue(*current_action_);
			}
		}
		else {
			if (walk_action_)
				walk_action_->Stop();

			is_idle_ = true;
			current_action_ = idle_action_;
		}

		if (is_switching_weapon_) {
			is_switching_weapon_ = false;

			raise_weapon_action_ = DevelopmentTools::make_animation_action(30, raise_weapon_fragment_id_);
			action_controller.Queue(*raise_weapon_action_);
		}

		if (raise_weapon_action_) {
			auto fragment_duration = 0.0f;
			auto transition_duration = 0.0f;
			action_controller.QueryDuration(*raise_weapon_action_, fragment_duration, transition_duration);

			const auto fragment_time = raise_weapon_action_->GetActiveTime();
			if (fragment_time > fragment_duration * 0.9f) {
				raise_weapon_action_->Stop();
				raise_weapon_action_.reset();

				BoomerShooterEventSystem::instance().send(entity.GetId(), FinishSwitchWeaponEvent{});
			}
		}
	}
}	// namespace user_code::animation