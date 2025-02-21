#ifndef AMMUNITION_TYPES_H
#define AMMUNITION_TYPES_H

#include "Components/DVEntity.h"
#include "Components/DynamicMesh/DynamicMesh.h"
#include "Components/Magazine/Magazine.h"
#include "Components/StoreToMagazine/StoreToMagazine.h"
#include "Utils/Tags.h"
#include "Datas/AmmunitionsData.h"
#include "Datas/ComponentsData.h"

namespace Types::Components {
	using NoChild = MP::type_list<>;
	using NoChildEventListener = MP::type_list<>;
	using AmmunitionComponentList = MP::type_list<DynamicMesh, Magazine, StoreToMagazine>;

	using PistolAmmunitionComponent = DVEntity<
			Tags::Pistol
		,	NoChild
		,	NoChildEventListener
		,	AmmunitionComponentList
		,	Data::PistolAmmunition
		,	ComponentData::PistolAmmunition
	>;
	
	using ShotgunAmmunitionComponent = DVEntity<
			Tags::AmericanShotgun
		,	NoChild
		,	NoChildEventListener
		,	AmmunitionComponentList
		,	Data::ShotgunAmmunition
		,	ComponentData::ShotgunAmmunition
	>;

	using DynamicVectorAmmunition = MP::type_list<PistolAmmunitionComponent, ShotgunAmmunitionComponent>;
}	// namespace Types::Components

#endif // !AMMUNITION_TYPES_H