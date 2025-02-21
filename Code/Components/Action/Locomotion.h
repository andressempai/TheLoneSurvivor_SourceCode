#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include "../../TimerManager.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAction/IActionMapManager.h>
#include <CrySchematyc/MathTypes.h>

#include <bitset>

class Locomotion final : public IEntityComponent, public IActionListener {
	enum struct EInputActions {
			move_forward
		,	move_back
		,	move_left
		,	move_right
		,	jumping
		,	crouching
		,	total_actions
	};

public:
	Locomotion() = default;

	template <typename Data>
	Locomotion(Data)
		:	walk_speed_{ Data::stand_speed_ }
		,	jump_height_{ Data::jump_force_ }
		,	jump_buffer_{ Data::jump_buffer_ }
		,	stand_eyes_height_{ Data::stand_height_eyes_ }
		,	stand_height_{ Data::stand_height_ }
		,	crouch_eyes_height_{ Data::crouch_height_eyes_ }
		,	crouch_height_{ Data::crouch_height_ }
		,	collider_radius_{ Data::radius_ }
		,	target_eyes_height_{ Data::stand_height_eyes_ }
		,	target_height_{ Data::stand_height_ }
	{}

	friend void ReflectType(Schematyc::CTypeDesc<Locomotion>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
	void OnAction(const ActionId& action, int activation_mode, float value) override;

#ifndef RELEASE
	struct IPersistantDebug* debugger_{};
#endif // !RELEASE

	std::bitset<static_cast<std::size_t>(EInputActions::total_actions)> input_actions_{};
	TimerId jump_buffer_timer_id_{};

	Schematyc::PositiveFloat walk_speed_{};
	Schematyc::PositiveFloat jump_height_{};
	Schematyc::PositiveFloat jump_buffer_{};
	Schematyc::PositiveFloat stand_eyes_height_{};
	Schematyc::PositiveFloat stand_height_{};
	Schematyc::PositiveFloat crouch_eyes_height_{};
	Schematyc::PositiveFloat crouch_height_{};
	Schematyc::PositiveFloat collider_radius_{};
	f32 target_eyes_height_{};
	f32 target_height_{};
	bool is_crouching_{};
};

#endif // !LOCOMOTION_H