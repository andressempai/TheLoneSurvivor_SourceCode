#ifndef COMPONENTS_DATA_H
#define COMPONENTS_DATA_H

#include <CryExtension/CryGUID.h>

#include <string_view>

using namespace std::string_view_literals;

constexpr auto character_category = "Characters"sv;
constexpr auto game_components_category = "Game Components"sv;
constexpr auto weapon_category = "Weapons"sv;
constexpr auto stage_elements_category = "Stage Elements"sv;

namespace ComponentData {
	struct PlayerEntityMesh {
		static constexpr auto guid = "{73ADC1B7-9AEF-411D-BA5E-4F0457EDB154}"_cry_guid;
		static constexpr auto category = character_category;
		static constexpr auto name = "playerentitymeshcomponent"sv;
		static constexpr auto label = "Player Entity Mesh"sv;
		static constexpr auto description = "Player Entity Mesh"sv;
	};

	struct PlayerEntityPhysic {
		static constexpr auto guid = "{799E034A-C31A-47F6-866D-4072B9D920DC}"_cry_guid;
		static constexpr auto category = character_category;
		static constexpr auto name = "playermeshphysiccomponent"sv;
		static constexpr auto label = "Player Entity Physic"sv;
		static constexpr auto description = "Player Entity Physic"sv;
	};

	struct PlayerBodyProxy {
		static constexpr auto guid = "{0597F23F-2973-45EE-9C45-457A41B4A691}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "playerbodyproxycomponent"sv;
		static constexpr auto label = "Player Body Proxy"sv;
		static constexpr auto description = "Player's Body Proxy"sv;
	};

	struct PlayerAnimation {
		static constexpr auto guid = "{F80B3809-71BD-47A9-AC27-D08443D129A7}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "playeranimationcomponent"sv;
		static constexpr auto label = "Player Animation Component"sv;
		static constexpr auto description = "Player's Animation Component"sv;
	};

	struct ZombieEntityMesh {
		static constexpr auto guid = "{A04DC505-8C97-436E-9C19-01F29623201A}"_cry_guid;
		static constexpr auto category = character_category;
		static constexpr auto name = "zombieentitymeshcomponent"sv;
		static constexpr auto label = "Zombie Entity Mesh"sv;
		static constexpr auto description = "Zombie Entity Mesh"sv;
	};

	struct ZombieEntityPhysic {
		static constexpr auto guid = "{45826519-5CB5-46F1-9AD8-C05CF9344B6C}"_cry_guid;
		static constexpr auto category = character_category;
		static constexpr auto name = "zombieentityphysiccomponent"sv;
		static constexpr auto label = "Zombie Entity Physic"sv;
		static constexpr auto description = "Zombie Entity Physic"sv;
	};

	struct ZombieBodyProxy {
		static constexpr auto guid = "{A8B0B263-90E0-42D3-A1F2-FE2CC079ECA8}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "zombiebodyproxycomponent"sv;
		static constexpr auto label = "Zombie Body Proxy"sv;
		static constexpr auto description = "Body Proxy for Zombies"sv;
	};

	struct PerderosEntityMesh {
		static constexpr auto guid = "{47E91601-9381-48A6-9064-1840E4B49C10}"_cry_guid;
		static constexpr auto category = character_category;
		static constexpr auto name = "perderosentitymeshcomponent"sv;
		static constexpr auto label = "Perderos Entity Mesh"sv;
		static constexpr auto description = "Perderos Entity Mesh"sv;
	};

	struct PerderosEntityPhysic {
		static constexpr auto guid = "{BDCB5B7E-DDD6-4A84-A4C8-B502638CE4A9}"_cry_guid;
		static constexpr auto category = character_category;
		static constexpr auto name = "perderosentityphysiccomponent"sv;
		static constexpr auto label = "Perderos Entity Physic"sv;
		static constexpr auto description = "Perderos Entity Physic"sv;
	};

	struct PerderosBodyProxy {
		static constexpr auto guid = "{5E499621-8A3E-4455-8AC1-0392CF913454}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "perderosbodyproxycomponent"sv;
		static constexpr auto label = "Perderos Body Proxy"sv;
		static constexpr auto description = "Perderos' Body Proxy"sv;
	};

