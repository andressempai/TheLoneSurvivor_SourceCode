#include "ZombieAnimation.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_zombie_animation_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(ZombieAnimation));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_zombie_animation_component);

void ReflectType(Schematyc::CTypeDesc<ZombieAnimation>& desc)
{
	desc.AddBase<Animation>();
	desc.SetGUID("{CA6F62AB-98A4-498A-8EB5-0A30DEC0687A}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("zombieanimationcomponent"));
	desc.SetLabel("Zombie Animation");
	desc.SetDescription("This component is used to animate Zombie entities.");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&Animation::animation_data_
		,	'adat'
		,	"animationdata"
		,	"Animation Data"
		,	"Animation Data"
		,	Animation::AnimationData{}
	);
}

void ZombieAnimation::Initialize()
{
	Animation::Initialize();

	idle_action_ = DevelopmentTools::make_animation_action(priority::very_low, idle_fragmend_id_, IAction::Interruptable);
	action_controller_->Queue(*idle_action_);

	BoomerShooterEventSystem::instance().subscribe<DamageReactionEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (not stunned_action_ or not (stunned_action_->GetStatus() == IAction::Installed)) {
			is_stunned_ = true;
			is_still_stunned_ = true;

			BoomerShooterEventSystem::instance().send(GetEntityId(), ApplyRootMotionEvent{});

			TimerManager::instance().make_timer(hit_reaction_timer_id_, 3.31f, false, [this]
			{
				is_still_stunned_ = false;
				BoomerShooterEventSystem::instance().send(GetEntityId(), DontApplyRootMotionEvent{});
			});
		}
	});

	BoomerShooterEventSystem::instance().subscribe<MelleeAttackEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (not is_stunned_) {
			is_attacking_ = true;

			BoomerShooterEventSystem::instance().send(GetEntityId(), ApplyRootMotionEvent{});

			TimerManager::instance().make_timer(attacking_timer_id_, 2.15f, false, [this]
			{
				BoomerShooterEventSystem::instance().send(GetEntityId(), DontApplyRootMotionEvent{});
			});
		}
	});
	
	BoomerShooterEventSystem::instance().subscribe<DeadEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (is_dead_)
			return;
	
		if (attacking_action_)
			attacking_action_->ForceFinish();

		is_dead_ = true;
		DevelopmentTools::play_2d_sound("zombie_dead");
	});

#ifndef RELEASE
	debugger_ = gEnv->pGameFramework->GetIPersistantDebug();
#endif // !RELEASE
}

void ZombieAnimation::OnShutDown()
{
	Animation::OnShutDown();

	BoomerShooterEventSystem::instance().unsubscribe<DamageReactionEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<MelleeAttackEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<DeadEvent>(GetEntityId());

	TimerManager::instance().remove_timer(attacking_timer_id_);
	TimerManager::instance().remove_timer(hit_reaction_timer_id_);
}

void ZombieAnimation::ProcessEvent(const SEntityEvent& event)
{
	using namespace DevelopmentTools;

	Animation::ProcessEvent(event);

	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	switch (event.event) {
	case Cry::Entity::EEvent::Update:
	{
		const auto parent = get_parent(*m_pEntity);
		if (not parent and is_dead_)
			break;

		if (get_physics_status<pe_status_living>(*parent).vel.len2() >= 0.1f) {
			if (is_idle_) {
				is_idle_ = false;

				walk_action_ = make_animation_action(priority::low, walk_fragmend_id_, IAction::Interruptable);
				action_controller_->Queue(*walk_action_);
			}
		}
		else {
			if (walk_action_)
				walk_action_->Stop();

			is_idle_ = true;
		}

		if (is_attacking_) {
			is_attacking_ = false;

			attacking_action_ = make_animation_action(priority::medium, melee_fragment_id_);
			action_controller_->Queue(*attacking_action_);
		}

		if (is_stunned_) {
			is_stunned_ = false;

			if (attacking_action_)
				attacking_action_->Stop();

			stunned_action_ = make_animation_action(priority::high, hit_reaction_fwd_fragment_id_);
			action_controller_->Queue(*stunned_action_);
		}
	}
	break;

	case Cry::Entity::EEvent::AnimationEvent:
	{
		const auto* const animation_event = reinterpret_cast<const AnimEventInstance*>(event.nParam[0]);
		const auto* const character = reinterpret_cast<const ICharacterInstance*>(event.nParam[1]);
		if (stricmp(animation_event->m_EventName, "melee") == 0) {
			const auto bone_id = character->GetIDefaultSkeleton().GetJointIDByName(animation_event->m_BonePathName);
			const auto bone_transform = character->GetISkeletonPose()->GetAbsJointByID(bone_id);
			const auto melee_location = GetWorldTransformMatrix() * bone_transform.t;
			const auto min_box = melee_location + (DevelopmentTools::make_unit_vector3() * -1.0f);
			const auto max_box = melee_location + DevelopmentTools::make_unit_vector3();
			const auto entities_in_box = DevelopmentTools::get_entities_in_box(min_box, max_box, ent_living);
			for (const auto physical_entity : entities_in_box) {
				const auto entity = DevelopmentTools::search_entity_by_physics(physical_entity);
				if (const auto child = DevelopmentTools::get_child(*entity)) {
					if (GetEntityId() == child->GetId())
						continue;

					auto& event_system = BoomerShooterEventSystem::instance();
					event_system.send(child->GetId(), ExplosiveDamageEvent{ melee_location, ZERO, *child, 35 });

					const auto impulse_direction = GetWorldTransformMatrix().GetColumn1();
					DevelopmentTools::impulse_entity_by_physics(*entity, melee_location, impulse_direction * 850.0f);
				}
			}
			
#ifndef RELEASE
			if (const auto console_variable = gEnv->pConsole->GetCVar("dv_DebugTrigger")) {
				if (console_variable->GetIVal()) {
					debugger_->Begin("melee", false);
					debugger_->AddAABB(min_box, max_box, Col_Red, 5.0f);
				}
			}
#endif // !RELEASE
		}
	}
	break;

	}
}

Cry::Entity::EventFlags ZombieAnimation::GetEventMask() const
{
	return Animation::GetEventMask() | Cry::Entity::EEvent::AnimationEvent;
}

void ZombieAnimation::initialize_fragment()
{
	idle_fragmend_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "idle");
	walk_fragmend_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "walk");
	melee_fragment_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "melee");
	hit_reaction_fwd_fragment_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "hit_reaction_fwd");
	dead_fragment_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "dead");
}