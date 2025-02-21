#include "EnemySpawner.h"
#include "../Zombie/Zombie.h"
#include "../Perderos/Perderos.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_enemy_spawner(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(EnemySpawner));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_enemy_spawner);

void ReflectType(Schematyc::CTypeDesc<EnemySpawner>& desc)
{
	desc.SetGUID("{7F824229-AE8A-4799-A093-55F624CDC2F9}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("enemyspawnercomponent"));
	desc.SetLabel("Enemy Spawner");
	desc.SetDescription("Spawns enemies at a set interval");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
			&EnemySpawner::enemy_to_spawn_
		,	'ensp'
		,	"enemytospawn"
		,	"Enemy to Spawn"
		,	"Enemy to Spawn at certain time"
		,	EnemySpawner::EnemyTypeContainer{}
	);

	desc.AddMember(
			&EnemySpawner::spawn_interval_
		,	'spin'
		,	"spawninterval"
		,	"Spawn Interval(Seconds)"
		,	"Time between enemy spawns"
		,	0.0f
	);

	desc.AddMember(
			&EnemySpawner::initial_delay_
		,	'idel'
		,	"initialdelay"
		,	"Initial Delay(Seconds)"
		,	"Delay before first spawn"
		,	0.0f
	);
}

bool Serialize(Serialization::IArchive& archive, EnemySpawner::EnemyTypeContainer& data, const char* name, const char* label)
{
	std::Serialize(archive, data.container_, name, label);

	return true;
}

Cry::Entity::EventFlags EnemySpawner::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::Update;
}

void EnemySpawner::ProcessEvent(const SEntityEvent& event)
{
	using namespace DevelopmentTools;

	if (event.event == Cry::Entity::EEvent::EditorPropertyChanged)
		time_since_spawn_ = initial_delay_;

	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	if (time_since_spawn_ <= 0.0f) {
		const auto current_enemy_index = make_random_number(static_cast<std::size_t>(0), enemy_to_spawn_.container_.size() - 1);

		auto spawn_params = SEntitySpawnParams{};
		spawn_params.vPosition = m_pEntity->GetWorldPos();
		spawn_params.qRotation = m_pEntity->GetWorldRotation();

		switch (enemy_to_spawn_.container_[current_enemy_index]) {
		case EnemyType::zombie:
			create_entity<Zombie>(spawn_params);
			break;
		case EnemyType::perderos:
			create_entity<Perderos>(spawn_params);
			break;
		}

		time_since_spawn_ = spawn_interval_;
	}
	else
		time_since_spawn_ -= event.fParam[0];
}