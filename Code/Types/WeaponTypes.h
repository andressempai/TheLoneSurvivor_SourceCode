#ifndef WEAPON_TYPES_H
#define WEAPON_TYPES_H

#include "Components/DVEntity.h"
#include "Components/SkeletalMesh/SkeletalMesh.h"
#include "Components/Muzzle/Muzzle.h"
#include "Components/Magazine/Magazine.h"
#include "Components/MuzzleFlash/MuzzleFlash.h"
#include "Types/AnimationType.h"
#include "Utils/Tags.h"
#include "MetaProgramming.h"

namespace Types::Components {
	using NoChild = MP::type_list<>;
	using NoChildEventListener = MP::type_list<>;
	
	using PistolComponentList = MP::type_list<
			SkeletalMesh
		,	Components::PistolAnimationComponent
		,	Muzzle
		,	MuzzleFlash
		,	Magazine
	>;
	using PistolComponent = DVEntity<
			Tags::Pistol
		,	NoChild
		,	NoChildEventListener
		,	PistolComponentList
		,	Data::Pistol
		,	ComponentData::Pistol
	>;
	
	using ShotgunComponentList = MP::type_list<
			SkeletalMesh
		,	Components::ShotgunAnimationComponent
		,	Muzzle
		,	MuzzleFlash
		,	Magazine
	>;
	using AmericanShotgunComponent = DVEntity<
			Tags::AmericanShotgun
		,	NoChild
		,	NoChildEventListener
		,	ShotgunComponentList
		,	Data::AmericanShotgun
		,	ComponentData::AmericanShotgun
	>;
	
	using DynamicVectorWeapons = MP::type_list<PistolComponent, AmericanShotgunComponent>;
}	// namespace Types

#endif // !WEAPON_TYPES_H