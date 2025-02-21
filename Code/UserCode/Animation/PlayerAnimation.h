#ifndef PLAYER_ANIMATON_H
#define PLAYER_ANIMATON_H

#include <CryCore/BaseTypes.h>

struct IEntity;
struct SControllerDef;
struct IActionController;
class IAction;

template<class _I>
class _smart_ptr;

using IActionPtr = _smart_ptr<IAction>;
using TagID = int32;
using FragmentID = TagID;

namespace user_code::animation {
	struct PlayerAnimation {
		void initialize_animation(const SControllerDef& controller_definition, IActionController& action_controller);
		void update(const IEntity& entity, IActionController& action_controller);

		static constexpr auto FRAGMENT_ID_INVALID = -1;
		static constexpr auto TAG_ID_INVALID = -1;

		IActionPtr idle_action_{};
		IActionPtr walk_action_{};
		IActionPtr shoot_action_{};
		IActionPtr lower_weapon_action_{};
		IActionPtr raise_weapon_action_{};
		IActionPtr current_action_{};

		FragmentID idle_fragmend_id_{ FRAGMENT_ID_INVALID };
		FragmentID walk_fragmend_id_{ FRAGMENT_ID_INVALID };
		FragmentID shoot_fragment_id_{ FRAGMENT_ID_INVALID };
		FragmentID lower_weapon_fragment_id_{ FRAGMENT_ID_INVALID };
		FragmentID raise_weapon_fragment_id_{ FRAGMENT_ID_INVALID };
		TagID current_weapon_animation_tag_{ TAG_ID_INVALID };

		bool is_idle_{ true };
		bool is_switching_weapon_{};
	};
}	// namespace user_code::animation

#endif // !PLAYER_ANIMATON_H