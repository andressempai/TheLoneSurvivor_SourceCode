#ifndef WEAPON_ANIMATON_H
#define WEAPON_ANIMATON_H

#include "TimerManager.h"

#include <CryCore/smartptr.h>
#include <CryCore/BaseTypes.h>

#include <string_view>

struct IEntity;
struct SControllerDef;
struct IActionController;
class IAction;

using IActionPtr = _smart_ptr<IAction>;
using FragmentID = int;

namespace user_code::animation {
	struct Weapon {
		Weapon() = default;

		template <typename Data>
		explicit Weapon(Data)
			:	tag_name_{ Data::animation_tag_name_ }
			,	cooldown_time_{ Data::cooldown_time_ }
		{}

		void initialize_animation(const SControllerDef& controller_definition, IActionController& action_controller);
		void update(const IEntity& entity, IActionController& action_controller);

		static constexpr auto FRAGMENT_ID_INVALID = -1;

		std::string_view tag_name_{};
		TimerId cooldown_timer_id_{};

		IActionPtr idle_action_{};
		IActionPtr shoot_action_{};
		FragmentID idle_fragment_id_{ FRAGMENT_ID_INVALID };
		FragmentID shoot_fragment_id_{ FRAGMENT_ID_INVALID };

		f32 cooldown_time_{};
	};
}	// namespace user_code::animation

#endif // !WEAPON_ANIMATON_H