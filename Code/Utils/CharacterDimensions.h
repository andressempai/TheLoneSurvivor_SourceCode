#ifndef CHARACTER_DIMENSIONS_H
#define CHARACTER_DIMENSIONS_H

#include <CrySchematyc/MathTypes.h>
#include <CryPhysics/physinterface.h>

class SCharacterDimensionsControls {
	friend struct SCharacterDimensions;

public:
	SCharacterDimensionsControls(float height) : height_{ height } {}

	SCharacterDimensionsControls& radius(float rad)
	{
		radius_ = rad;
		return *this;
	}

	SCharacterDimensionsControls& height_eyes(float heyes)
	{
		height_eyes_ = heyes;
		return *this;
	}

	SCharacterDimensionsControls& ground_contact_epsilon(float gce)
	{
		ground_contact_epsilon_ = gce;
		return *this;
	}

	SCharacterDimensionsControls& can_use_capsule(bool cuc)
	{
		can_use_capsule_ = cuc;
		return *this;
	}

private:
	float height_{};
	float radius_{};
	float height_eyes_{};
	float ground_contact_epsilon_{};
	bool can_use_capsule_{};
};

struct SCharacterDimensions {
	SCharacterDimensions() = default;
	explicit SCharacterDimensions(const SCharacterDimensionsControls& character_dimensions_constrols)
		:	height_{ character_dimensions_constrols.height_ }
		,	radius_{ character_dimensions_constrols.radius_ }
		,	height_eyes_{ character_dimensions_constrols.height_eyes_ }
		,	ground_contact_epsilon_{ character_dimensions_constrols.ground_contact_epsilon_ }
		,	can_use_capsule_{ character_dimensions_constrols.can_use_capsule_ }
	{}

	Schematyc::PositiveFloat height_{};
	Schematyc::PositiveFloat radius_{};
	Schematyc::PositiveFloat height_eyes_{};
	Schematyc::PositiveFloat ground_contact_epsilon_{};
	bool can_use_capsule_{};

	operator pe_player_dimensions() const
	{
		const auto collider_radius = radius_ * 0.5f;
		const auto collider_height = height_ * 0.25f;

		auto dimensions = pe_player_dimensions{};
		dimensions.bUseCapsule = can_use_capsule_;
		dimensions.sizeCollider = { collider_radius, 1.0f, collider_height };
		dimensions.heightPivot = 0.0f;
		dimensions.heightCollider = collider_radius + collider_height + 0.05f;
		dimensions.heightEye = height_eyes_;
		dimensions.groundContactEps = ground_contact_epsilon_;

		return dimensions;
	}
};

static void ReflectType(Schematyc::CTypeDesc<SCharacterDimensions>& desc)
{
	desc.SetGUID("{A774B558-E79D-4460-BBCA-4888584B5F2B}"_cry_guid);
	desc.AddMember(
			&SCharacterDimensions::can_use_capsule_
		,	'cuc'
		,	"canusecapsule"
		,	"Can Use Capsule"
		,	"Allo to use Capsule in Phycical Entity Dimensions"
		,	false
	);

	desc.AddMember(
			&SCharacterDimensions::height_
		,	'hei'
		,	"height"
		,	"Height"
		,	"Character's Height"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDimensions::radius_
		,	'rad'
		,	"radius"
		,	"Radius"
		,	"Character's Radius"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDimensions::height_eyes_
		,	'heie'
		,	"heighteyes"
		,	"Height Eyes"
		,	"Character's Height Eyes"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&SCharacterDimensions::ground_contact_epsilon_
		,	'gce'
		,	"groundcontactepsilon"
		,	"Ground Contact Epsilon"
		,	"Character's Ground Contact Epsilon"
		,	Schematyc::PositiveFloat{}
	);
}

inline bool operator==(const SCharacterDimensions& lhs, const SCharacterDimensions& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(SCharacterDimensions)) == 0;
}

inline bool operator!=(const SCharacterDimensions& lhs, const SCharacterDimensions& rhs)
{
	return !(operator==(lhs, rhs));
}

#endif // !CHARACTER_DIMENSIONS_H