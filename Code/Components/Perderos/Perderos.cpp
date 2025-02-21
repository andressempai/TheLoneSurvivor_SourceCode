#include "Perderos.h"
#include "../DVEntity.h"
#include "../LivingPhysic/LivingPhysic.h"
#include "../SkeletalMesh/SkeletalMesh.h"
#include "../Animation/PerderosAnimation.h"
#include "../AIBehaviorTree/AIBehaviorTree.h"
#include "../../Datas/CharacterData.h"
#include "../../Datas/ComponentsData.h"
#include "../../Types/BodyProxyTypes.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

void register_perderos_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Perderos));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_perderos_component);

void ReflectType(Schematyc::CTypeDesc<Perderos>& desc)
{
	desc.SetGUID("{FDB9E112-1C5B-4422-8922-5F7E2A204AF1}"_cry_guid);
	desc.SetEditorCategory("Characters");
	desc.SetName(Schematyc::CTypeName{ "perderoscomponent" });
	desc.SetLabel("Perderos");
	desc.SetDescription("Perderos Character");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::HiddenFromUser });
}

void Perderos::Initialize()
{
	using NoChild = MP::type_list<>;
	using NoChildEventListener = MP::type_list<>;
	using PerderosEntityMeshComponentList = MP::type_list<
			SkeletalMesh
		,	PerderosAnimation
		,	Types::Components::PerderosBodyProxyComponent
		,	AIBehaviorTree
	>;
	using PerderosEntityMesh = DVEntity<
			int
		,	NoChild
		,	NoChildEventListener
		,	PerderosEntityMeshComponentList
		,	Data::Perderos
		,	ComponentData::PerderosEntityMesh
	>;

	using PerderosEntityPhysicChild = MP::type_list<PerderosEntityMesh>;
	using PerderosEntityPhysicComponentList = MP::type_list<LivingPhysic, IEntityNavigationComponent>;
	using PerderosEntityPhysic = DVEntity<
			int
		,	PerderosEntityPhysicChild
		,	NoChildEventListener
		,	PerderosEntityPhysicComponentList
		,	Data::Perderos
		,	ComponentData::PerderosEntityPhysic
	>;

	m_pEntity->GetOrCreateComponentClass<PerderosEntityPhysic>();

	if (const auto navigation_component = DevelopmentTools::get_component_from_entity<IEntityNavigationComponent>(*m_pEntity)) {
		navigation_component->SetNavigationAgentType("MediumSizedCharacters");
		
		IEntityNavigationComponent::SMovementProperties movement_properties{};
		movement_properties.normalSpeed = 7.35f;
		movement_properties.minSpeed = 5.0f;
		movement_properties.maxSpeed = 7.5f;
		navigation_component->SetMovementProperties(movement_properties);

		IEntityNavigationComponent::SCollisionAvoidanceProperties collision_avoidance_properties{};
		collision_avoidance_properties.radius = 0.8f;
		collision_avoidance_properties.height = 2.0f;
		navigation_component->SetCollisionAvoidanceProperties(collision_avoidance_properties);

		navigation_component->SetStateUpdatedCallback([this](const Vec3& velocity)
		{
			move_entity_by_physics(*m_pEntity, velocity, gEnv->pTimer->GetFrameTime());
		});
	}
}