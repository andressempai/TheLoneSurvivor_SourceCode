#include "ExplosiveBarrel.h"
#include "../DynamicMesh/DynamicMesh.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <algorithm>

constexpr auto hardness = 50;
constexpr auto time_before_explosion = 3000;
constexpr auto damage = 300.0f;

void register_explosive_barrel_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(ExplosiveBarrel));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_explosive_barrel_component);

void ReflectType(Schematyc::CTypeDesc<ExplosiveBarrel>& desc)
{
	desc.SetGUID("{BDD5EF32-AF3F-4840-92EE-3E2A4B434531}"_cry_guid);
	desc.SetEditorCategory("Stage Elements");
	desc.SetName(Schematyc::CTypeName("explosivebarrel"));
	desc.SetLabel("Explosive Barrel");
	desc.SetDescription("Explosive Barrel inside World.");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
			&ExplosiveBarrel::hardness_
		,	'hard'
		,	"hardness"
		,	"Hardness"
		,	"Hardness of the Barrel"
		,	hardness
	);

	desc.AddMember(
			&ExplosiveBarrel::time_before_explosion_
		,	'time'
		,	"timebeforeexplosion"
		,	"Time Before Explosion"
		,	"Time before explosion (must be in milliseconds)"
		,	time_before_explosion
	);

	desc.AddMember(
			&ExplosiveBarrel::damage_
		,	'dama'
		,	"damage"
		,	"Damage"
		,	"The damage made it by Barrel when this explode"
		,	damage
	);
}

ExplosiveBarrel::ExplosiveBarrel()
	:	hardness_{ hardness }
	,	time_before_explosion_{ time_before_explosion }
	,	damage_{ damage }
{}

void ExplosiveBarrel::Initialize()
{
	BoomerShooterEventSystem::instance().subscribe<PointDamageEvent>(GetEntityId(), [this](const EventBase& event)
	{
		const auto& point_damage_event = static_cast<const PointDamageEvent&>(event);
		impact_damage(point_damage_event.impacts_points_, point_damage_event.impact_direction_, point_damage_event.damage_);
	});

	BoomerShooterEventSystem::instance().subscribe<ExplosiveDamageEvent>(GetEntityId(), [this](const EventBase& event)
	{
		const auto& damage_event = static_cast<const ExplosiveDamageEvent&>(event);
		explosion_damage(damage_event.damage_);
	});

	if (not m_pEntity->GetComponent<DynamicMesh>()) {
		auto* const mesh = m_pEntity->CreateComponentClass<DynamicMesh>();

		const auto simulation_params = SSimulationParams{
			CSimulationParamsControls(DevelopmentTools::make_vector3(0.0f, 0.0f, -19.8f)).mass(21.0f).friction(0.85f)
		};

		const auto mesh_params = SMeshParams{
				"objects/props/explosive_barrel/explosive_barrel.cgf"
			,	"objects/props/explosive_barrel/explosive_barrel.mtl"
			,	100
			,	55
		};

		mesh->set_simulation_params(simulation_params);
		mesh->set_mesh_params(mesh_params);
		mesh->set_type(EDynamicMeshType::Rigid);
		mesh->load_mesh();
	}
}

void ExplosiveBarrel::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<PointDamageEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<ExplosiveDamageEvent>(GetEntityId());
}

Cry::Entity::EventFlags ExplosiveBarrel::GetEventMask() const
{
	return Cry::Entity::EEvent::TimerExpired | Cry::Entity::EEvent::Remove;
}

void ExplosiveBarrel::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event) {
	case Cry::Entity::EEvent::TimerExpired:
		DevelopmentTools::destroy_entity_by_id(GetEntityId());
	break;

	case Cry::Entity::EEvent::Remove:
	{
		make_explosion(*m_pEntity, damage_);
		m_pEntity->KillAllTimers(this);
	}
	break;
	}
}

void ExplosiveBarrel::impact_damage(const std::vector<Vec3>& impact_points, const Vec3& impact_direction, int32 damage)
{	
	if (hardness_ <= 0)
		return;

	auto out_impact_point = DevelopmentTools::make_zero_vector3();
	auto try_apply_damage = [this, &out_impact_point, damage](const Vec3& impact_point)
	{
		hardness_ -= damage;
		out_impact_point = impact_point;

		return hardness_ <= 0;
	};

	if (std::find_if(impact_points.begin(), impact_points.end(), try_apply_damage) != impact_points.end())
			m_pEntity->SetTimer(this, GetEntityId(), GetGUID(), 'expl', time_before_explosion_);

	DevelopmentTools::impulse_entity_by_physics(*m_pEntity, out_impact_point, impact_direction.normalized() * 21.0f * 1.55f);
}

void ExplosiveBarrel::explosion_damage(int damage)
{
	if (hardness_ -= damage; hardness_ <= 0)
		m_pEntity->SetTimer(this, GetEntityId(), GetGUID(), 'expl', time_before_explosion_);
}