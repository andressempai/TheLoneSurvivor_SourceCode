#include "Components/BulletproofVest/BulletproofVest.h"
#include "Components/DynamicMesh/DynamicMesh.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_bulletproof_vest_componenent(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(BulletproofVest));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_bulletproof_vest_componenent);

void ReflectType(Schematyc::CTypeDesc<BulletproofVest>& desc)
{
	desc.SetGUID("{47CED89F-8E61-40AB-879E-7D6E580DAB39}"_cry_guid);
	desc.SetEditorCategory("Stage Elements");
	desc.SetName(Schematyc::CTypeName("bulletproofvestcomponent"));
	desc.SetLabel("Bulletproof Vest");
	desc.SetDescription("Use to represent Bulletproof Vest inside World.");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&BulletproofVest::armor_
		,	'armo'
		,	"armor"
		,	"Armor"
		,	"Armor Configuration."
		,	LargeArmor
	);
}

void BulletproofVest::Initialize()
{
	const auto mesh = m_pEntity->GetOrCreateComponent<DynamicMesh>();
	mesh->set_mesh_params(
		SMeshParams{ "objects/stage_elements/armor/armor.cgf", "objects/stage_elements/armor/armor.mtl", 150, 50 }
	);
	mesh->load_mesh();

	BoomerShooterEventSystem::instance().subscribe<CollectEvent>(GetEntityId(), [this](const EventBase& event)
	{
		const auto& collect_event = static_cast<const CollectEvent&>(event);
		collect(collect_event.collector_.get());
	});
}

void BulletproofVest::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<CollectEvent>(GetEntityId());
}

Cry::Entity::EventFlags BulletproofVest::GetEventMask() const
{
	return Cry::Entity::EEvent::Remove;
}

void BulletproofVest::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;

	DevelopmentTools::play_2d_sound("armor_chest");
}

void BulletproofVest::collect(IEntity& other_entity)
{
	BoomerShooterEventSystem::instance().send(other_entity.GetId(), RepairArmorEvent{ *m_pEntity, armor_ });
}