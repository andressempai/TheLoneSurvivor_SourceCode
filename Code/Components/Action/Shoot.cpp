#include "Shoot.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_shoot_action_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Shoot));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_shoot_action_component);

void ReflectType(Schematyc::CTypeDesc<Shoot>& desc)
{
	desc.SetGUID("{E5657836-4B5E-4A87-B3AD-ADBD5A072E76}"_cry_guid);
	desc.SetEditorCategory("Game Actions");
	desc.SetName(Schematyc::CTypeName("shootactioncomponent"));
	desc.SetLabel("Shoot");
	desc.SetDescription("Allow Entity to Shoot any Weapon");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
}

void Shoot::Initialize()
{
	DevelopmentTools::add_action_listener(this, "player");

	BoomerShooterEventSystem::instance().subscribe<SwitchWeaponEvent>(GetEntityId(), [this](const EventBase&)
	{
		is_switching_weapon_ = true;
	});

	BoomerShooterEventSystem::instance().subscribe<FinishSwitchWeaponEvent>(GetEntityId(), [this](const EventBase&)
	{
		is_switching_weapon_ = false;
	});
}

void Shoot::OnShutDown()
{
	DevelopmentTools::remove_action_listener(this);
	BoomerShooterEventSystem::instance().unsubscribe<SwitchWeaponEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<FinishSwitchWeaponEvent>(GetEntityId());
}

Cry::Entity::EventFlags Shoot::GetEventMask() const
{
	return Cry::Entity::EEvent::ChildAttached;
}

void Shoot::ProcessEvent(const SEntityEvent& event)
{
	weapon_id_ = static_cast<EntityId>(event.nParam[0]);
}

void Shoot::OnAction(const ActionId& action, int activation_mode, float value)
{
	if ((strcmp(action.c_str(), "shoot") != 0) or is_switching_weapon_)
		return;

	if (activation_mode == eAAM_OnPress)
		BoomerShooterEventSystem::instance().send(weapon_id_, PullTriggerEvent{ *m_pEntity });
	if (activation_mode == eAAM_OnRelease)
		BoomerShooterEventSystem::instance().send(weapon_id_, ReleaseTriggerEvent{});
}