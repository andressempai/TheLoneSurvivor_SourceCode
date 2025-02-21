#include "Magazine.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <algorithm>

void register_magazine_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Magazine));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_magazine_component);

void ReflectType(Schematyc::CTypeDesc<Magazine>& desc)
{
	desc.SetGUID("{189A556E-B45C-4C9D-9B29-615FD75B6C76}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("magazinecomponent"));
	desc.SetLabel("Magazine");
	desc.SetDescription("The Weapon Magazine");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&Magazine::size_
		,	'size'
		,	"size"
		,	"Size"
		,	"Size"
		,	Magazine::PositiveInt{}
	);

	desc.AddMember(
			&Magazine::capacity_
		,	'capa'
		,	"capacity"
		,	"Capacity"
		,	"Capacity"
		,	Magazine::PositiveInt{}
	);
}

int32 Magazine::remove(const int32 amount)
{
	const auto temp = size_.value > amount ? amount : size_.value;
	size_.value = std::clamp(size_.value - amount, 0, capacity_.value);

	return temp;
}

void Magazine::restore(const int32 amount)
{
	size_.value = std::clamp(size_.value + amount, 0, capacity_.value);
}

void Magazine::Initialize()
{
	BoomerShooterEventSystem::instance().subscribe<PullTriggerEvent>(GetEntityId(), [this](const EventBase&)
	{
		if (TimerManager::instance().is_timer_active(cooldown_timer_id_))
			return;

		can_remove_bullet_ = true;

		TimerManager::instance().make_timer(cooldown_timer_id_, cooldown_time_, false, nullptr);
	});
}

void Magazine::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<PullTriggerEvent>(GetEntityId());
}

Cry::Entity::EventFlags Magazine::GetEventMask() const
{
	return Cry::Entity::EEvent::AttachedToParent | Cry::Entity::EEvent::Update;
}

void Magazine::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	switch (event.event) {
	case Cry::Entity::EEvent::AttachedToParent:
	{
		parent_id_ = static_cast<EntityId>(event.nParam[0]);
		BoomerShooterEventSystem::instance().send(parent_id_, MagazineUpdateEvent{ *m_pEntity, size_, capacity_ });
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (can_remove_bullet_) {
			can_remove_bullet_ = false;
			remove(1);
			BoomerShooterEventSystem::instance().send(parent_id_, MagazineUpdateEvent{ *m_pEntity, size_, capacity_ });
		}
	}
	break;

	}
}

IEntityComponent::ComponentEventPriority Magazine::GetEventPriority() const
{
	return 13;
}