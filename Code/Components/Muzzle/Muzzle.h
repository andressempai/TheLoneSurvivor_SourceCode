#ifndef MUZZLE_H
#define MUZZLE_H

#include "../../Utils/ShakeParams.h"
#include "../../TimerManager.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

class Muzzle final : public IEntityComponent {
	using PositiveFloatLowRange = Schematyc::Range<0, 10>;
	using PositiveFloatHighRange = Schematyc::PositiveFloat;
	using PositiveIntLowRange = Schematyc::Range<0, 16, 0, 16, int32>;
	using PositiveIntMiddleRange = Schematyc::Range<0, 640, 0, 640, int32>;

public:
	Muzzle() = default;

	template <typename Data>
	Muzzle(Data)
		:	shake_params_{
					Data::pos_shake_frequency_
				,	Data::rot_shake_frequency_
				,	Data::pos_shake_amplitude_
				,	Data::rot_shake_amplitude_
				,	Data::shake_duration_
			}
		,	damage_{ Data::damage_ }
		,	cooldown_time_{ Data::cooldown_time_ }
		,	spread_{ Data::spread_ }
		,	number_of_projectiles_{ Data::number_of_projectiles_ }
		,	max_distance_{ Data::max_distance_ }
		,	is_automatic_{ Data::is_automatic_ }
	{}

	friend void ReflectType(Schematyc::CTypeDesc<Muzzle>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
	ComponentEventPriority GetEventPriority() const override;

#ifndef RELEASE
	struct IPersistantDebug* debugger_{};
#endif // !RELEASE

	ShakeParams shake_params_{};
	TimerId cooldown_timer_id_{};
	PositiveIntMiddleRange damage_{};
	PositiveFloatLowRange cooldown_time_{};
	PositiveFloatLowRange spread_{};
	PositiveIntLowRange number_of_projectiles_{};
	PositiveFloatHighRange max_distance_{ 100.0f };
	bool is_automatic_{};
	bool is_active_{};
};

#endif // !MUZZLE_H