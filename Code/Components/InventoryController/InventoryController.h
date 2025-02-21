#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../MetaProgramming.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

#include <array>
#include <optional>
#include <variant>
#include <algorithm>
#include <cassert>

template <typename TComponentData, typename... TItems>
class InventoryController final : public IEntityComponent {
public:
	using ItemList = MP::type_list<TItems...>;
	using Item = std::variant<TItems...>;
	using Box = std::optional<Item>;
	using Container = std::array<Box, MP::size_v<ItemList>>;
	using Iterator = typename Container::iterator;
	using ConstIterator = typename Container::const_iterator;
	using ReverseIterator = typename Container::reverse_iterator;
	using ConstReverseIterator = typename Container::const_reverse_iterator;

	friend void ReflectType(Schematyc::CTypeDesc<InventoryController>& desc)
	{
		desc.SetGUID(TComponentData::guid);
		desc.SetEditorCategory(TComponentData::category.data());
		desc.SetName(Schematyc::CTypeName(TComponentData::name.data()));
		desc.SetLabel(TComponentData::label.data());
		desc.SetDescription(TComponentData::description.data());
		desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	}

	static void register_component(Schematyc::CEnvRegistrationScope& scope)
	{
		scope.Register(Schematyc::EnvComponent::template MakeShared<InventoryController>(SCHEMATYC_SOURCE_FILE_INFO));
	}

	template <typename TItem>
	bool contain() const
	{
		static_assert(MP::has_type_v<TItem, ItemList>, "Type must be any of Inventory Parameter Pack.");
		return inventory_[MP::get_pos_v<TItem, ItemList>].has_value();
	}

	template <typename TItem>
	InventoryController::Iterator add_item(TItem item)
	{
		InventoryController::Iterator item_slot = get<TItem>();
		*item_slot = item;

		return item_slot;
	}

	template <typename TItem>
	[[nodiscard]] InventoryController::Iterator get()
	{
		return get_impl<TItem>(this);
	}

	template <typename TItem>
	[[nodiscard]] InventoryController::ConstIterator get() const
	{
		return get_impl<TItem>(this);
	}

	void clear()
	{
		std::for_each(std::begin(inventory_), std::end(inventory_), [](Box& item_box) noexcept
			{
				item_box.reset();
			});
	}

	Iterator begin()
	{
		return inventory_.begin();
	}

	Iterator end()
	{
		return inventory_.end();
	}

	ConstIterator begin() const
	{
		return inventory_.begin();
	}

	ConstIterator end() const
	{
		return inventory_.end();
	}

	ConstIterator cbegin() const
	{
		return inventory_.cbegin();
	}

	ConstIterator cend() const
	{
		return inventory_.cend();
	}

	ConstReverseIterator crbegin() const
	{
		return inventory_.crbegin();
	}

	ConstReverseIterator crend() const
	{
		return inventory_.crend();
	}

	[[nodiscard]] ConstIterator& iterator()
	{
		return iterator_;
	}

private:
	template <typename TItem, typename TThis>
	static InventoryController::Iterator get_impl(TThis&& self)
	{
		static_assert(MP::has_type_v<TItem, ItemList>, "Type must be any of Inventory Parameter Pack.");
		return std::next(self->inventory_.begin(), MP::get_pos_v<TItem, ItemList>);
	}

	Container inventory_{};
	ConstIterator iterator_{ begin() };
};

#endif // !INVENTORY_H