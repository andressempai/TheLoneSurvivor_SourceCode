#include "CollectableSpawner.h"
#include "Components/BulletproofVest/BulletproofVest.h"
#include "Components/MedKit/MedKit.h"
#include "Types/AmmunitionTypes.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_collectable_spawner_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(CollectableSpawner));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_collectable_spawner_component);

void ReflectType(Schematyc::CTypeDesc<CollectableSpawner>& desc)
{
	desc.SetGUID("{F1461415-DBEC-4D3E-AC99-DB217CBAD237}"_cry_guid);
	desc.SetEditorCategory("Stage Elements");
	desc.SetName(Schematyc::CTypeName("collectablespawnercomponent"));
	desc.SetLabel("Collectable Spawner");
	desc.SetDescription("Use this Component to Spawn Collectables Entities(Ammo, Armor and MedKit)");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
		&CollectableSpawner::collectable_type_
		, 'colt'
		, "collectabletype"
		, "Collectable Type"
		, "Type of Collectable to Spawn"
		, CollectableType::pistol_ammunition
	);

	desc.AddMember(
			&CollectableSpawner::time_to_spawn_
		,	'tisp'
		,	"timetospawn"
		,	"Time to Spawn"
		,	"Time to Spawn the Collectable"
		,	CollectableSpawner::default_time_to_spawn
	);
}

Cry::Entity::EventFlags CollectableSpawner::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted;
}

void CollectableSpawner::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	spawn();
}

void CollectableSpawner::OnEntityEvent(IEntity* pEntity, const SEntityEvent& event)
{
	TimerManager::instance().make_timer(timer_id_, time_to_spawn_, false, [this]() { spawn(); });
}

void CollectableSpawner::spawn()
{
	using namespace Types::Components;

	auto spawn_params = SEntitySpawnParams{};
	spawn_params.vPosition = m_pEntity->GetWorldPos();
	spawn_params.qRotation = m_pEntity->GetWorldRotation();

	auto collectable_entity = ObjectPointer<IEntity>{};
	switch (collectable_type_) {
	case CollectableType::pistol_ammunition:
		collectable_entity = DevelopmentTools::create_entity<PistolAmmunitionComponent>(spawn_params);
		break;
	case CollectableType::american_shotgun_ammunition:
		collectable_entity = DevelopmentTools::create_entity<ShotgunAmmunitionComponent>(spawn_params);
		break;
	case CollectableType::armor:
		collectable_entity = DevelopmentTools::create_entity<BulletproofVest>(spawn_params);
		break;
	case CollectableType::medKit:
		collectable_entity = DevelopmentTools::create_entity<MedKit>(spawn_params);
		break;
	}

	if (collectable_entity)
		collectable_entity->AddEventListener(Cry::Entity::EEvent::Remove, this);
}