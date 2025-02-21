#include "Muzzle.h"
#include "../Magazine/Magazine.h"
#include "../MuzzleFlash/MuzzleFlash.h"
#include "../Shooter.h"
#include "../../Utils/DamageInfo.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_muzzle_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Muzzle));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_muzzle_component);

void ReflectType(Schematyc::CTypeDesc<Muzzle>& desc)
{
	desc.SetGUID("{A5511A8C-D3E8-4A3F-ACDD-AE5F071FA162}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("muzzlecomponent"));
	desc.SetLabel("Muzzle");
	desc.SetDescription("Allow Entities to Make Projectile(Single RayCast)");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&Muzzle::damage_
		,	'dama'
		,	"damage"
		,	"Damage per Projectile"
		,	"Damage to Apply per Projectile"
		,	Muzzle::PositiveIntMiddleRange{}
	);

	desc.AddMember(
			&Muzzle::cooldown_time_
		,	'cool'
		,	"cooldowntime"
		,	"Cooldown Timer"
		,	"Time to Cooldown"
		,	Muzzle::PositiveFloatLowRange{}
	);

	desc.AddMember(
			&Muzzle::spread_
		,	'spre'
		,	"spread"
		,	"Spread"
		,	"Spread"
		,	Muzzle::PositiveFloatLowRange{}
	);

	desc.AddMember(
			&Muzzle::number_of_projectiles_
		,	'npro'
		,	"numberofprojectiles"
		,	"Number of Projectiles"
		,	"Max Projectiles to Shoot"
		,	Muzzle::PositiveIntLowRange{}
	);

	desc.AddMember(
			&Muzzle::max_distance_
		,	'madi'
		,	"maxdistance"
		,	"Max Distance"
		,	"Max Distance to Shoot"
		,	Muzzle::PositiveFloatHighRange{}
	);

	desc.AddMember(
			&Muzzle::is_automatic_
		,	'iaut'
		,	"isautomatic"
		,	"Is Automatic"
		,	"Is Automatic"
		,	false
	);

	desc.AddMember(
			&Muzzle::shake_params_
		,	'shak'
		,	"shakeparams"
		,	"Shake Parameters"
		,	"Shake Parameters"
		,	ShakeParams{}
	);
}

void Muzzle::Initialize()
{
	BoomerShooterEventSystem::instance().subscribe<PullTriggerEvent>(GetEntityId(), [this](const EventBase&)
	{
		is_active_ = true;
		
		if (const auto parent_entity = DevelopmentTools::get_parent(*m_pEntity))
			BoomerShooterEventSystem::instance().send(parent_entity->GetId(), CancelReloadEvent{});
	});

	BoomerShooterEventSystem::instance().subscribe<ReleaseTriggerEvent>(GetEntityId(), [this](const EventBase&)
	{
		is_active_ = false;
	});
	
#ifndef RELEASE
	debugger_ = gEnv->pGameFramework->GetIPersistantDebug();
#endif // !RELEASE
}

void Muzzle::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<PullTriggerEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<ReleaseTriggerEvent>(GetEntityId());
}

