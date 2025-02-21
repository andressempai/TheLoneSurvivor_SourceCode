#ifndef INVENTORY_TYPES_H
#define INVENTORY_TYPES_H

#include "Components/InventoryController/InventoryController.h"
#include "Utils/WeaponBox.h"
#include "Utils/Tags.h"
#include "Datas/ComponentsData.h"

namespace Types::Components {
	using PistolWeaponBox = WeaponBox<Tags::Pistol>;
	using ShotgunWeaponBox = WeaponBox<Tags::AmericanShotgun>;
	using WeaponInventoryComponent = InventoryController<ComponentData::WeaponInventory, PistolWeaponBox, ShotgunWeaponBox>;
}	// namespace Types::Components

#endif // !INVENTORY_TYPES_H