	struct PistolAmmunition {
		static constexpr auto guid = "{37A35A55-4EAD-4D43-A67A-13AD2E8BDC39}"_cry_guid;
		static constexpr auto category = stage_elements_category;
		static constexpr auto name = "pistolammunitioncomponent"sv;
		static constexpr auto label = "Pistol Ammunition"sv;
		static constexpr auto description = "Pistol Ammunition inside world."sv;
	};

	struct ShotgunAmmunition {
		static constexpr auto guid = "{8EF75C96-0B98-42C4-A03E-971076C08CDB}"_cry_guid;
		static constexpr auto category = stage_elements_category;
		static constexpr auto name = "shotgunammunitioncomponent"sv;
		static constexpr auto label = "Shotgun Ammunition"sv;
		static constexpr auto description = "Shotgun Ammunition inside world"sv;
	};

	struct Pistol {
		static constexpr auto guid = "{9DB8D155-8AA0-4217-92F0-1333B08F8352}"_cry_guid;
		static constexpr auto category = weapon_category;
		static constexpr auto name = "pistolcomponent"sv;
		static constexpr auto label = "Pistol"sv;
		static constexpr auto description = "Pistol Component"sv;
	};

	struct PistolAnimation {
		static constexpr auto guid = "{6088BAF3-93A0-4179-ABDA-89AB7AAB6773}"_cry_guid;
		static constexpr auto category = weapon_category;
		static constexpr auto name = "pistolanimationcomponent"sv;
		static constexpr auto label = "Pistol Animation Component"sv;
		static constexpr auto description = "Pistol's Animation Component"sv;
	};

	struct AmericanShotgun {
		static constexpr auto guid = "{EDF3CF87-2FB1-4D34-9D02-4711952E442D}"_cry_guid;
		static constexpr auto category = weapon_category;
		static constexpr auto name = "americanshotguncomponent"sv;
		static constexpr auto label = "American Shotgun"sv;
		static constexpr auto description = "American Shotgun Component"sv;
	};

	struct ShotgunAnimation {
		static constexpr auto guid = "{2D400D96-0166-40A6-BE64-C7A4440CC65B}"_cry_guid;
		static constexpr auto category = weapon_category;
		static constexpr auto name = "shotgunanimationcomponent"sv;
		static constexpr auto label = "Shotgun Animation Component"sv;
		static constexpr auto description = "Shotgun's Animation Component"sv;
	};

	struct AmmunitionInventory {
		static constexpr auto guid = "{86A9237E-4367-4DE2-8084-83F52349E550}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "mainammunitioninventorycomponent"sv;
		static constexpr auto label = "Main Ammunition Inventory"sv;
		static constexpr auto description = "Use to represent the Ammunition Inventory for the Main Character."sv;
	};

	struct WeaponInventory {
		static constexpr auto guid = "{906A3D55-9CBF-45DB-A3FF-9B71786E681D}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "mainweaponinventorycomponent"sv;
		static constexpr auto label = "Main Weapon Inventory"sv;
		static constexpr auto description = "Use to represent the Weapon Inventory for the Main Character."sv;
	};

	struct MainMenu {
		static constexpr auto guid = "{984D6EB8-6492-41EA-9061-02F0AAA95B65}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "mainmenucomponent"sv;
		static constexpr auto label = "Main Menu"sv;
		static constexpr auto description = "Main Menu Component"sv;
	};

	struct PlayerHUD {
		static constexpr auto guid = "{EB75272F-F61C-42A1-9FBC-CC83ABA95FBD}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "playerhudcomponent"sv;
		static constexpr auto label = "Player HUD"sv;
		static constexpr auto description = "Player HUD Component"sv;
	};

	struct PauseMenu {
		static constexpr auto guid = "{524325B5-3A67-4892-8F75-AF50FF9153A6}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "pausemenucomponent"sv;
		static constexpr auto label = "Pause Menu"sv;
		static constexpr auto description = "Pause Menu Component"sv;
	};

	struct DeadMenu {
		static constexpr auto guid = "{6E88D270-BD5D-40FC-B9CD-9B5D421E3FDB}"_cry_guid;
		static constexpr auto category = game_components_category;
		static constexpr auto name = "deadmenucomponent"sv;
		static constexpr auto label = "Dead Menu"sv;
		static constexpr auto description = "Dead Menu Component"sv;
	};
}

#endif // !COMPONENTS_DATA_H