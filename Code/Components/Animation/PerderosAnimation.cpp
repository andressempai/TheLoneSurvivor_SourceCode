#include "PerderosAnimation.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_perderos_animation_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(PerderosAnimation));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_perderos_animation_component);

void ReflectType(Schematyc::CTypeDesc<PerderosAnimation>& desc)
{
	desc.AddBase<Animation>();
	desc.SetGUID("{73D2CF59-44F2-4270-96CF-A58E0FEB0BAC}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("perderosanimationcomponent"));
	desc.SetLabel("Perderos Animation");
	desc.SetDescription("This component is used to animate Perdero Entity.");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&Animation::animation_data_
		,	'adat'
		,	"animationdata"
		,	"Animation Data"
		,	"Animation Data"
		, Animation::AnimationData{}
	);
}

void PerderosAnimation::Initialize()
{
	Animation::Initialize();

	if (not controller_definition_ or not action_controller_)
		return;

	BoomerShooterEventSystem::instance().subscribe<DamageReactionEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (is_reactiving_to_damage_)
			return;

		is_reactiving_to_damage_ = true;

		if (hit_reaction_action_)
			hit_reaction_action_->Stop();

		hit_reaction_action_ = new TAction<SAnimationContext>(0, hit_reaction_fwd_fragment_id_);
		action_controller_->Queue(*hit_reaction_action_);
	});

	BoomerShooterEventSystem::instance().subscribe<MelleeAttackEvent>(GetEntityId(), [this](const EventBase&)
	{
		action_->Stop();
		action_ = new TAction<SAnimationContext>(0, melee_fragment_id_);
		action_controller_->Queue(*action_);
	});

	BoomerShooterEventSystem::instance().subscribe<DeadEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (is_dead_)
			return;

		is_dead_ = true;
		action_->Stop();
		action_ = new TAction<SAnimationContext>(0, dead_fragment_id_);
		action_controller_->Queue(*action_);
	});

#ifndef RELEASE
	debugger_ = gEnv->pGameFramework->GetIPersistantDebug();
#endif // !RELEASE
}

void PerderosAnimation::OnShutDown()
{
	Animation::OnShutDown();

	BoomerShooterEventSystem::instance().unsubscribe<DamageReactionEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<MelleeAttackEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<DeadEvent>(GetEntityId());
}

Cry::Entity::EventFlags PerderosAnimation::GetEventMask() const
{
	return Animation::GetEventMask() | Cry::Entity::EEvent::AnimationEvent;
}

void PerderosAnimation::ProcessEvent(const SEntityEvent& event)
{
	Animation::ProcessEvent(event);

	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	switch (event.event) {
	case Cry::Entity::EEvent::Update:
	{
		const auto parent = DevelopmentTools::get_parent(*m_pEntity);
		if (not parent)
			break;

		const auto desired_fragment_id = current_fragment_id_;
		if (DevelopmentTools::get_physics_status<pe_status_living>(*parent).vel.len2() > 5.5f) {
			if (current_fragment_id_ != walk_fragmend_id_)
				current_fragment_id_ = walk_fragmend_id_;
		}
		else
			if (current_fragment_id_ != idle_fragmend_id_)
				current_fragment_id_ = idle_fragmend_id_;

		if (desired_fragment_id != current_fragment_id_) {
			if (action_)
				action_->Stop();

			action_ = new TAction<SAnimationContext>(Lower, current_fragment_id_);
			action_controller_->Queue(*action_);
		}
	}
	break;

	case Cry::Entity::EEvent::AnimationEvent:
	{
		auto* const animation_event = reinterpret_cast<const AnimEventInstance*>(event.nParam[0]);
		auto* const character = reinterpret_cast<const ICharacterInstance*>(event.nParam[1]);
		const auto* const event_name = animation_event->m_EventName;
		
		if ((stricmp(event_name, "end_melee") == 0) or (stricmp(event_name, "end_hit_reaction") == 0)) {
			if (action_)
				action_->Stop();

			action_ = new TAction<SAnimationContext>(0, current_fragment_id_);
			action_controller_->Queue(*action_);

			is_reactiving_to_damage_ = false;
		}

		if (stricmp(event_name, "melee") == 0) {
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

void PerderosAnimation::initialize_fragment()
{
	idle_fragmend_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "idle");
	walk_fragmend_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "walk");
	melee_fragment_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "melee");
	hit_reaction_fwd_fragment_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "hit_reaction_fwd");
	dead_fragment_id_ = DevelopmentTools::find_fragment_id(*controller_definition_, "dead");
}