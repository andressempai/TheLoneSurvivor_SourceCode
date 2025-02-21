#ifndef BODY_PROXY_TYPE_H
#define BODY_PROXY_TYPE_H

#include "Components/BodyProxy.h"
#include "GameEvents/Body/SubscribeToAIDeadEvent.h"
#include "GameEvents/Body/SubscribeToAIExplosiveDamageEvent.h"
#include "GameEvents/Body/SubscribeToAIPointDamageEvent.h"
#include "GameEvents/Body/SubscribeToDeadEvent.h"
#include "GameEvents/Body/SubscribeToExplosiveDamageEvent.h"
#include "GameEvents/Body/SubscribeToHealEvent.h"
#include "GameEvents/Body/SubscribeToPointDamageEvent.h"
#include "GameEvents/Body/SubscribeToRepairArmorEvent.h"
#include "Datas/ComponentsData.h"

namespace Types::Components {
	using PlayerBodyProxyComponentEventToSubscribeList = MP::type_list<
			GameEvents::Body::SubscribeToPointDamageEvent
		,	GameEvents::Body::SubscribeToExplosiveDamageEvent
		,	GameEvents::Body::SubscribeToDeadEvent
		,	GameEvents::Body::SubscribeToHealEvent
		,	GameEvents::Body::SubscribeToRepairArmorEvent
	>;
	
	using ZombieBodyProxyComponentEventToSubscribeList = MP::type_list<
			GameEvents::Body::SubscribeToAIDeadEvent
		,	GameEvents::Body::SubscribeToAIPointDamageEvent<ZombiePelvis>
		,	GameEvents::Body::SubscribeToAIExplosiveDamageEvent<ZombiePelvis>
	>;
	
	using PerderosBodyProxyComponentEventToSubscribeList = MP::type_list<
			GameEvents::Body::SubscribeToAIDeadEvent
		,	GameEvents::Body::SubscribeToAIPointDamageEvent<PerderosPelvis>
		,	GameEvents::Body::SubscribeToAIExplosiveDamageEvent<PerderosPelvis>
	>;

	using PlayerBodyProxyComponent = BodyProxy<
			PlayerSpine
		,	ComponentData::PlayerBodyProxy
		,	PlayerBodyProxyComponentEventToSubscribeList
	>;

	using ZombieBodyProxyComponent = BodyProxy<
			ZombiePelvis
		,	ComponentData::ZombieBodyProxy
		,	ZombieBodyProxyComponentEventToSubscribeList
	>;

	using PerderosBodyProxyComponent = BodyProxy<
			PerderosPelvis
		,	ComponentData::PerderosBodyProxy
		,	PerderosBodyProxyComponentEventToSubscribeList
	>;
}	// namespace Types::Components

#endif // !BODY_PROXY_TYPE_H