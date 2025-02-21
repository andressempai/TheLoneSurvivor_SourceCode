#ifndef BUOYANCY_PARAMS_H
#define BUOYANCY_PARAMS_H

#include <CrySchematyc/MathTypes.h>
#include <CryPhysics/physinterface.h>

class BuoyancyParamsControls {
	friend struct BuoyancyParams;

public:
	BuoyancyParamsControls(float water_density) : water_density_{ water_density } {}

	BuoyancyParamsControls& water_damping(float value)
	{
		water_damping_ = value;
		return *this;
	}

	BuoyancyParamsControls& min_energy(float value)
	{
		water_resistance_ = value;
		return *this;
	}

private:
	float water_density_{};
	float water_damping_{};
	float water_resistance_{};
};

struct BuoyancyParams {
	BuoyancyParams() = default;
	explicit BuoyancyParams(const BuoyancyParamsControls& controls)
		:	water_density_{ controls.water_density_ }
		,	water_damping_{ controls.water_damping_ }
		,	water_resistance_{ controls.water_resistance_ }
	{}

	Schematyc::PositiveFloat water_density_{};
	Schematyc::PositiveFloat water_damping_{};
	Schematyc::PositiveFloat water_resistance_{};

	operator pe_params_buoyancy() const
	{
		auto buoyancy = pe_params_buoyancy{};
		buoyancy.waterDensity = water_density_.value;
		buoyancy.waterDamping = water_damping_.value;
		buoyancy.kwaterResistance = water_resistance_.value;
		return buoyancy;
	}
};

static void ReflectType(Schematyc::CTypeDesc<BuoyancyParams>& desc)
{
	desc.SetGUID("{A7C8DA1E-975E-49EE-9BD0-7FC9D52AB8E2}"_cry_guid);
	desc.SetLabel("Buoyancy Params");
	desc.AddMember(
			&BuoyancyParams::water_density_
		,	'wden'
		,	"waterdensity"
		,	"Water Density"
		,	"Water Density"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&BuoyancyParams::water_damping_
		,	'wdam'
		,	"waterdamping"
		,	"Water Damping"
		,	"Water Damping"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&BuoyancyParams::water_resistance_
		,	'wres'
		,	"waterresistance"
		,	"Water Resistance"
		,	"Water Resistance"
		,	Schematyc::PositiveFloat{}
	);
}

inline bool operator==(const BuoyancyParams& lhs, const BuoyancyParams& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

inline bool operator!=(const BuoyancyParams& lhs, const BuoyancyParams& rhs)
{
	return !(operator==(lhs, rhs));
}

#endif // !BUOYANCY_PARAMS_H