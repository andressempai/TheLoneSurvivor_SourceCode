#include "StoreToMagazine.h"
#include "Components/Magazine/Magazine.h"
#include "Types/AmmunitionTypes.h"
#include "Types/InventoryTypes.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_store_to_magazine_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(StoreToMagazine));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_store_to_magazine_component);

void ReflectType(Schematyc::CTypeDesc<string>& desc)
{
	desc.SetGUID("{198A9805-19E4-4782-BDD1-DE518249EE50}"_cry_guid);
}

void ReflectType(Schematyc::CTypeDesc<StoreToMagazine>& desc)
{
	desc.SetGUID("{81ACD353-7F91-4361-8D87-ABAC7FC16284}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("storetomagazinecomponent"));
	desc.SetLabel("Store to Magazine");
	desc.SetDescription("Allow Ammunition to be Store inside Weapon Magazine when this entity is Collected");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton });
	desc.AddMember(
			&StoreToMagazine::audio_trigger_params_
		,	'audp'
		,	"audiotriggerparams"
		,	"Audio Trigger Params"
		,	"Parameters for Audio Trigger"
		,	AudioTriggerParams{}
	);
}

template <typename AmmoComponent>
bool store_in_magazine_impl(IEntity& collecting_entity, IEntity& ammo_entity, const string& audio_name)
{
	using namespace DevelopmentTools;

	if (entity_has_component<AmmoComponent>(ammo_entity)) {
		const auto inventory = get_component_from_entity<Types::Components::WeaponInventoryComponent>(collecting_entity);
		CRY_ASSERT(inventory.get() != nullptr, "Collecting Entity must have a Weapon Inventory");
		
		const auto weapon_iterator = inventory->get<WeaponBox<typename AmmoComponent::Tag>>();
		const auto weapon_box = std::get<WeaponBox<typename AmmoComponent::Tag>>((*weapon_iterator).value());
		const auto weapon_entity = search_entity_by_id(weapon_box.weapon_id());
		const auto weapon_magazine = get_component_from_entity<Magazine>(*weapon_entity);
		CRY_ASSERT(weapon_magazine.get() != nullptr, "Weapon Entity must have a Magazine Component");
		
		if (not weapon_magazine->is_full()) {
			static constexpr auto max_bullet_to_add = 1000;
			weapon_magazine->restore(max_bullet_to_add);

			// PROVICIONAL
			BoomerShooterEventSystem::instance().send(
					collecting_entity.GetId()
				,	MagazineUpdateEvent{ *weapon_entity, weapon_magazine->size(), weapon_magazine->capacity() }
			);
			// END PROVICIONAL

			play_2d_sound(audio_name.c_str());
			destroy_entity_by_id(ammo_entity.GetId());

			return true;
		}
	}

	return false;
}

template <template <typename...> typename TypeListLike, typename... AmmoComponent>
void store_in_magazine(
		IEntity& collecting_entity
	,	IEntity& ammo_entity
	,	const string& audio_name
	,	TypeListLike<AmmoComponent...>
)
{
	(store_in_magazine_impl<AmmoComponent>(collecting_entity, ammo_entity, audio_name) || ...);
}

void StoreToMagazine::Initialize()
{
	BoomerShooterEventSystem::instance().subscribe<CollectEvent>(GetEntityId(), [this](const EventBase& event)
	{
		const auto& collect_event = static_cast<const CollectEvent&>(event);
		store_in_magazine(
				collect_event.collector_.get()
			,	*m_pEntity
			,	audio_trigger_params_.collected_audio_trigger_name_
			,	Types::Components::DynamicVectorAmmunition{}
		);
	});
}

void StoreToMagazine::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<CollectEvent>(GetEntityId());
}