Cry::Entity::EventFlags Muzzle::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void Muzzle::ProcessEvent(const SEntityEvent& event)
{
	using namespace DevelopmentTools;

	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	if (not is_active_)
		return;

	if (not is_automatic_)
		is_active_ = false;

	if (TimerManager::instance().is_timer_active(cooldown_timer_id_))
		return;

	const auto magazine = get_component_from_entity<Magazine>(*m_pEntity);
	if (not magazine or magazine->is_empty())
		return;

	const auto gunner_mesh = get_parent(*m_pEntity);
	if (not gunner_mesh)
		return;

	const auto gunner = get_parent(*gunner_mesh);
	if (not gunner)
		return;

	std::vector<DamageInformation> damage_information_vector{};
	damage_information_vector.reserve(number_of_projectiles_);

	std::vector<std::pair<short, EntityId>> possible_affected_entities{};
	possible_affected_entities.reserve(number_of_projectiles_);

	for (auto i = 0; i < number_of_projectiles_; ++i) {
		const auto spread_to_apply = make_vector3(
				make_random_number(-spread_.value, spread_.value)
			,	make_random_number(-spread_.value, spread_.value)
			,	make_random_number(-spread_.value, spread_.value)
		);
	
		const auto ray = [this, gunner, spread_to_apply]
		{
			if (const auto shooter = get_component_from_entity<Shooter>(*gunner)) {
				const auto shooter_view_point = shooter->get_view_point();
				return make_ray(
						shooter_view_point.position
					,	shooter_view_point.direction + spread_to_apply
					,	max_distance_
				);
			}
	
			return make_ray(gunner->GetWorldPos(), gunner->GetForwardDir(), max_distance_);
		}();
	
		auto ignored_entities = make_array(gunner_mesh->GetPhysicalEntity());
		const auto optional_hit = ray_cast(
			CRayWorldIntersectionControls{ ray }
			.ignored_entities(ignored_entities.data())
			.max_ignored_entities(ignored_entities.size())
			.max_hits(1)
			.entity_query(ent_static | ent_sleeping_rigid | ent_rigid | ent_independent | ent_terrain)
			.flags(rwi_stop_at_pierceable)
		);
		
		if (optional_hit) {
			const auto afected_entity = search_entity_by_physics(optional_hit->pCollider);
			if (not afected_entity)
				continue;

			const auto surface_type = get_surface_type(optional_hit->surface_idx);
			if (not surface_type)
				continue;

			const auto damage_to_apply = damage_ / number_of_projectiles_;
			if (damage_information_vector.empty()) {
				damage_information_vector.emplace_back(
						*afected_entity
					,	optional_hit->pt
					,	ray.direction_
					,	optional_hit->partid
					,	damage_to_apply
					,	number_of_projectiles_
				);
			}
			else {
				const auto predicate = [id = afected_entity->GetId()](const DamageInformation& damage_info)
				{
					return damage_info.afected_entity_.get().GetId() == id;
				};

				if (const auto in_vector = std::find_if(
						begin(damage_information_vector)
					,	end(damage_information_vector)
					,	predicate
				); in_vector == end(damage_information_vector)) {
					damage_information_vector.emplace_back(
							*afected_entity
						,	optional_hit->pt
						,	ray.direction_
						,	optional_hit->partid
						,	damage_to_apply
						,	number_of_projectiles_
					);
				}
				else {
					in_vector->impacts_points_.emplace_back(optional_hit->pt);
					in_vector->part_ids_.emplace_back(optional_hit->partid);
					in_vector->damage_ += damage_to_apply;
				}
			}

			const auto is_contained = [id = afected_entity->GetId()](const auto entity_and_surface_index)
			{
				return entity_and_surface_index.second == id;
			};
			const auto iter = std::find_if(possible_affected_entities.begin(), possible_affected_entities.end(), is_contained);
			if ((iter == possible_affected_entities.end()) or possible_affected_entities.empty())
				possible_affected_entities.push_back(std::make_pair(optional_hit->surface_idx, afected_entity->GetId()));

			if (const auto effect_id = get_runtime_effect_id("projectiles", optional_hit->surface_idx)) {
				auto runtime_effects_params = SMFXRunTimeEffectParams{};
				runtime_effects_params.pos = optional_hit->pt;
				runtime_effects_params.normal = optional_hit->n;
				execute_runtime_effect_id(effect_id, runtime_effects_params);
			}

#ifndef RELEASE
			if (const auto console_variable = gEnv->pConsole->GetCVar("dv_DebugRaycast")) {
				if (console_variable->GetIVal()) {
					debugger_->Begin("WeaponRayCast", false);
					debugger_->AddLine(ray.origin_, ray.origin_ + ray.direction_ + spread_to_apply, Col_Red, 5.0f);
					debugger_->AddSphere(optional_hit->pt, 0.2f, Col_Yellow, 5.0f);
				}
			}
#endif // !RELEASE
		}
	}

	if (const auto muzzle_flash = get_component_from_entity<MuzzleFlash>(*m_pEntity))
		muzzle_flash->start_muzzle_flash();

	const auto send_point_damage_event = [](const DamageInformation& damage_information)
	{
		BoomerShooterEventSystem::instance().send(
				damage_information.afected_entity_.get().GetId()
			,	PointDamageEvent{
						damage_information.impacts_points_
					,	damage_information.part_ids_
					,	damage_information.impact_direction_
					,	damage_information.afected_entity_
					,	damage_information.damage_
				}
		);
	};
	std::for_each(begin(damage_information_vector), end(damage_information_vector), send_point_damage_event);

	const auto play_impact_sound_effects = [](auto entity_and_surface)
	{
		if (const auto effect_id = get_runtime_effect_id("projectiles_sfx", entity_and_surface.first)) {
			auto runtime_effects_params = SMFXRunTimeEffectParams{};
			runtime_effects_params.audioProxyEntityId = entity_and_surface.second;
			execute_runtime_effect_id(effect_id, runtime_effects_params);
		}
	};
	std::for_each(begin(possible_affected_entities), end(possible_affected_entities), play_impact_sound_effects);

	BoomerShooterEventSystem::instance().send(gunner->GetId(), RecoilEvent{ shake_params_ });
	BoomerShooterEventSystem::instance().send(gunner_mesh->GetId(), RecoilEvent{});

	TimerManager::instance().make_timer(cooldown_timer_id_, cooldown_time_, false, nullptr);
}

IEntityComponent::ComponentEventPriority Muzzle::GetEventPriority() const
{
	return 11;
}