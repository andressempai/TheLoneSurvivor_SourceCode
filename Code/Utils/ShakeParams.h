#ifndef SHAKE_PARAMS_H
#define SHAKE_PARAMS_H

#include <CrySchematyc/MathTypes.h>

struct ShakeParams {
	using PositiveFloatLow = Schematyc::Range<0, 2>;
	using PositiveFloatHigh = Schematyc::Range<0, 30>;

	PositiveFloatHigh pos_shake_frequency_{ 1.0f };
	PositiveFloatHigh rot_shake_frequency_{ 1.0f };
	PositiveFloatLow pos_shake_amplitude_{ 0.1f };
	PositiveFloatLow rot_shake_amplitude_{ 0.1f };
	PositiveFloatLow shake_duration_{ 0.5f };
};

static void ReflectType(Schematyc::CTypeDesc<ShakeParams>& desc)
{
	desc.SetGUID("{BFB12CD5-FA60-4271-A406-7EDFE5650797}"_cry_guid);
	desc.AddMember(
			&ShakeParams::pos_shake_frequency_
		,	'pfre'
		,	"posshakefrequency"
		,	"Position Shake Frequency"
		,	"Frequency of the Position Shake"
		,	1.0f
	);

	desc.AddMember(
			&ShakeParams::rot_shake_frequency_
		,	'rfre'
		,	"rotshakefrequency"
		,	"Rotation Shake Frequency"
		,	"Frequency of the Rotation Shake"
		,	1.0f
	);

	desc.AddMember(
			&ShakeParams::pos_shake_amplitude_
		,	'pamp'
		,	"postshakeamplitude"
		,	"Position Shake Amplitude"
		,	"Amplitude of the Position Shake"
		,	0.1f
	);

	desc.AddMember(
			&ShakeParams::rot_shake_amplitude_
		,	'ramp'
		,	"rotshakeamplitude"
		,	"Rotation Shake Amplitude"
		,	"Amplitude of the Rotation Shake"
		,	0.1f
	);

	desc.AddMember(
			&ShakeParams::shake_duration_
		,	'dur'
		,	"Shake Duration"
		,	"Shake Duration"
		,	"Duration of the Shake"
		,	0.5f
	);
}

static bool operator==(const ShakeParams& lhs, const ShakeParams& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(ShakeParams)) == 0;
}

static bool operator!=(const ShakeParams& lhs, const ShakeParams& rhs)
{
	return not (lhs == rhs);
}

#endif // !SHAKE_PARAMS_H