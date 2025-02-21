#ifndef BLACKBOARD_KEYS_H
#define BLACKBOARD_KEYS_H

#include <CryEntitySystem/IEntityBasicTypes.h>

#include <string_view>

using namespace std::string_view_literals;

namespace BlackboardKey {
	struct EnemyIdBlackboardKey {
		static constexpr auto name_ = "enemy_id"sv;
		static constexpr auto default_value_ = INVALID_ENTITYID;
	};

	struct IsInAttackRangeBlackboardKey {
		static constexpr auto name_ = "is_in_attack_range"sv;
		static constexpr auto default_value_ = false;
	};

	struct CooldownCompletedBlackboardKey {
		static constexpr auto name_ = "cooldown_completed"sv;
		static constexpr auto default_value_ = true;
	};

	struct CanSearchBlackboardKey {
		static constexpr auto name_ = "can_search"sv;
		static constexpr auto default_value_ = true;
	};

	struct IsStunnedBlackboardKey {
		static constexpr auto name_ = "is_stunned"sv;
		static constexpr auto default_value_ = false;
	};
}	//!BlackboardKey

#endif // !BLACKBOARD_KEYS_H