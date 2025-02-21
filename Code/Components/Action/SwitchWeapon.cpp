#include "SwitchWeapon.h"
#include "Types/InventoryTypes.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_switch_weapon_action_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(SwitchWeapon));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_switch_weapon_action_component);

void ReflectType(Schematyc::CTypeDesc<SwitchWeapon>& desc)
{
	desc.SetGUID("{31133E40-1ED8-462A-8091-65F3B2ECBACD}"_cry_guid);
	desc.SetEditorCategory("Game Actions");
	desc.SetName(Schematyc::CTypeName("switchweaponcomponent"));
	desc.SetLabel("Switch Weapon Component");
	desc.SetDescription("Allow Entity to Switching between Weapons (Iterate over Inventory or Select One)");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

void SwitchWeapon::Initialize()
{
	DevelopmentTools::add_action_listener(this, "player");

	BoomerShooterEventSystem::instance().subscribe<FinishSwitchWeaponEvent>(GetEntityId(), [this](const EventBase&)
	{
		is_switching_weapon_ = false;
	});
}

void SwitchWeapon::OnShutDown()
{
	DevelopmentTools::remove_action_listener(this);
	BoomerShooterEventSystem::instance().unsubscribe<FinishSwitchWeaponEvent>(GetEntityId());
}

void SwitchWeapon::ProcessEvent(const SEntityEvent& event)
{
	using namespace Types::Components;

	if (gEnv->IsEditing() or m_pEntity->IsGarbage() or input_actions_.none())
		return;

	const auto inventory = DevelopmentTools::get_component_from_entity<WeaponInventoryComponent>(*m_pEntity);
	CRY_ASSERT(inventory != nullptr, "The Entity must have a Weapon Inventory Component");

	auto& gun_iterator = inventory->iterator();
	const auto temp = gun_iterator;
	const auto has_item = [](const WeaponInventoryComponent::Box& box)
	{
		return box.has_value();
	};

	if (input_actions_.test(next_gun)) {
		if (gun_iterator = std::find_if(std::next(gun_iterator), inventory->cend(), has_item);
			gun_iterator == inventory->end())
			//	DE SER NECESARIO SE NECESITARIA OTRO FIND PARA BUSCAR UN ELEMENTO QUE CUMPLA CON EL PREDICATE
			//	A PARTIR DEL PRIMER ELEMENTO, SIMILAR A COMO SE HIZO EN PREVIOUS GUN.
			gun_iterator = inventory->begin();
	}
	else if (input_actions_.test(previous_gun)) {
		auto reverse_begin = std::make_reverse_iterator(std::next(gun_iterator));
		auto reverse_end = inventory->crend();
		if (auto iter = std::find_if(std::next(reverse_begin), reverse_end, has_item); iter == reverse_end) {
			if (auto i = std::find_if(inventory->crbegin(), reverse_end, has_item); i != reverse_end)
				gun_iterator = std::prev(i.base());
		}
		else
			gun_iterator = std::prev(iter.base());
	}
	else if (input_actions_.test(select_pistol))
		gun_iterator = inventory->get<PistolWeaponBox>();
	else if (input_actions_.test(select_shotgun))
		gun_iterator = inventory->get<ShotgunWeaponBox>();

	auto before_attach_new_weapon = [this](auto&& weapon_box)
	{
		const auto weapon = DevelopmentTools::search_entity_by_id(weapon_box.weapon_id());
		BoomerShooterEventSystem::instance().send(weapon->GetId(), BeforeSwitchWeaponEvent{});
	};

	auto attach_new_weapon = [this](auto&& weapon_box)
	{
		const auto weapon = DevelopmentTools::search_entity_by_id(weapon_box.weapon_id());

		TimerManager::instance().make_timer(parent_to_entity_timer_id_, 0.55f, false, [=]
		{
			m_pEntity->AttachChild(weapon);
		});

		BoomerShooterEventSystem::instance().send(GetEntityId(), SwitchWeaponEvent{""});
	};

	if (gun_iterator != temp) {
		std::visit(before_attach_new_weapon, temp->value());
		std::visit(attach_new_weapon, gun_iterator->value());

		is_switching_weapon_ = true;
	}

	input_actions_.reset();
}

Cry::Entity::EventFlags SwitchWeapon::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void SwitchWeapon::OnAction(const ActionId& action, int activation_mode, float value)
{
	if (is_switching_weapon_)
		return;

	if (activation_mode == eAAM_OnPress)
		if ((strcmp(action.c_str(), "equip_next_gun") == 0))
			input_actions_.set(next_gun);
		else if ((strcmp(action.c_str(), "equip_previous_gun") == 0))
			input_actions_.set(previous_gun);
		else if ((strcmp(action.c_str(), "equip_pistol") == 0))
			input_actions_.set(select_pistol);
		else if ((strcmp(action.c_str(), "equip_shotgun") == 0))
			input_actions_.set(select_shotgun);
}