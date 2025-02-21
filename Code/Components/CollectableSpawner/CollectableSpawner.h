#ifndef COLLECTABLE_SPAWNER_H
#define COLLECTABLE_SPAWNER_H

#include "TimerManager.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

enum class CollectableType {
		pistol_ammunition
	,	american_shotgun_ammunition
	,	armor
	,	medKit
};

static void ReflectType(Schematyc::CTypeDesc<CollectableType>& desc) {
	desc.SetGUID("{6D6866E0-047B-4BA6-84EA-99C24B55B74F}"_cry_guid);
	desc.AddConstant(CollectableType::pistol_ammunition, "pistolammunition", "Pistol Ammunition");
	desc.AddConstant(CollectableType::american_shotgun_ammunition, "americanshotgunammunition", "American Shotgun Ammunition");
	desc.AddConstant(CollectableType::armor, "armor", "Armor");
	desc.AddConstant(CollectableType::medKit, "medKit", "MedKit");
}

class CollectableSpawner final : public IEntityComponent, public IEntityEventListener {
public:
	friend void ReflectType(Schematyc::CTypeDesc<CollectableSpawner>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
	void OnEntityEvent(IEntity* pEntity, const SEntityEvent& event) override;

	void spawn();

	static constexpr auto default_time_to_spawn = 60.0f;

	TimerId timer_id_{};
	Schematyc::PositiveFloat time_to_spawn_{ default_time_to_spawn };
	CollectableType collectable_type_{};
};

#endif // !COLLECTABLE_SPAWNER_H