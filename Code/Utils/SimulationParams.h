#ifndef SIMULATION_PARAMS_H
#define SIMULATION_PARAMS_H

#include <CrySchematyc/MathTypes.h>
#include <CryPhysics/physinterface.h>

class CSimulationParamsControls {
	friend struct SSimulationParams;

public:
	CSimulationParamsControls() = default;
	CSimulationParamsControls(Vec3 gravity) : gravity_{ gravity }{}

	CSimulationParamsControls& max_time_step(float value)
	{
		max_time_step_ = value;
		return *this;
	}

	CSimulationParamsControls& min_energy(float value)
	{
		min_energy_ = value;
		return *this;
	}

	CSimulationParamsControls& damping(float value)
	{
		damping_ = value;
		return *this;
	}

	CSimulationParamsControls& mass(float value)
	{
		mass_ = value;
		return *this;
	}

	CSimulationParamsControls& friction(float value)
	{
		friction_ = value;
		return *this;
	}

	CSimulationParamsControls& enable_physics(bool value)
	{
		enable_physics_ = value;
		return *this;
	}

	CSimulationParamsControls& use_custom_gravity(bool value)
	{
		use_custom_gravity_ = value;
		return *this;
	}

private:
	Vec3 gravity_{ ZERO };
	float max_time_step_{ 0.02f };
	float min_energy_{ 0.04f };
	float damping_{ 0.1f };
	float mass_{ 1.0f };
	float friction_{ 0.6f };
	bool enable_physics_{ true };
	bool use_custom_gravity_{ true };
};

struct SSimulationParams {
	SSimulationParams() : SSimulationParams(CSimulationParamsControls{}){}
	SSimulationParams(const CSimulationParamsControls& controls)
		:	gravity_{ controls.gravity_ }
		,	max_time_step_{ controls.max_time_step_ }
		,	min_energy_{ controls.min_energy_ }
		,	damping_{ controls.damping_ }
		,	mass_{ controls.mass_ }
		,	friction_{ controls.friction_ }
		,	enable_physics_{ controls.enable_physics_ }
		,	use_custom_gravity_{ controls.use_custom_gravity_ }
	{}

	Vec3 gravity_;
	Schematyc::PositiveFloat max_time_step_;
	Schematyc::PositiveFloat min_energy_;
	Schematyc::PositiveFloat damping_;
	Schematyc::PositiveFloat mass_;
	Schematyc::PositiveFloat friction_;
	bool enable_physics_;
	bool use_custom_gravity_;

	operator pe_simulation_params() const
	{
		auto simulation = pe_simulation_params{};
		simulation.maxTimeStep = max_time_step_;
		simulation.minEnergy = min_energy_;
		simulation.mass = mass_;
		simulation.density = -1.0f;
		simulation.damping = damping_;
		simulation.gravity = gravity_;
		simulation.gravityFreefall = gravity_;
		simulation.maxFriction = friction_;

		return simulation;
	}
};

static void ReflectType(Schematyc::CTypeDesc<SSimulationParams>& desc)
{
	desc.SetGUID("{824A03A9-149A-4C39-AFD1-659EDDD6AF17}"_cry_guid);
	desc.SetLabel("Simulation Params");
	desc.AddMember(
			&SSimulationParams::enable_physics_
		,	'enab'
		,	"enablephysics"
		,	"Enable Physics"
		,	"Enable Physics"
		,	true
	);

	desc.AddMember(
			&SSimulationParams::use_custom_gravity_
		,	'ucgr'
		,	"usecustomgravity"
		,	"Use Custom Gravity"
		,	"Use Custom Gravity"
		,	true
	);

	desc.AddMember(
			&SSimulationParams::gravity_
		,	'sgra'
		,	"gravity"
		,	"Gravity"
		,	"Gravity"
		,	ZERO
	);

	desc.AddMember(
			&SSimulationParams::max_time_step_
		,	'sste'
		,	"maxtimestep"
		,	"Max Time Step"
		,	"Max Time Step"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SSimulationParams::min_energy_
		,	'sene'
		,	"minenergy"
		,	"Min Energy"
		,	"Min Energy"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SSimulationParams::damping_
		,	'sdam'
		,	"damping"
		,	"Damping"
		,	"Damping"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SSimulationParams::mass_
		,	'smas'
		,	"mass"
		,	"Mass"
		,	"Mass in kilograms"
		,	0.0f
	);

	desc.AddMember(
			&SSimulationParams::friction_
		,	'sfri'
		,	"friction"
		,	"Friction"
		,	"Friction"
		,	Schematyc::PositiveFloat{}
	);
}

inline bool operator==(const SSimulationParams& lhs, const SSimulationParams& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

inline bool operator!=(const SSimulationParams& lhs, const SSimulationParams& rhs)
{
	return !(operator==(lhs, rhs));
}

#endif // !SIMULATION_PARAMS_H