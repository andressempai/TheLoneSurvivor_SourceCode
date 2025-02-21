#include "EntityEventListeners/EntityAttachEventListener.h"
#include "Types/InventoryTypes.h"
#include "Types/WeaponTypes.h"
#include "DevelopmentTools.h"

namespace EntityEventListeners {
	template <typename WeaponComponent>
	bool store_into_inventory_impl(Types::Components::WeaponInventoryComponent& inventory, IEntity& weapon_entity)
	{
		if (DevelopmentTools::get_component_from_entity<WeaponComponent>(weapon_entity)) {
			const auto weapon_iter = inventory.add_item(
				WeaponBox<typename WeaponComponent::Tag>{ "", weapon_entity.GetId() }
			);

			auto& iterator = inventory.iterator();
			iterator = weapon_iter;

			return true;
		}

		return false;
	}

	template <template <typename...> typename TypeListLike, typename... WeaponComponent>
	void store_into_inventory(
			Types::Components::WeaponInventoryComponent& inventory
		,	IEntity& weapon_entity
		,	TypeListLike<WeaponComponent...>
	)
	{
		(store_into_inventory_impl<WeaponComponent>(inventory, weapon_entity) || ...);
	}

	void EntityAttachEventListener::OnEntityEvent(IEntity* pEntity, const SEntityEvent& event)
	{
		using namespace DevelopmentTools;

		if (const auto parent = search_entity_by_id(static_cast<EntityId>(event.nParam[0]))) {
			if (const auto inventory = get_component_from_entity<Types::Components::WeaponInventoryComponent>(*parent))
				store_into_inventory(*inventory, *pEntity, Types::Components::DynamicVectorWeapons{});

			pEntity->RemoveEventListener(event_to_listen, this);
		}
	}
}	// namespace EntityEventListeners