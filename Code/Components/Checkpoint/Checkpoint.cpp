#include "Checkpoint.h"
#include "../MaleArms/MaleArms.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_player_start_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCheckpoint));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_player_start_component);

void ReflectType(Schematyc::CTypeDesc<CCheckpoint>& desc)
{
	desc.SetGUID("{EC44DD79-73AA-4590-BB5C-4D1C66E83DFF}"_cry_guid);
	desc.SetEditorCategory("Stage Elements");
	desc.SetName(Schematyc::CTypeName("playerstartcomponent"));
	desc.SetLabel("Player Start");
	desc.SetDescription("Use this Component to Spawn The Main Player.");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
}

Cry::Entity::EventFlags CCheckpoint::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted;
}

void CCheckpoint::ProcessEvent(const SEntityEvent& event)
{
	auto spawn_params = SEntitySpawnParams{};
	spawn_params.vPosition = m_pEntity->GetWorldPos();
	spawn_params.qRotation = m_pEntity->GetWorldRotation();
	spawn_params.sName = "male_arms";

	DevelopmentTools::create_entity<MaleArms>(spawn_params);
}