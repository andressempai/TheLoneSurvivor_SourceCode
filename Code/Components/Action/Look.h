#ifndef LOOK_H
#define LOOK_H

#include "Utils/CameraInfo.h"
#include "Utils/ShakeParams.h"
#include "TimerManager.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CryAction/IActionMapManager.h>
#include <CryMath/Cry_Camera.h>

class Look final : public IEntityComponent, public IActionListener {
public:
	friend void ReflectType(Schematyc::CTypeDesc<Look>& desc);

	Look() = default;

	template <typename Data>
	Look(Data)
		:	camera_info_{
				SCameraInfoControls(Data::fov_)
				.near_plane(Data::near_plane_)
				.far_plane(Data::far_plane_)
				.min_pitch_rotation(Data::min_pitch_rotation_)
				.max_pitch_rotation(Data::max_pitch_rotation_)
				.mouse_sensitivity(Data::mouse_sensitivity_)
			}
		,	shake_params_{ 5.5f, 9.5f, 0.085f, 0.35f, 0.5f }
	{}

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
	void OnAction(const ActionId& action, int activation_mode, float value) override;

	static constexpr auto rotation_speed = 0.002f;

	CCamera main_{};
	SCameraInfo camera_info_{};
	ShakeParams shake_params_{};
	Vec3 camera_location_{ ZERO };
	Vec2 mouse_location_delta_{ ZERO };
	TimerId shake_timer_id_{};
	f32 perlin_noise_coordinate_{};
	f32 position_shake_noise_{};
	f32 rotation_shake_noise_{};
	bool is_shaking_{};
	bool is_dead_{};
};

#endif // !LOOK_H