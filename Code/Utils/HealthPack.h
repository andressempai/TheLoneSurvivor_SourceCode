#ifndef HEALTH_PACK_H
#define HEALTH_PACK_H

#include <CrySchematyc/Reflection/TypeDesc.h>

class HealthPack {
public:
	static void ReflectType(Schematyc::CTypeDesc<HealthPack>& desc)
	{
		desc.SetGUID("{5E65D446-F0F6-48E6-8618-3681E84B9FA9}"_cry_guid);
		desc.SetLabel("Health Pack");
		desc.SetDescription("Health Pack Configuration.");
		desc.AddMember(
				&HealthPack::health_
			,	'hea'
			,	"healthamount"
			,	"Health Amount."
			,	"Maximun Health Amount."
			,	0
		);
	}

	constexpr HealthPack() : HealthPack(0) {}
	explicit constexpr HealthPack(int32 max, int32 health = 0)
		:	max_{ max }
		,	health_{ health > 0 and health <= max ? health : max }
	{}

	int32 harm(int32 amount)
	{
		health_ -= amount;

		return amount;
	}

	int32 fill(HealthPack other)
	{
		health_ = crymath::clamp(health_ + other.health_, 0, max_);

		return other.health_;
	}

	int32 health() const
	{
		return health_;
	}

	bool is_full() const
	{
		return health_ >= max_;
	}

	bool is_empty() const
	{
		return health_ <= 0;
	}

private:
	int32 max_;
	int32 health_;
};

inline bool operator==(const HealthPack& lhs, const HealthPack& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(HealthPack)) == 0;
}

inline bool operator!=(const HealthPack& lhs, const HealthPack& rhs)
{
	return !(operator==(lhs, rhs));
}

constexpr auto small_health_pack = HealthPack{ 10 };
constexpr auto medium_health_pack = HealthPack{ 25 };
constexpr auto large_health_pack = HealthPack{ 50 };
constexpr auto extra_large_health_pack = HealthPack{ 100 };
constexpr auto ultra_large_health_pack = HealthPack{ 180 };

#endif // !HEALTH_PACK_H