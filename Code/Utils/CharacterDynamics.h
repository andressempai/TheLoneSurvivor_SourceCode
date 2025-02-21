#ifndef CHARACTER_DYNAMICS_H
#define CHARACTER_DYNAMICS_H

#include <CrySchematyc/MathTypes.h>
#include <CryPhysics/physinterface.h>

class SCharacterDynamicsControls {
	friend struct SCharacterDynamics;

public:
	explicit SCharacterDynamicsControls(const Vec3& gravity) : gravity_{ gravity } {}

	SCharacterDynamicsControls& mass(float value)
	{
		mass_ = value;
		return *this;
	}

	SCharacterDynamicsControls& air_control(float value)
	{
		air_control_ = value;
		return *this;
	}

	SCharacterDynamicsControls& air_resistance(float value)
	{
		air_resistance_ = value;
		return *this;
	}

	SCharacterDynamicsControls& inertia(float value)
	{
		inertia_ = value;
		return *this;
	}

	SCharacterDynamicsControls& inertia_acceleration(float value)
	{
		inertia_acceleration_ = value;
		return *this;
	}

	SCharacterDynamicsControls& can_use_custom_gravity(bool value)
	{
		can_use_custom_gravity_ = value;
		return *this;
	}

private:
	Vec3 gravity_{};
	float mass_{};
	float air_control_{};
	float air_resistance_{};
	float inertia_{};
	float inertia_acceleration_{};
	bool can_use_custom_gravity_;
};

struct SCharacterDynamics {
	SCharacterDynamics() = default;
	explicit SCharacterDynamics(const SCharacterDynamicsControls& character_dynamics_controls)
		:	gravity_{ character_dynamics_controls.gravity_ }
		,	mass_{ character_dynamics_controls.mass_ }
		,	air_control_{ character_dynamics_controls.air_control_ }
		,	air_resistance_{ character_dynamics_controls.air_resistance_ }
		,	inertia_{ character_dynamics_controls.inertia_ }
		,	inertia_acceleration_{ character_dynamics_controls.inertia_acceleration_ }
		,	can_use_custom_gravity_{ character_dynamics_controls.can_use_custom_gravity_ }
	{}

	Vec3 gravity_{ ZERO };
	Schematyc::PositiveFloat mass_{};
	Schematyc::PositiveFloat air_control_{};
	Schematyc::PositiveFloat air_resistance_{};
	Schematyc::PositiveFloat inertia_{};
	Schematyc::PositiveFloat inertia_acceleration_{};
	bool can_use_custom_gravity_{};

	operator pe_player_dynamics() const
	{
		auto dynamics = pe_player_dynamics{};
		dynamics.gravity = gravity_;
		dynamics.mass = mass_;
		dynamics.kAirControl = air_control_;
		dynamics.kAirResistance = air_resistance_;
		dynamics.kInertia = inertia_;
		dynamics.kInertiaAccel = inertia_acceleration_;
		dynamics.minSlideAngle = 70.0f;
		dynamics.maxClimbAngle = 50.0f;
		dynamics.maxJumpAngle = 50.0f;
		dynamics.minFallAngle = 80.0f;
		dynamics.maxVelGround = 16.0f;

		return dynamics;
	}
};

static void ReflectType(Schematyc::CTypeDesc<SCharacterDynamics>& desc)
{
	desc.SetGUID("{D24DC360-CAE5-4D33-AE58-BDD1D6E85016}"_cry_guid);
	desc.AddMember(
			&SCharacterDynamics::can_use_custom_gravity_
		,	'cucg'
		,	"canusecustomgravity"
		,	"Can Use Custom Gravity"
		,	"Allow to use the custom gravity"
		,	true
	);

	desc.AddMember<Vec3>(
			&SCharacterDynamics::gravity_
		,	'grav'
		,	"gravity"
		,	"Gravity"
		,	"Charater's Custom Gravity"
		,	ZERO
	);

	desc.AddMember(
			&SCharacterDynamics::mass_
		,	'mass'
		,	"mass"
		,	"Mass"
		,	"Character's Mass in kilograms"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDynamics::air_control_
		,	'acon'
		,	"aircontrol"
		,	"Air Control"
		,	"Character's Air Control"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDynamics::air_resistance_
		,	'ares'
		,	"airresistance"
		,	"Air Resistance"
		,	"Character's Air Resistance"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDynamics::inertia_
		,	'iner'
		,	"deaccelerateinertia"
		,	"De Accelerate Inertia"
		,	"Character's Inertia coefficient"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDynamics::inertia_acceleration_
		,	'iacc'
		,	"inertiaacceleration"
		,	"Inertia Acceleration"
		,	"Character's Inertia Acceleration"
		,	Schematyc::PositiveFloat{}
	);
}

inline bool operator==(const SCharacterDynamics& lhs, const SCharacterDynamics& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(SCharacterDynamics)) == 0;
}

inline bool operator!=(const SCharacterDynamics& lhs, const SCharacterDynamics& rhs)
{
	return !(operator==(lhs, rhs));
}

#endif // !CHARACTER_DYNAMICS_H