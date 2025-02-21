#ifndef CAMERA_INFO_H
#define CAMERA_INFO_H

#include <CrySchematyc/MathTypes.h>

class SCameraInfoControls {
	friend struct SCameraInfo;

public:
	explicit SCameraInfoControls(float fov) : fov_{ fov } {}

	SCameraInfoControls& near_plane(float value)
	{
		near_plane_ = value;
		return *this;
	}

	SCameraInfoControls& far_plane(float value)
	{
		far_plane_ = value;
		return *this;
	}

	SCameraInfoControls& min_pitch_rotation(float min)
	{
		min_pitch_rotation_ = min;
		return *this;
	}

	SCameraInfoControls& max_pitch_rotation(float max)
	{
		max_pitch_rotation_ = max;
		return *this;
	}

	SCameraInfoControls& mouse_sensitivity(float sensitivity)
	{
		mouse_sensitivity_ = sensitivity;
		return *this;
	}

private:
	float fov_{};
	float near_plane_{};
	float far_plane_{};
	float min_pitch_rotation_{};
	float max_pitch_rotation_{};
	float mouse_sensitivity_{};
};

struct SCameraInfo {
	using FOVAngle = CryTransform::CClampedAngle<20, 360>;
	using RotationAngle = CryTransform::CClampedAngle<>;
	using ClipPlane = Schematyc::Range<0, 32768>;
	using MouseSensitivity = Schematyc::Range<0, 2>;

	SCameraInfo() = default;
	SCameraInfo(const SCameraInfoControls controls)
		:	fov_{ FOVAngle::FromDegrees(controls.fov_) }
		,	near_plane_{ controls.near_plane_ }
		,	far_plane_{ controls.far_plane_ }
		,	min_pitch_rotation_{ RotationAngle::FromDegrees(controls.min_pitch_rotation_) }
		,	max_pitch_rotation_{ RotationAngle::FromDegrees(controls.max_pitch_rotation_) }
		,	mouse_sensitivity_{ controls.mouse_sensitivity_ }
	{}

	FOVAngle fov_{};
	ClipPlane near_plane_{};
	ClipPlane far_plane_{};
	RotationAngle min_pitch_rotation_{};
	RotationAngle max_pitch_rotation_{};
	MouseSensitivity mouse_sensitivity_{};
};

static void ReflectType(Schematyc::CTypeDesc<SCameraInfo>& desc)
{
	desc.SetGUID("{623F01DA-3A13-4BCE-AC90-CE6394A7CF4A}"_cry_guid);
	desc.SetLabel("Camera Information");
	desc.AddMember(
			&SCameraInfo::fov_
		,	'fov'
		,	"fieldofview"
		,	"Field of View"
		,	"Camera Field of View on degrees."
		,	SCameraInfo::FOVAngle{}
	);

	desc.AddMember(
			&SCameraInfo::near_plane_
		,	'npla'
		,	"nearplane"
		,	"Near Plane"
		,	"Camera Near Plane."
		,	SCameraInfo::ClipPlane{}
	);

	desc.AddMember(
			&SCameraInfo::far_plane_
		,	'fpla'
		,	"farlane"
		,	"Far Plane"
		,	"Camera Far Plane."
		,	SCameraInfo::ClipPlane{}
	);

	desc.AddMember(
			&SCameraInfo::min_pitch_rotation_
		,	'minp'
		,	"minpithrotation"
		,	"Min Pitch Rotation"
		,	"Min Pitch Rotation(In Degrees)."
		,	SCameraInfo::RotationAngle{}
	);

	desc.AddMember(
			&SCameraInfo::max_pitch_rotation_
		,	'maxp'
		,	"maxpithrotation"
		,	"Max Pitch Rotation"
		,	"Max Pitch Rotation(In Degrees)."
		,	SCameraInfo::RotationAngle{}
	);

	desc.AddMember(
			&SCameraInfo::mouse_sensitivity_
		,	'msen'
		,	"mousesensitivity"
		,	"Mouse Sensitivity"
		,	"How strong is the Sensitivity of the Mouse."
		,	SCameraInfo::MouseSensitivity{}
	);
}

inline bool operator==(const SCameraInfo& lhs, const SCameraInfo& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

inline bool operator!=(const SCameraInfo& lhs, const SCameraInfo& rhs)
{
	return !(operator==(lhs, rhs));
}

#endif // !CAMERA_INFO_H