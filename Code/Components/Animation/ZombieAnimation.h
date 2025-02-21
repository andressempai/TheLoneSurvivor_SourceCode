#ifndef ZOMBIE_ANIMATION_H
#define ZOMBIE_ANIMATION_H

#include "Animation.h"
#include "../../TimerManager.h"
#include "../../Utils/ObjectPointer.h"

#include <string_view>

class ZombieAnimation final : public Animation {
public:
	ZombieAnimation() = default;

	template <typename Data>
	ZombieAnimation(Data) : Animation{ Data{} } {}

	friend void ReflectType(Schematyc::CTypeDesc<ZombieAnimation>& desc);

	bool is_still_stunned() const
	{
		return is_still_stunned_;
	}

private:
	void Initialize() override;
	void OnShutDown() override;
	void ProcessEvent(const SEntityEvent& event) override;
	Cry::Entity::EventFlags GetEventMask() const override;

	void initialize_fragment() override;

#ifndef RELEASE
	ObjectPointer<struct IPersistantDebug> debugger_{};
#endif // !RELEASE

	IActionPtr idle_action_{};
	IActionPtr walk_action_{};
	IActionPtr stunned_action_{};
	IActionPtr attacking_action_{};

	TimerId attacking_timer_id_{};
	TimerId hit_reaction_timer_id_{};

	FragmentID idle_fragmend_id_{ FRAGMENT_ID_INVALID };
	FragmentID walk_fragmend_id_{ FRAGMENT_ID_INVALID };
	FragmentID melee_fragment_id_{ FRAGMENT_ID_INVALID };
	FragmentID hit_reaction_fwd_fragment_id_{ FRAGMENT_ID_INVALID };
	FragmentID dead_fragment_id_{ FRAGMENT_ID_INVALID };

	bool is_idle_{ true };
	bool is_attacking_{};
	bool is_stunned_{};
	bool is_dead_{};
	bool is_still_stunned_{};
};
#endif // !ZOMBIE_ANIMATION_H