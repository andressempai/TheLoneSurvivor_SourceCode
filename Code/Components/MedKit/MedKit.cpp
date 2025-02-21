#include "Components/MedKit/MedKit.h"
#include "Components/DynamicMesh/DynamicMesh.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_medkit_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(MedKit));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_medkit_component);

void ReflectType(Schematyc::CTypeDesc<MedKit>& desc)
{
	desc.SetGUID("{8AEB0A72-A76D-4565-9C5C-5AFC9F124122}"_cry_guid);
	desc.SetEditorCategory("Stage Elements");
	desc.SetName(Schematyc::CTypeName("medkitcomponent"));
	desc.SetLabel("MedKit");
	desc.SetDescription("Use to represent MedKit inside world.");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&MedKit::health_pack_
		,	'hpac'
		,	"healthpack"
		,	"Health Pack"
		,	"Health Pack."
		,	large_health_pack
	);
}

void MedKit::Initialize()
{
	const auto mesh = m_pEntity->GetOrCreateComponent<DynamicMesh>();

	const auto simulation_params = SSimulationParams{
		CSimulationParamsControls{ DevelopmentTools::make_vector3(0.0f, 0.0f, -12.98f) }
		.mass(1.33f)
	};

	const auto mesh_params = SMeshParams{
			"objects/stage_elements/medkit/medical_kit_bag_tp.cgf"
		,	"objects/stage_elements/medkit/medical_kit_bag.mtl"
	};

	mesh->set_simulation_params(simulation_params);
	mesh->set_mesh_params(mesh_params);
	mesh->set_type(EDynamicMeshType::Rigid);
	mesh->load_mesh();

	BoomerShooterEventSystem::instance().subscribe<CollectEvent>(GetEntityId(), [this](const EventBase& event)
	{
		const auto& collect_event = static_cast<const CollectEvent&>(event);
		collect(collect_event.collector_.get());
	});
}

void MedKit::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<CollectEvent>(GetEntityId());
}

Cry::Entity::EventFlags MedKit::GetEventMask() const
{
	return Cry::Entity::EEvent::Remove;
}

void MedKit::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;

	DevelopmentTools::play_2d_sound("medkit_medium");
}

void MedKit::collect(IEntity& other_entity)
{
	BoomerShooterEventSystem::instance().send(other_entity.GetId(), HealEvent{ *m_pEntity, health_pack_ });
}