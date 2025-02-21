#include "Components/WeaponHolder/WeaponHolder.h"
#include "Types/WeaponTypes.h"
#include "DevelopmentTools.h"
#include "EventSystem.h"
#include "MetaProgramming.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

void register_weapon_holder_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(WeaponHolder));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_weapon_holder_component);

void ReflectType(Schematyc::CTypeDesc<WeaponHolder>& desc)
{
	desc.SetGUID("{708A9FDA-1952-41B1-961E-0E97B46C7D49}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("weaponholdercomponet"));
	desc.SetLabel("Weapon Holder");
	desc.SetDescription("Allow Entity to Hold Weapons");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

template <typename Weapon>
bool has_weapon_type_impl(ObjectPointer<IEntity> entity)
{
	return DevelopmentTools::get_component_from_entity<Weapon>(*entity);
}

template <template <typename...> typename TypeListLike, typename... Weapon>
bool has_weapon_type(ObjectPointer<IEntity> entity, TypeListLike<Weapon...>)
{
	return (has_weapon_type_impl<Weapon>(entity) or ...);
}

struct FindAttachmentAndSendEquipEvent {
	FindAttachmentAndSendEquipEvent(WeaponHolder& obj, IEntity& weapon)
		:	obj_{ obj }
		,	weapon_{ weapon }
	{}

	template <typename Weapon>
	bool operator()() const
	{
		if (DevelopmentTools::get_component_from_entity<Weapon>(weapon_)) {
			obj_.equipped_weapon_grip_ = Weapon::Data::grip_name_;

			auto& event_system = BoomerShooterEventSystem::instance();
			event_system.send(obj_.GetEntityId(), EquipWeaponEvent{ Weapon::Data::animation_tag_name_ });

			return true;
		}

		return false;
	}

	WeaponHolder& obj_;
	IEntity& weapon_;
};

Cry::Entity::EventFlags WeaponHolder::GetEventMask() const
{
	return Cry::Entity::EEvent::ChildAttached | Cry::Entity::EEvent::Update;
}

void WeaponHolder::ProcessEvent(const SEntityEvent& event)
{
	using namespace DevelopmentTools;
	using namespace Types::Components;

	switch (event.event) {
	case Cry::Entity::EEvent::ChildAttached:
	{
		const auto child_entity = search_entity_by_id(static_cast<EntityId>(event.nParam[0]));
		if (not has_weapon_type(child_entity, DynamicVectorWeapons{}))
			break;

		if (current_weapon_) {
			current_weapon_->DetachThis(IEntity::ATTACHMENT_KEEP_TRANSFORMATION);
			current_weapon_->Invisible(true);
		}

		if (current_weapon_ = child_entity) {
			MP::for_each_type(FindAttachmentAndSendEquipEvent{ *this, *current_weapon_ }, DynamicVectorWeapons{});
			current_weapon_->Invisible(false);
		}
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (gEnv->IsEditing() or m_pEntity->IsGarbage())
			break;

		const auto character = get_character_from_entity(*m_pEntity, 0);
		if (not character)
			return;

		if (current_weapon_) {
			const auto attachment_manager = make_ptr(character->GetIAttachmentManager());
			if (const auto attachment = make_ptr(attachment_manager->GetInterfaceByName(equipped_weapon_grip_.data())))
				current_weapon_->SetWorldTM(make_matrix34(attachment->GetAttWorldAbsolute()));
		}
	}
	break;

	}
}