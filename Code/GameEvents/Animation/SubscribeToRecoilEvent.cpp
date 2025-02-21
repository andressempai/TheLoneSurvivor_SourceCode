#include "SubscribeToRecoilEvent.h"
#include "UserCode/Animation/PlayerAnimation.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

namespace GameEvents::Animation {
	SubscribeToRecoilEvent::SubscribeToRecoilEvent(user_code::animation::PlayerAnimation& player_animation)
		:	player_animation_{ player_animation }
	{}

	void SubscribeToRecoilEvent::subcribe_to_event(
			EntityId id
		,	IActionController& action_controller
		,	const SControllerDef& controller_definition
	)
	{
		BoomerShooterEventSystem::instance().subscribe<RecoilEvent>(id, [&](const EventBase&)
		{
			using namespace DevelopmentTools;

			auto& player_animation = player_animation_.get();
			if (player_animation.shoot_action_)
				player_animation.shoot_action_->ForceFinish();

			player_animation.shoot_action_ = make_animation_action(30, player_animation.shoot_fragment_id_);
			action_controller.Queue(*player_animation.shoot_action_);
		});
	}

	void SubscribeToRecoilEvent::unscribe_to_event(EntityId id)
	{
		BoomerShooterEventSystem::instance().unsubscribe<RecoilEvent>(id);
	}

}	// namespace GameEvents::Animation