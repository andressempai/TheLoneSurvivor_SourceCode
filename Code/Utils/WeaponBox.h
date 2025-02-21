#ifndef WEAPON_BOX_H
#define WEAPON_BOX_H

#include <string_view>
#include <utility>

using EntityId = unsigned int;

template<typename Tag>
class WeaponBox {
public:
	WeaponBox() : WeaponBox("", INVALID_ENTITYID) {}
	WeaponBox(std::string_view thumbnail, EntityId weapon_id)
		:	thumbnail_{ std::move(thumbnail) }
		,	weapon_id_{ std::move(weapon_id) }
	{}

	EntityId weapon_id() const
	{
		return weapon_id_;
	}

private:
	std::string_view thumbnail_;
	EntityId weapon_id_;
};

#endif // !WEAPON_BOX_H