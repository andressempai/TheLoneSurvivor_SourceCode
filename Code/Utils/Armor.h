#ifndef ARMOR_H
#define ARMOR_H

#include <CrySchematyc/Reflection/TypeDesc.h>

class Armor {
public:
	static void ReflectType(Schematyc::CTypeDesc<Armor>& desc)
	{
		desc.SetGUID("{261BE99B-E8D6-4FDA-B882-DCE8833A6756}"_cry_guid);
		desc.SetLabel("Armor");
		desc.SetDescription("Armor Configuration.");
		desc.AddMember(
				&Armor::hardness_
			,	'hard'
			,	"hardness"
			,	"Hardness"
			,	"Hardness."
			,	0
		);
	}

	constexpr Armor() : Armor(0){}
	explicit constexpr Armor(int32 hardness) : max_{ hardness }, hardness_{ hardness } {}

	int32 harm(int32 damage)
	{
		hardness_ -= damage;

		return int32(damage * 0.7f);
	}

	int32 repair(Armor other)
	{
		hardness_ = crymath::clamp(hardness_ + other.hardness_, 0, max_);

		return other.hardness_;
	}

	int32 hardness() const
	{
		return hardness_;
	}

	bool is_in_good_condition() const
	{
		return hardness_ >= max_;
	}

	bool is_broken() const
	{
		return hardness_ <= 0;
	}

private:
	int32 max_;
	int32 hardness_;
};

inline bool operator==(const Armor& lhs, const Armor& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(Armor)) == 0;
}

inline bool operator!=(const Armor& lhs, const Armor& rhs)
{
	return !(operator==(lhs, rhs));
}

constexpr auto SmallArmor = Armor{ 10 };
constexpr auto MediumArmor = Armor{ 50 };
constexpr auto LargeArmor = Armor{ 100 };

#endif // !ARMOR_H