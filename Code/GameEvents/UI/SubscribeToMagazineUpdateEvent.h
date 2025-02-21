#ifndef SUBSCRIBE_TO_MAGAZINE_UPDATE_EVENT_H
#define SUBSCRIBE_TO_MAGAZINE_UPDATE_EVENT_H

#include "DevelopmentTools.h"

#include <functional>

namespace GameEvents::UI {
	struct SubscribeToMagazineUpdateEvent {
		explicit SubscribeToMagazineUpdateEvent(IUIElement& ui_element);

		template <typename WeaponComponent>
		bool invoke_flash_function_impl(
				IEntity& entity
			,	IUIElement& ui_element
			,	std::string_view function_name
			,	int32 size
			,	int32 capacity
		)
		{
			if (DevelopmentTools::entity_has_component<WeaponComponent>(entity)) {
				SUIArguments args{};
				args.AddArgument(size);
				args.AddArgument(capacity);

				ui_element.CallFunction(function_name.data(), args);

				return true;
			}

			return false;
		}

		template <template <typename...> typename TypeListLike, typename... WeaponComponent>
		void invoke_flash_function(
				IEntity& entity
			,	IUIElement& ui_element
			,	int32 size
			,	int32 capacity
			,	TypeListLike<WeaponComponent...>
		)
		{
			static constexpr auto function_names = DevelopmentTools::make_array(
					"update_pistol_info"
				,	"update_american_shotgun_info"
			);

			(invoke_flash_function_impl<WeaponComponent>(
					entity
				,	ui_element
				,	function_names[MP::get_pos_v<WeaponComponent, TypeListLike<WeaponComponent...>>]
				,	size
				,	capacity
			) or ...);
		}

		void subcribe_to_event(EntityId id);
		void unscribe_to_event(EntityId id);

		std::reference_wrapper<IUIElement> ui_element_;
	};
}	// namespace GameEvents::UI

#endif // !SUBSCRIBE_TO_MAGAZINE_UPDATE_EVENT_H