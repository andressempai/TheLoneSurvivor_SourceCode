#ifndef ANIMATION_TYPE_H
#define ANIMATION_TYPE_H

#include "Components/TAnimation.h"
#include "UserCode/Animation/PlayerAnimation.h"
#include "UserCode/Animation/Weapon.h"
#include "GameEvents/Animation/SubscribeToEquipWeaponEvent.h"
#include "GameEvents/Animation/SubscribeToSwitchWeaponEvent.h"
#include "GameEvents/Animation/SubscribeToRecoilEvent.h"
#include "GameEvents/Animation/SubscribeToPullTriggerEvent.h"
#include "GameEvents/Animation/SubscribeToBeforeSwitchWeaponEvent.h"
#include "Datas/CharacterData.h"
#include "Datas/WeaponsData.h"
#include "Datas/ComponentsData.h"

namespace Types::Components {
	using PlayerAnimationEventToSubscribeList = MP::type_list<
			GameEvents::Animation::SubscribeToEquipWeaponEvent
		,	GameEvents::Animation::SubscribeToSwitchWeaponEvent
		,	GameEvents::Animation::SubscribeToRecoilEvent
	>;

	using PlayerAnimationComponent = TAnimation<
			Data::MaleArms
		,	user_code::animation::PlayerAnimation
		,	PlayerAnimationEventToSubscribeList
		,	ComponentData::PlayerAnimation
	>;

	using WeaponAnimationEventToSubscribeList = MP::type_list<
			GameEvents::Animation::SubscribeToPullTriggerEvent
		,	GameEvents::Animation::SubscribeToBeforeSwitchWeaponEvent
	>;

	using PistolAnimationComponent = TAnimation<
			Data::Pistol
		,	user_code::animation::Weapon
		,	WeaponAnimationEventToSubscribeList
		,	ComponentData::PistolAnimation
	>;

	using ShotgunAnimationComponent = TAnimation<
			Data::AmericanShotgun
		,	user_code::animation::Weapon
		,	WeaponAnimationEventToSubscribeList
		,	ComponentData::ShotgunAnimation
	>;
}	// namespace Types::Components

#endif // !ANIMATION_TYPE_H