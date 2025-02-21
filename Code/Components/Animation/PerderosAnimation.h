#ifndef PERDEROS_ANIMATION_H
#define PERDEROS_ANIMATION_H

#include "Animation.h"

#include <string_view>

class PerderosAnimation final : public Animation {
public:
	PerderosAnimation() = default;

	template <typename Data>
	PerderosAnimation(Data) : Animation{ Data{} } {}

	friend void ReflectType(Schematyc::CTypeDesc<PerderosAnimation>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void initialize_fragment() override;

#ifndef RELEASE
	struct IPersistantDebug* debugger_{};
#endif // !RELEASE

	IActionPtr action_{};
	IActionPtr hit_reaction_action_{};

	FragmentID idle_fragmend_id_{ FRAGMENT_ID_INVALID };
	FragmentID walk_fragmend_id_{ FRAGMENT_ID_INVALID };
	FragmentID melee_fragment_id_{ FRAGMENT_ID_INVALID };
	FragmentID hit_reaction_fwd_fragment_id_{ FRAGMENT_ID_INVALID };
	FragmentID dead_fragment_id_{ FRAGMENT_ID_INVALID };
	FragmentID current_fragment_id_{ FRAGMENT_ID_INVALID };

	bool is_reactiving_to_damage_{};
	bool is_dead_{};
};

#endif // !PERDEROS_ANIMATION_H