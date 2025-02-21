#include "Zombie.h"
#include "../DVEntity.h"
#include "../LivingPhysic/LivingPhysic.h"
#include "../SkeletalMesh/SkeletalMesh.h"
#include "../Animation/ZombieAnimation.h"
#include "../AIBehaviorTree/AIBehaviorTree.h"
#include "../../Datas/CharacterData.h"
#include "../../Datas/ComponentsData.h"
#include "../../Types/BodyProxyTypes.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

void register_zombie_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Zombie));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_zombie_component);

void ReflectType(Schematyc::CTypeDesc<Zombie>& desc)
{
	desc.SetGUID("{BFBFD0A8-4940-40A0-BF06-5CA1B9FDC640}"_cry_guid);
	desc.SetEditorCategory("Characters");
	desc.SetName(Schematyc::CTypeName{ "zombiecomponent" });
	desc.SetLabel("Zombie");
	desc.SetDescription("Zombie Character");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::HiddenFromUser });
}

void Zombie::Initialize()
{
	using NoChild = MP::type_list<>;
	using NoChildEventListener = MP::type_list<>;
	using ZombieEntityMeshComponentList = MP::type_list<
			SkeletalMesh
		,	ZombieAnimation
		,	Types::Components::ZombieBodyProxyComponent
		,	AIBehaviorTree
	>;
	using ZombieEntityMesh = DVEntity<
			int
		,	NoChild
		,	NoChildEventListener
		,	ZombieEntityMeshComponentList
		,	Data::Zombie
		,	ComponentData::ZombieEntityMesh
	>;

	using ZombieEntityPhysicChild = MP::type_list<ZombieEntityMesh>;
	using ZombieEntityPhysicComponentList = MP::type_list<LivingPhysic, IEntityNavigationComponent>;
	using ZombieEntityPhysic = DVEntity<
			int
		,	ZombieEntityPhysicChild
		,	NoChildEventListener
		,	ZombieEntityPhysicComponentList
		,	Data::Zombie
		,	ComponentData::ZombieEntityPhysic
	>;

	m_pEntity->GetOrCreateComponentClass<ZombieEntityPhysic>();

	if (const auto child = DevelopmentTools::get_child(*m_pEntity)) {
		BoomerShooterEventSystem::instance().subscribe<ApplyRootMotionEvent>(child->GetId(), [this](const EventBase&)
		{
			apply_root_motion_ = true;
		});

		BoomerShooterEventSystem::instance().subscribe<DontApplyRootMotionEvent>(child->GetId(), [this](const EventBase&)
		{
			apply_root_motion_ = false;
		});
	}

	if (const auto navigation_component = DevelopmentTools::get_component_from_entity<IEntityNavigationComponent>(*m_pEntity)) {
		navigation_component->SetNavigationAgentType("MediumSizedCharacters");
		
		IEntityNavigationComponent::SMovementProperties movement_properties{};
		movement_properties.normalSpeed = 0.85f;
		movement_properties.minSpeed = 0.1f;
		movement_properties.maxSpeed = 1.15f;
		navigation_component->SetMovementProperties(movement_properties);

		IEntityNavigationComponent::SCollisionAvoidanceProperties collision_avoidance_properties{};
		collision_avoidance_properties.radius = 0.8f;
		collision_avoidance_properties.height = 2.0f;
		navigation_component->SetCollisionAvoidanceProperties(collision_avoidance_properties);

		navigation_component->SetStateUpdatedCallback([this](const Vec3& velocity)
		{
			requested_velocity_ = velocity;
		});
	}
}

void Zombie::ProcessEvent(const SEntityEvent& event)
{
	auto out_velocity = DevelopmentTools::make_zero_vector3();
	if (const auto child = DevelopmentTools::get_child(*m_pEntity)) {
		if (apply_root_motion_) {
			if (const auto child = DevelopmentTools::get_child(*m_pEntity)) {
				if (const auto character_instance = get_character_from_entity(*child, 0)) {
					out_velocity = child->GetWorldRotation() * character_instance->GetISkeletonAnim()->GetCurrentVelocity();
				}
			}
		}
		else {
			out_velocity = requested_velocity_;
		}
	}

	DevelopmentTools::move_entity_by_physics(*m_pEntity, out_velocity);
}

Cry::Entity::EventFlags Zombie::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}